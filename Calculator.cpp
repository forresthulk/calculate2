// Calculator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "pch.h"
#include"stdlib.h"
#include<iostream>
#include<cstdio>
#include<string>
#include<cmath>
#include<algorithm>
#include<time.h>
#include<sys/timeb.h>
#include<fstream>
using namespace std;
struct fraction {//存储分数的数据结构
	int p;//分子
	int q;//分母
};
class Giveequation {
public:
	Giveequation();//构造函数
	string getequation();//提供读取生成式子的接口
	void randequation();//生成式子
private :
	string equation;//存储生成的式子
	
};
class Countequation {
public:
	Countequation();
	int count(string qequation);//计算式子的答案
	int gcd(int a, int b);//求最大公约数
	
private:
	string equation;//保存需要计算的算式
	int length;//式子字符串的长度
	int numberstack[21];//存放逆波兰表达式
	int numberptr;
	int signstack[10];//生成逆波兰表达式的辅助堆栈
	int signptr;
	int power(int a, int b);//求乘方的运算
};
class Givefraction {
public:
	Givefraction();
	string getequation();//提供读取式子的接口
	void randequation();//生成式子
private:
	string equation;
};

class Countfraction {
public:
	Countfraction();
	string count(string qfraction);
	int gcd(int a, int b);
private:
	string thfraction;//存储读入的式子
	int length;
	fraction numberstack[11];//存储逆波兰表达式的堆栈
	int numberstackptr;
	fraction signstack[5];//生成逆波兰表达式的辅助堆栈
	int signstackptr;
};

Giveequation::Giveequation() {
	equation = "";
	
}
string Giveequation::getequation() {
	return equation;
}
void Giveequation::randequation() {
	//srand((int)time(NULL));
	int num = rand() % 10 + 1;//符号数至多为10，最少为1
	int numeral[11];//最多可有11个数字
	string sign[10];//存放符号
	for (int i = 0; i < 11; i++) {//初始化数字数组
		numeral[i] = 0;
	}
	for (int i = 0; i < num + 1; i++) {
		numeral[i] = rand() % 99+1;//避免除数是零
	}
	for (int i = 0; i < num ; i++) {//随机生成符号
		int x = rand() % 5;
		if (0 == x) sign[i] = "+";
		if (1 == x) sign[i] = "-";
		if (2 == x) {
			if (sign[i - 1] == "+" || sign[i - 1] == "-") {
				sign[i] = "*";
			}
			else {
				i--;
			}
		}
		if (3 == x) {
			if (sign[i - 1] == "+" || sign[i - 1] == "-") {
				sign[i] = "/";
				numeral[i + 1] %= 10;
				numeral[i + 1]++;
			}
			else {
				i--;
			}
		}
		if (4 == x) {
			sign[i] = "^";
			if (sign[i - 1] == "^") {//避免连续两个符号均为乘方的情况
				i--;
			}
		}
	}
	
	for (int i = 0; i < num; i++) {//保证被除数可以整除除数
		if (sign[i] == "/") {
			numeral[i] -= (numeral[i] % numeral[i + 1]);
		}
	}
	for (int i = 0; i < num; i++) {
		if (sign[i] == "^") {
			numeral[i + 1] = numeral[i + 1] % 3 + 1;//过高次数的乘方可能会导致溢出
		}
	}
	for (int i = 0; i < num; i++) {	//随机生成了数字和运算符号，将它们转化成字符串并存储下来
		equation+= to_string(numeral[i]);
		equation += sign[i];
	}
	equation += to_string(numeral[num]);
}

int Countequation::power(int a, int b) {//计算乘方
	int answer = a;
	while (b > 1) {
		answer *= a;
		b--;
	}
	return answer;
}
Countequation::Countequation() {
	equation = "";
	length = 0;
	numberptr = 0;
	signstack[0] = 200;//栈底为优先级最低的运算符
	signptr = 1;
}
/*整式计算*/
int Countequation::count(string qequation) {
	equation = qequation;
	length = equation.length();
	int number[11];
	string sign[10];
	int start = 0, end = 0;
	int count_of_num = 0;
	/* 将字符串中的数字和字符提取出来*/
	for (int i = 0; i < length; i++) {
		if (equation[i] > '9'|| equation[i] < '0') {
			if ((end - start) == 2) {//数字是两位数
				number[count_of_num] = 10 * (equation[start] - '0') + equation[start + 1] - '0';
			}
			else {//一位数
				number[count_of_num] = equation[start] - '0';
			}
			sign[count_of_num] = equation[end];//记录下符号
			count_of_num++;
			end++;
			start = end;
		}
		else {
			end++;
		}
		if (i == length - 1) {
			if ((end - start) == 2) {//数字是两位数
				number[count_of_num] = 10 * (equation[start] - '0') + equation[start + 1] - '0';
			}
			else {//一位数
				number[count_of_num] = equation[start] - '0';
			}
		}
	}
	int midstack[22] = {0};
	int midptr = 0;
	cout << endl;
	/*将中序表达式存储在一个临时数组中*/
	for (int i = 0; i < count_of_num; i++) {
		midstack[2 * i] = number[i];
		if (sign[i] == "+") { midstack[2 * i + 1] = 201; }
		if (sign[i] == "-") { midstack[2 * i + 1] = 202; }
		if (sign[i] == "*") { midstack[2 * i + 1] = 203; }
		if (sign[i] == "/") { midstack[2 * i + 1] = 204; }
		if (sign[i] == "^") { midstack[2 * i + 1] = 205; }
	}
	midstack[count_of_num*2]= number[count_of_num];
	/*for (int i = 0; i <= count_of_num * 2; i++) {
		cout << midstack[i]<<" ";
	}
	*/
	/* 生成算式的逆波兰表达式*/
	for (int i = 0; i < count_of_num * 2 + 1; i++) {
		if (midstack[i] < 200) {//取出的是数字，直接压栈
			numberstack[numberptr] = midstack[i];
			numberptr++;
		}
		else {//取出了符号
			while (midstack[i] <= signstack[signptr - 1]) {//将符号栈里优先级大于等于当前符号的符号全部弹出并压入表达式栈
				numberstack[numberptr] = signstack[signptr - 1];
				numberptr++;
				signptr--;
			}
			signstack[signptr] = midstack[i];//将当前符号入符号栈
			signptr++;
		}
	}
	/*经过上述操作，仍有符号留在符号栈里，将剩余符号压入表达式栈中*/
	while (signptr > 1) {
		numberstack[numberptr] = signstack[signptr - 1];
		numberptr++;
		signptr--;
	}
	/*
	for (int i = 0; i < numberptr; i++) {
		cout << numberstack[i] << " ";
	}*/
	/* 计算逆波兰表达式的值*/
	midptr = 0;
	for (int i = 0; i < 22; i++) {//初始化中间栈
		midstack[i] = 0;
	}
	for (int i = 0; i < numberptr; i++) {
		if (numberstack[i] < 200) {//若取出的是数字，压栈
			midstack[midptr] = numberstack[i];
			midptr++;
		}
		else {
			if (numberstack[i] == 201) {
				midstack[midptr - 2] += midstack[midptr - 1];
				midptr--;
			}
			if (numberstack[i] == 202) {
				midstack[midptr - 2] -= midstack[midptr - 1];
				midptr--;
			}
			if (numberstack[i] == 203) {
				midstack[midptr - 2] *= midstack[midptr - 1];
				midptr--;
			}
			if (numberstack[i] == 204) {
				midstack[midptr - 2] /= midstack[midptr - 1];
				midptr--;
			}
			if (numberstack[i] == 205) {
				midstack[midptr - 2] =power(midstack[midptr - 2], midstack[midptr - 1]);
				midptr--;
			}
		}
	}
	//cout << endl;
	//cout << midstack[0];
	return midstack[0];
}
/*gcd用来计算分子和分母之间的最大公约数*/
int Countequation::gcd(int a, int b) {
	int c = 0;
	if (a < b) {
		c = a;
		a = b;
		b = c;
	}
	while (1) {
		c = a % b;
		if (c == 0) {
			return b;
		}
		if (c == 1) {
			return 1;
		}
		a = b;
		b = c;
	}
}
/* 分数计算类的构造函数*/
Countfraction::Countfraction() {
	thfraction = "";
	length = 0;
	numberstackptr = 0;
	signstackptr = 1;
	signstack[0].p = signstack[0].q = 20;
}
int Countfraction::gcd(int a, int b) {
	int c = 0;
	if (a < b) {
		c = a;
		a = b;
		b = c;
	}
	while (1) {
		c = a % b;
		if (c == 0) {
			return b;
		}
		if (c == 1) {
			return 1;
		}
		a = b;
		b = c;
	}

}

string Countfraction::count(string qfraction) {
	thfraction = qfraction;
	length = thfraction.length();
	fraction number[6];
	string sign[5];
	int signptr = 0, numberptr = 0;

	for (int i = 0; i < length; i++) {
		if (thfraction[i] == ' ') { continue; }
		if (thfraction[i] > '0'&&thfraction[i] <= '9') {//读到数字的情况
			if (numberptr%2==0) {//之前取了偶数个数字，当前取出的数是分子
				number[numberptr / 2].p = thfraction[i]-'0';
			}
			else {//取出的数为分母
				number[numberptr / 2].q = thfraction[i]-'0';
			}
			numberptr++;

		}
		else {//读到符号
			if (signptr%2 != 0) {//读到的是运算符号
				sign[signptr / 2] = thfraction[i];
			}
			signptr++;
		}
	}
	
	fraction midstack[11];
	numberptr /= 2;//表示实际有几个分数
	for (int i = 0; i < numberptr; i++) {
		midstack[i*2].p = number[i].p;
		midstack[i*2].q = number[i].q;
		if (i != numberptr - 1) {//读到的不是最后一个数
			if (sign[i] == "+") { midstack[i * 2 + 1].p = midstack[i * 2 + 1].q= 21; }
			if (sign[i] == "-") { midstack[i * 2 + 1].p = midstack[i * 2 + 1].q= 22; }
			if (sign[i] == "*") { midstack[i * 2 + 1].p = midstack[i * 2 + 1].q=23; }
			if (sign[i] == "/") { midstack[i * 2 + 1].p = midstack[i * 2 + 1].q=24; }
		}
		
	}

	
	for (int i = 0; i < 2 * numberptr - 1; i++) {
		if (midstack[i].p < 20) {//读到数字，直接压栈
			numberstack[numberstackptr].p = midstack[i].p;
			numberstack[numberstackptr].q = midstack[i].q;
			numberstackptr++;
		}
		else {//取出了符号
			while (midstack[i].p <= signstack[signstackptr-1].p) {//将符号栈里优先级大于等于当前符号的符号全部弹出并压入表达式栈
				numberstack[numberstackptr] = signstack[signstackptr - 1];
				numberstackptr++;
				signstackptr--;
			}
			signstack[signstackptr] = midstack[i];//将当前符号入符号栈
			signstackptr++;
		}
		
	}
	while (signstackptr > 1) {
		numberstack[numberstackptr] = signstack[signstackptr - 1];
		numberstackptr++;
		signstackptr--;
	}
	signstack[0].p = signstack[0].q = 0;
	signstackptr = 0;//符号栈之后用不到了，可以把它用作逆波兰表达式中的中间栈
	
	
	for (int i = 0; i < numberstackptr; i++) {
		if (numberstack[i].p < 20) {//读到数字，压入中间栈
			signstack[signstackptr].p = numberstack[i].p;
			signstack[signstackptr].q = numberstack[i].q;
			signstackptr++;
		}
		else {//读到符号，进行运算
			/*加法计算，分母相乘，分子互相乘对方的分母并相加*/
			if (numberstack[i].p == 21) {
				int midp = signstack[signstackptr - 1].p*signstack[signstackptr - 2].q+ signstack[signstackptr - 1].q*signstack[signstackptr - 2].p;
				int midq = signstack[signstackptr - 1].q*signstack[signstackptr - 2].q;
				int midgcd = gcd(midp, midq);
				midp /= midgcd;//约分
				midq /= midgcd;
				signstack[signstackptr - 2].p = midp;
				signstack[signstackptr - 2].q = midq;
				signstackptr--;
			}
			/*减法计算，通分后分子互相乘以另一个数的分母并相减*/
			if (numberstack[i].p == 22) {
				int midp = signstack[signstackptr - 1].q*signstack[signstackptr - 2].p-signstack[signstackptr - 1].p*signstack[signstackptr - 2].q;
				int midq = signstack[signstackptr - 1].q*signstack[signstackptr - 2].q;
				int midgcd = gcd(midp, midq);
				midp /= midgcd;//约分
				midq /= midgcd;
				signstack[signstackptr - 2].p = midp;
				signstack[signstackptr - 2].q = midq;
				signstackptr--;
			}
			/*乘法计算，分子乘以分子，分母乘以分母*/
			if (numberstack[i].p == 23) {
				int midp = signstack[signstackptr - 1].p*signstack[signstackptr - 2].p;
				int midq = signstack[signstackptr - 1].q*signstack[signstackptr - 2].q;
				int midgcd = gcd(midp, midq);
				midp /= midgcd;//约分
				midq /= midgcd;
				signstack[signstackptr - 2].p = midp;
				signstack[signstackptr - 2].q = midq;
				signstackptr--;
			}
			/*除法，被除数分子乘以除数分母，除数分子乘以被除数分母*/
			if (numberstack[i].p == 24) {
				int midp = signstack[signstackptr - 1].q*signstack[signstackptr - 2].p;
				int midq = signstack[signstackptr - 1].p*signstack[signstackptr - 2].q;
				int midgcd = gcd(midp, midq);
				midp /= midgcd;//约分
				midq /= midgcd;
				signstack[signstackptr - 2].p = midp;
				signstack[signstackptr - 2].q = midq;
				signstackptr--;
			}
			
		}
	}
	if (signstack[0].q < 0) {//分母为负数，分子分母均为负可以约去，仅分母为负数将符号提到前面
		signstack[0].q = -signstack[0].q;
		signstack[0].p = -signstack[0].p;
	}
	/*
	cout << endl;
	cout << signstack[0].p<<"/"<<signstack[0].q<<endl;*/
	string answer = "";
	answer += to_string(signstack[0].p) + "/" + to_string(signstack[0].q);
	return answer;
}


Givefraction::Givefraction() {
	equation = "";
}
string Givefraction::getequation() {
	return equation;
};

void Givefraction::randequation() {
	int num = rand() % 5 + 1;//至多五个运算符号
	
	/*生成数字*/
	fraction number[6];
	string sign[5];
	for (int i = 0; i < num+1; i++) {
		number[i].q = rand() % 9 + 1;
		number[i].p = rand() % number[i].q+1;//真分数分子小于等于分母
	}
	/*生成符号*/
	//cout << num << endl;
	for (int i = 0; i < num; i++) {
		int number2sign=rand()%4;
		if (number2sign == 0) { sign[i] = "+"; }
		if (number2sign == 1) { sign[i] = "-"; }
		if (number2sign == 2) { 
			sign[i] = "*"; 
			if (sign[i - 1] == "*" || sign[i - 1] == "/") {
				i--;
			}
		}
		if (number2sign == 3) { 
			sign[i] = "/";
			if (sign[i - 1] == "*" || sign[i - 1] == "/") {
				i--;
			}
		} 
	}
	/*
	for (int i = 0; i < num; i++) {
		cout << sign[i]<<endl;
	}*/
	for (int i = 0; i < num; i++) {
		equation += to_string(number[i].p);
		equation += "/";
		equation += to_string(number[i].q);
		equation += " ";
		equation += sign[i];
		equation += " ";
	}
	equation += to_string(number[num].p);
	equation += "/";
	equation += to_string(number[num].q);
	//cout << equation << endl;
};


void UI() {
	int controlflag=0;
	int right = 0, wrong = 0;//记录用户对错的题目数量
	cout << "请输入要求" << endl<<"输入1进行整数计算"<<endl<<"输入2进行分数运算"<<endl<<"输入3生成整数考题"<<endl<<"输入4生成分数考题"<<endl;
	cin >> controlflag;
	
	if (1 == controlflag) {//做整数题
		for (int i = 0; i < 1000; i++) {
			Giveequation myequation;
			Countequation mycount;
			int useranswer = -1;//存储用户输入的答案
			myequation.randequation();//生成式子
			string thisequation = myequation.getequation();
			cout << thisequation << endl;//输出题目
			int answer = mycount.count(thisequation);
			cin >> useranswer;
			if (useranswer == answer) { right++; }
			else { wrong++; }
			cout << "你做对了" << right << "道题" << "," << "做错了" << wrong << "道题" << endl;
		}
	}
	if (2 == controlflag) {//做分数计算
		for (int i = 0; i < 1000; i++) {
			Givefraction myequation;
			Countfraction mycount;
			string useranswer ="";//存储用户输入的答案
			myequation.randequation();//生成式子
			string thisequation = myequation.getequation();
			cout << thisequation << endl;//输出要求回答的题目
			string answer = mycount.count(thisequation);
			cin >> useranswer;
			if (useranswer == answer) { right++; }
			else { wrong++; }
			cout << "你做对了" << right << "道题" << "," << "做错了" << wrong << "道题" << endl;
		}
	}
	if (3 == controlflag) {//生成整数计算题
		ofstream ofile;
		string equation[1000];
		for (int i = 0; i < 1000; i++) {
			Giveequation myequation;
			myequation.randequation();
			equation[i] = myequation.getequation();
		}
		ofile.open("equation.txt");
		for (int i = 0; i < 1000; i++) {
			ofile << equation[i] << endl;
		}
	}
	if (4 == controlflag) {//生成分数计算题
		ofstream ofile;
		string equation[1000];
		for (int i = 0; i < 1000; i++) {
			Givefraction myequation;
			myequation.randequation();
			equation[i] = myequation.getequation();
		}
		ofile.open("fraction.txt");
		for (int i = 0; i < 1000; i++) {
			ofile << equation[i] << endl;
		}

	}
	if (controlflag < 1 || controlflag>4) {
		cout << " 请输入正确参数"<<endl;
	}
}


int main()
{
	UI();

	return 0;
}

