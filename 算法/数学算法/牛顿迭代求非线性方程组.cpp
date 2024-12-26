#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <array>
using namespace std;
const int maxn = 100;
using matrix = double[maxn][maxn];
using vect = array<double, maxn>;
const int iterations = 50;
const double eps = 1e-8;
const double delta = 1e-5; //delta指定差分的幅度
int row[maxn], var[maxn];
int one_possible(matrix A, int n, int m, vect& ans) {
	memset(row, -1, sizeof(row));
	int r = 0;
	for (int c = 0; c < m && r < n; ++c) {
		int x = r;
		for (int i = x + 1; i < n; ++i)
			if (fabs(A[i][c]) > fabs(A[x][c]))
				x = i;
		if (x != r) for (int j = 0; j <= m; ++j)
			swap(A[x][j], A[r][j]);
		if (fabs(A[r][c]) < eps)
			continue;
		for (int k = r + 1; k < n; ++k)
			for (int j = m; j >= c; --j)
				A[k][j] -= A[k][c] / A[r][c] * A[r][j];
		row[c] = r++;
	}
	for (int i = r; i < n; ++i) if (fabs(A[i][m]) > eps)
		return -1;
	for (int c = m - 1; c >= 0; --c) {
		int x = row[c];
		if (x < 0)
			ans[c] = 0;
		else {
			for (int i = x - 1; i >= 0; --i)
				A[i][m] -= A[i][c] / A[x][c] * A[x][m];
			ans[c] = A[x][m] / A[x][c];
		}
	}
	return r < m;
}
void least_square(matrix A, int n, int m, vect& ans) {
	static matrix T;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j <= m; ++j)
			T[i][j] = A[i][j];
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j <= m; ++j) {
			A[i][j] = 0;
			for (int k = 0; k < n; ++k)
				A[i][j] += T[k][i] * T[k][j];
		}
	}
	one_possible(A, m, m, ans);
}
vect solve(vect x, function<void(vect, matrix&)> calc, int n, int m) {
	static matrix A;
	static vect res;
	for (int T = 0; T < iterations; ++T) { //不同的问题应当设置不同的迭代次数
		calc(x, A);
		least_square(A, n, m, res);
		for (int i = 0; i < m; ++i)
			x[i] -= res[i];
	}
	return x;
}
vect solve(vect x, function<vect(vect)> f, int n, int m) {
	static matrix A; //n * (m + 1)
	static vect res;
	for (int T = 0; T < iterations; ++T) { //不同的问题应当设置不同的迭代次数
		auto y = f(x);
		for (int i = 0; i < m; ++i) { //差分法近似求雅可比矩阵
			auto u = x, v = x;
			u[i] += delta / 2; v[i] -= delta / 2;
			auto a = f(u), b = f(v);
			for (int j = 0; j < n; ++j)
				A[j][i] = (a[j] - b[j]) / delta;
		}
		for (int i = 0; i < n; ++i) {
			A[i][m] = y[i];
		}
		least_square(A, n, m, res);
		for (int i = 0; i < m; ++i)
			x[i] -= res[i];
	}
	return x;
}
int main() {
	// (x - 1)^2 + y^2 + z^2 - 1 = 0
	// x^2 + (y - 1)^2 + z^2 - 1 = 0
	// x^2 + y^2 + (z - 1)^2 - 1 = 0
	auto f = [](vect arg) ->vect {
		auto x = arg[0], y = arg[1], z = arg[2];
		return vect {
			(x - 1) * (x - 1) + y * y + z * z - 1,
			x * x + (y - 1) * (y - 1) + z * z - 1,
			x * x + y * y + (z - 1) * (z - 1) - 1,
		};
	};
	auto calc = [&](vect x, matrix& A) {
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
	vect x = { -122, 100, 30 };
	auto res = solve(x, f, 3, 3);
	for (int i = 0; i < 3; ++i)
		printf("%.12f ", res[i]);
	printf("\n");


	auto f2 = [](vect arg) ->vect {
		auto x = arg[0], y = arg[1];
		return vect{
			x * x + y * y - 1,
			x - y,
			y + x - 1
		};
	};
	vect x2 = { -1, -1 };
	auto res2 = solve(x2, f2, 3, 2);
	for (int i = 0; i < 2; ++i)
		printf("%.12f ", f2(res2)[i]);
	printf("\n");
	return 0;
}