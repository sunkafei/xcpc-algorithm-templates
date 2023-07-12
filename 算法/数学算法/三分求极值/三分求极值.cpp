#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
#define f(x) fabs(x - 8)
using namespace std;
const double eps = 1e-6;
int maximum_int(int L, int R) { //三分求f函数的最大值（定义域为整数）
	while (R > L) {
		int m1 = (2 * L + R) / 3;
		int m2 = (2 * R + L + 2) / 3;
		if (f(m1) > f(m2))
			R = m2 - 1;
		else
			L = m1 + 1;
	}
	return L; //f(L)为最大值
}
int minimun_int(int L, int R) { //三分求f函数的最小值（定义域为整数）
	while (R > L) {
		int m1 = (2 * L + R) / 3;
		int m2 = (2 * R + L + 2) / 3;
		if (f(m1) < f(m2))
			R = m2 - 1;
		else
			L = m1 + 1;
	}
	return L; //f(L)为最小值
}
double maximum_double(double L, double R) { //三分求f函数的最大值（定义域为实数）
	while (R - L > eps) { // for i in range(100):
		double m1 = (2 * L + R) / 3;
		double m2 = (2 * R + L) / 3;
		if (f(m1) > f(m2))
			R = m2;
		else
			L = m1;
	}
	return L; //f(L)为最大值
}
double minimun_double(double L, double R) { //三分求f函数的最小值（定义域为实数）
	while (R - L > eps) { // for i in range(100):
		double m1 = (2 * L + R) / 3;
		double m2 = (2 * R + L) / 3;
		if (f(m1) < f(m2))
			R = m2;
		else
			L = m1;
	}
	return L; //f(L)为最小值
}
int main() {
	double pos = minimun_double(-1, 100);
	printf("%f\n", pos);
	return 0;
}