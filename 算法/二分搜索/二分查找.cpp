#include <cstdio>
#include <cstdlib>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <functional>
#define MAXN 1000000
using namespace std;
int BSearch_Upper(function<bool(int)> ok) //求满足ok函数的值的上界（满足ok函数的最大值）
{
	int L = 0, R = MAXN;
	while (L < R)
	{
		int M = L + (R - L + 1) / 2;
		if (ok(M)) L = M;
		else R = M - 1;
	}
	return L;
}
int BSearch_Lower(function<bool(int)> ok) //求满足ok函数的值的下界（满足ok函数的最小值）
{
	int L = 0, R = MAXN;
	while (L < R)
	{
		int M = L + (R - L) / 2;
		if (ok(M)) R = M;
		else L = M + 1;
	}
	return L;
}
double BSearch_Upper_Double(function<bool(double)> ok) //精度型二分法（满足ok函数的最大值）
{
	double L = 0, R = MAXN;
	while (R - L > 1e-5)
	{
		double M = (L + R) / 2;
		if (ok(M)) L = M;
		else R = M;
	}
	return L;
}
double BSearch_Lower_Double(function<bool(double)> ok) //精度型二分法（满足ok函数的最小值）
{
	double L = 0, R = MAXN;
	while (R - L > 1e-5)
	{
		double M = (L + R) / 2;
		if (ok(M)) R = M;
		else L = M;
	}
	return L;
}
int main()
{
	std::cout << BSearch_Upper([](int v) {return v <= 5; }) << std::endl;
	std::cout << BSearch_Lower([](int v) {return v >= 5; }) << std::endl;
	std::cout << BSearch_Upper_Double([](double v) {return v <= 5; }) << std::endl;
	std::cout << BSearch_Lower_Double([](double v) {return v >= 5; }) << std::endl;
	return 0;
}