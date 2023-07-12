#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>
using namespace std;
const int maxn = 110;
const long double pi = acos(-1.0L);
const long double L = -pi, R = pi; //函数f(x)的定义域为[L, R]
const int T = 15; //用不超过T次的多项式对函数f(x)进行拟合
long double a[maxn][maxn]; //a[i]表示第i个规范正交基，a[i][j]表示第i个基的x^j的系数
long double pl[maxn], pr[maxn]; //pl[i] = pow(L, i), pr[i] = pow(R, i)
long double c[maxn]; //拟合出来的多项式系数
int pos = 0;
long double f(long double x) { //待拟合函数
	return sin(x);
}
long double calc(long double x) {
	long double result = 0, y = 1;
	for (int i = 0; i <= pos; ++i) {
		result += a[pos][i] * y;
		y *= x;
	}
	return result * f(x);
}
long double simpson(long double l, long double r) { //计算函数calc在区间[l, r]上的辛普森积分
	return (calc(l) + calc(r) + 4 * calc((l + r) / 2)) * (r - l) / 6;
}
long double solve(long double l, long double r, long double eps, long double val) { //递归求解积分
	long double mid = (l + r) / 2;
	long double L = simpson(l, mid), R = simpson(mid, r);
	if (fabs(L + R - val) <= 15 * eps)
		return L + R + (L + R - val) / 15;
	return solve(l, mid, eps / 2, L) + solve(mid, r, eps / 2, R);
}
//eps设置太小可能会导致死循环
long double asme(long double l, long double r, long double eps = 1e-16l) { //自适应辛普森积分
	return solve(l, r, eps, simpson(l, r));	//求函数calc在区间[l, r]上的自适应辛普森积分，eps指定精度
}
int main() {
	pl[0] = pr[0] = 1;
	for (int i = 1; i < maxn; ++i) {
		pl[i] = pl[i - 1] * L;
		pr[i] = pr[i - 1] * R;
	}
	a[0][0] = 1 / sqrt(R - L);
	for (int i = 1; i <= T; ++i) { //计算第i个规范正交基
		a[i][i] = 1;
		for (int j = 0; j < i; ++j) {
			long double sum = 0; //sum = <x^i, a[j]>
			for (int k = 0; k <= j; ++k)
				sum += a[j][k] * (pr[i + k + 1] - pl[i + k + 1]) / (i + k + 1);
			for (int k = 0; k <= j; ++k)
				a[i][k] -= sum * a[j][k];
		}
		long double total = 0;
		for (int j = 0; j <= i; ++j)
			for (int k = 0; k <= i; ++k)
				total += a[i][j] * a[i][k] * (pr[j + k + 1] - pl[j + k + 1]) / (j + k + 1);
		long double length = sqrt(total);
		for (int j = 0; j <= i; ++j)
			a[i][j] /= length;
	}
	for (::pos = 0; pos <= T; ++pos) {
		long double res = asme(L, R);
		for (int i = 0; i <= pos; ++i)
			c[i] += a[pos][i] * res;
	}
	for (int i = 0; i <= T; ++i)
		printf("%.17le,", c[i]);
	return 0;
}
