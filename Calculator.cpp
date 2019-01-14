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

class Giveequation {
public:
	Giveequation();
	string getequation();
	void randequation();
private :
	string equation;

};
class Countequation {
public:
	Countequation();
	string count(string qequation);
	int gcd(int a, int b);
	
private:
	string equation;//保存需要计算的算式
	int length;
	int numberstack[21];
	int numberptr;
	int signstack[10];
	int signptr;
};


Giveequation::Giveequation() {
	equation = "";
	
}
string Giveequation::getequation() {
	return equation;
}

void Giveequation::randequation() {
	srand((int)time(NULL));
	int num = rand() % 10 + 1;//符号数至多为10，最少为1
	int numeral[11];//最多可有11个数字
	string sign[10];
	for (int i = 0; i < 11; i++) {
		numeral[i] = 0;
	}
	for (int i = 0; i < num + 1; i++) {
		numeral[i] = rand() % 99+1;//避免除数是零
	}
	for (int i = 0; i < num ; i++) {
		int x = rand() % 4;
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
		if (3 == x)
			if (sign[i - 1] == "+" || sign[i - 1] == "-") {
				sign[i] = "/";
				numeral[i + 1] %= 10;
				numeral[i + 1]++;
			}
			else {
				i--;
			}
	}
	//for(int i=0;i<)
	for (int i = 0; i < num; i++) {
		if (sign[i] == "/") {
			numeral[i] -= (numeral[i] % numeral[i + 1]);
		}
	}




	for (int i = 0; i < num; i++) {
		//cout << numeral[i];
		equation+= to_string(numeral[i]);
		equation += sign[i];
		//cout << equation[2 * i] << sign[i];
	}
	equation += to_string(numeral[num]);
	cout << equation << endl;


}



Countequation::Countequation() {
	equation = "";
	length = 0;
	numberptr = 0;
	signstack[0] = 200;//栈底为优先级最低的运算符
	signptr = 1;
}

string Countequation::count(string qequation) {
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
	/*
	for (int i = 0; i < count_of_num; i++) {
		cout << number[i] << sign[i];
	}
	cout << number[count_of_num];
*/
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
	}
	midstack[count_of_num*2]= number[count_of_num];
	for (int i = 0; i <= count_of_num * 2; i++) {
		cout << midstack[i]<<" ";
	}

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
	cout << endl;
	cout << numberptr;
	cout << endl;
	for (int i = 0; i < numberptr; i++) {
		cout << numberstack[i] << " ";
	}
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
		}
	}
	cout << endl;
	cout << midstack[0];
	return equation;
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


/*int languageNum, num_i = 4;
string str = "";
int random(double start, double end)
{
	return (int)(start + (end - start)*rand() / (RAND_MAX + 1.0));
}   //随机数函数

int gcd(int a, int b)       //定义函数gcd 计算 
{
	if (a%b == 0)
		return b;
	else
		return gcd(b, a%b);
}////求最大公约数
void strout1(int fenzi, int fenmu)   //根据分子分母随机生成真分数式子
{
	int f = 1 + random(0, 50);
	if (f < 25) {
		if (num_i > 2)
		{
			num_i--;
			int x = 1;
			int y = 1 + random(fenmu, fenmu + 3);
			strout1(x, y);
			int sum1 = fenzi * y - x * fenmu;
			int sum2 = fenmu * y;
			int gcd1 = gcd(sum2, sum1);
			x = sum1 / gcd1; y = sum2 / gcd1;
			str = to_string(x) + "/" + to_string(y) + "+" + str;
		}
		else
		{
			int x = 1;
			int y = 1 + random(fenmu, fenmu + 3);
			int sum1 = fenzi * y - x * fenmu;
			int sum2 = fenmu * y;
			int gcd1 = gcd(sum2, sum1);
			int x1 = sum1 / gcd1; int y1 = sum2 / gcd1;
			str = str + to_string(x) + "/" + to_string(y) + "+" + to_string(x1) + "/" + to_string(y1);
		}
	}
	else if (f >= 25 && f < 50) {
		if (num_i > 2)
		{
			num_i--;
			int x = random(1, 10);
			int y = 1 + random(1, 15);
			strout1(x, y);
			int sum1 = fenzi * y + x * fenmu;
			int sum2 = fenmu * y;
			int gcd1 = gcd(sum2, sum1);
			int x1 = sum1 / gcd1;
			int y1 = sum2 / gcd1;
			str = to_string(x1) + "/" + to_string(y1) + "-" + str;
		}
		else
		{
			int x = random(1, 10);
			int y = 1 + random(1, 15);
			int sum1 = fenzi * y + x * fenmu;
			int sum2 = fenmu * y;
			int gcd1 = gcd(sum2, sum1);
			int x1 = sum1 / gcd1;
			int y1 = sum2 / gcd1;
			str = str + to_string(x1) + "/" + to_string(y1) + "-" + to_string(x) + "/" + to_string(y);
		}

	}

}    //生成
void strout(int ans)           //根据随机数随机生成整数运算式
{
	int f = 1 + random(0, 100);
	if (f < 25) {// +
		if (num_i > 2) {
			num_i--;
			int x = random(1, ans);
			strout(x);
			int y = ans - x;
			if (y >= 0)
				str = to_string(y) + "+" + str;

		}
		else {
			int x = random(1, ans);
			int y = ans - x;
			if (y >= 0)
				str = str + to_string(y) + "+" + to_string(x);
		}
	}
	if (f >= 25 && f < 50) {// -
		if (num_i > 2) {
			num_i--;
			int x = random(1, ans);
			strout(x);
			int y = ans + x;
			str = to_string(y) + "-" + "(" + str + ")";

		}
		else {
			int x = random(1, ans);
			int y = ans + x;
			str = str + to_string(y) + "-" + to_string(x);
		}
	}
	if (f >= 50 && f < 75) {// *
		if (num_i > 2) {
			num_i--;
			int x = 0;
			for (int i = (int)sqrt(ans); i > 0; i++) {
				if (ans%i == 0) {
					x = i;
					break;
				}
			}
			int y = ans / x;
			strout(x);
			str = to_string(y) + "*" + "(" + str + ")";

		}
		else {
			int x = 0;
			for (int i = (int)sqrt(ans); i > 0; i++) {
				if (ans%i == 0) {
					x = i;
					break;
				}
			}
			int y = ans / x;
			str = str + to_string(y) + "*" + to_string(x);
		}
	}
	if (f >= 75) {// /
		if (num_i > 2) {
			num_i--;
			int x = 1 + random(0, 10);
			strout(x);
			int y = ans * x;
			str = to_string(y) + "÷" + "(" + str + ")";

		}
		else {
			int x = 1 + random(0, 10);
			int y = ans * x;
			str = str + to_string(y) + "÷" + to_string(x);
		}
	}
	//str += "=";

}

void run2()                                  //进行分数运算
{
	int n, i;
	int trueCount = 0, errCount = 0, ansCount = 1, exeCount = 1;
	ofstream fout("FracAnswers.txt");
	ofstream fout1("FracExercises.txt");
	num_i = 2;
	cout << "请输入运算式数量：";
	cin >> n;
	i = n;
	while (n--)
	{
		int fenzi, fenmu, gcd1;
		string get, ans;
		//int num1, num2, a;
		//char signal;
		struct timeb timeSeed;
		ftime(&timeSeed);
		srand(timeSeed.time * 1000 + timeSeed.millitm);
		fenzi = random(1, 5);
		fenmu = random(2, 10);
		gcd1 = gcd(fenzi, fenmu);
		fenzi = fenzi / gcd1; fenmu = fenmu / gcd1;
		strout1(fenzi, fenmu);
		cout << str << endl;
		fout1 << ansCount++ << "." << str << endl;
		str = "";
		if (fenzi%fenmu == 0) ans = fenzi / fenmu;
		else ans = to_string(fenzi) + "/" + to_string(fenmu);
		cin >> get;
		fout << exeCount++ << "." << ans << endl;
		if (get == ans) trueCount++;
		else errCount++;
		cout << "你完成了：" << i - n << "道题" << "   正确：" << trueCount << "道" << "   错误：" << errCount << "道" << endl << endl;;

		num_i = 2;
	}
}

void run()                     //进行整数运算
{
	int n, i;
	int trueCount = 0, errCount = 0, ansCount = 1, exeCount = 1;
	ofstream fout("IntAnswers.txt");
	ofstream fout1("IntExercises.txt");
	cout << "请输入运算式数量：";
	cin >> n;
	i = n;
	while (n--)
	{
		int get, ans;
		//int num1, num2, a;
		//char signal;
		struct timeb timeSeed;
		ftime(&timeSeed);
		srand(timeSeed.time * 1000 + timeSeed.millitm);
		ans = random(1, 50);
		fout << ansCount++ << "." << ans << endl;
		strout(ans);
		cout << str << endl;
		fout1 << exeCount++ << "." << str << endl;
		str = "";
		num_i = 4;
		cin >> get;
		if (get == ans) trueCount++;
		else errCount++;
		cout << "你完成了：" << i - n << "道题" << "   正确：" << trueCount << "道" << "   错误：" << errCount << "道" << endl;

	}
}
void mainUI()
{
	int choose;
	if (languageNum == 1)
	{
		cout << "                                       *****************主菜单*********************" << endl;
		cout << "                                                     1.整数运算                       " << endl;
		cout << "                                                     2.真分数运算                       " << endl;
		cout << "                                                     2.语言设置                           " << endl;
		cout << "                                                     3.退出                            " << endl << endl << endl;
		cout << "                          请输入选择：";
	}
	cin >> choose;
	if (choose == 1)
	{
		system("cls");
		run();
	}
	if (choose == 2)
	{
		system("cls");
		run2();
	}
	if (choose == 3) exit(0);
}
*/
int main()
{
	Giveequation mygiv;
	mygiv.randequation();
	string i = mygiv.getequation();
	Countequation mycount;
	mycount.count(i);
	//cout<<mycount.gcd(14, 7);
	//int i = 10;
	//string a=to_string(i);
	/*string a = "";
	string b = "34";
	a += b;
	cout << a << endl;*/
	//cout << a << endl;
	return 0;
}

