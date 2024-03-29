#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <array>
#include "gauss.h"
using namespace std;
//const int maxn = 111;
//using matrix = double[maxn][maxn];
using vec = array<double, 3>; //用vector的话会很慢
//x为初始解向量
//calc函数返回一个n * (n + 1)的矩阵，左边是雅可比矩阵，右边是函数值向量
vec solve(vec x, function<void(vec, matrix&)> calc) {
	static matrix A;
	static vect res;
	int n = x.size();
	for (int T = 0; T < 50; ++T) { //不同的问题应当设置不同的迭代次数
		calc(x, A);
		least_square(A, n, n, res);
		for (int i = 0; i < n; ++i)
			x[i] -= res[i];
	}
	return x;
}
//求向量值函数f的零点
const double h = 1e-5; //h指定差分的幅度
vec solve(vec x, function<vec(vec)> f) {
	static matrix A;
	static vect res;
	int n = x.size();
	for (int T = 0; T < 50; ++T) { //不同的问题应当设置不同的迭代次数
		vec y = f(x);
		for (int i = 0; i < n; ++i) { //差分法近似求雅可比矩阵
			vec u = x, v = x;
			u[i] += h / 2; v[i] -= h / 2;
			vec a = f(u), b = f(v);
			for (int j = 0; j < n; ++j)
				A[j][i] = (a[j] - b[j]) / h;
			A[i][n] = y[i];
		}
		least_square(A, n, n, res);
		for (int i = 0; i < n; ++i)
			x[i] -= res[i];
	}
	return x;
}
int main() {
	// (x - 1)^2 + y^2 + z^2 - 1 = 0
	// x^2 + (y - 1)^2 + z^2 - 1 = 0
	// x^2 + y^2 + (z - 1)^2 - 1 = 0
	auto f = [](vec arg) ->vec {
		auto x = arg[0], y = arg[1], z = arg[2];
		return vec{
			(x - 1) * (x - 1) + y * y + z * z - 1,
			x * x + (y - 1) * (y - 1) + z * z - 1,
			x * x + y * y + (z - 1) * (z - 1) - 1,
		};
	};
	auto calc = [&](vec x, matrix& A) {
		//Jacobi
		for (int i = 0; i < x.size(); ++i) {
			for (int j = 0; j < x.size(); ++j) {
				if (i == j)
					A[i][j] = 2 * (x[i] - 1);
				else
					A[i][j] = 2 * x[j];
			}
		}
		//f(x)
		auto res = f(x);
		for (int i = 0; i < x.size(); ++i)
			A[i][x.size()] = res[i];
	};
	vec x = { -122, 100, 30 };
	auto res = solve(x, f);
	for (auto i : res)
		printf("%.12f ", i);
	printf("\n");
	return 0;
}