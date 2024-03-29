#include<cmath>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
double f(double x) { //任意一个自定义的函数
	return x * x; 
}
double simpson(double l, double r) { //计算函数f在区间[l, r]上的辛普森积分
	return (f(l) + f(r) + 4 * f((l + r) / 2)) * (r - l) / 6;
}
double solve(double l, double r, double eps, double val) { //递归求解积分
	double mid = (l + r) / 2;
	double L = simpson(l, mid), R = simpson(mid, r);
	if (fabs(L + R - val) <= 15 * eps)
		return L + R + (L + R - val) / 15;
	return solve(l, mid, eps / 2, L) + solve(mid, r, eps / 2, R);
}
double asme(double l, double r, double eps = 1e-7) { //自适应辛普森积分
	return solve(l, r, eps, simpson(l, r));			 //求函数f在区间[l, r]上的自适应辛普森积分，eps指定精度
}
int main() {
	printf("%.15f\n", asme(0, 1)); //ans = 0.33333333333333
	return 0;
}