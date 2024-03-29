#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
using namespace std;
const int maxn = 100;
using matrix = double[maxn][maxn];
using vect = array<double, maxn>;
//当方程有唯一解的时候，算出唯一解
//矩阵A的大小为n * (n + 1)
void gauss_elimination(matrix A, int n) {
	for (int i = 0; i < n; ++i) {
		int r = i;
		for (int j = i + 1; j < n; ++j)
			if (fabs(A[j][i]) > fabs(A[r][i]))
				r = j;
		if (r != i) for (int j = 0; j <= n; ++j)
			swap(A[r][j], A[i][j]);
		for (int k = i + 1; k < n; ++k)
			for (int j = n; j >= i; --j)
				A[k][j] -= A[k][i] / A[i][i] * A[i][j];
	}
	for (int i = n - 1; i >= 0; --i) {
		for (int j = i + 1; j < n; ++j)
			A[i][n] -= A[j][n] * A[i][j];
		A[i][n] /= A[i][i];
	}
}
//无解返回-1，有唯一解返回0，有无穷多解返回1。
//在有解的情况下通过ans返回任意一个解。
//矩阵A的大小为n * (m + 1)，表示有n个方程，m个变量。
const double eps = 1e-8;
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
//计算方程的最小二乘解
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
//将矩阵A化为简化阶梯形
//ans为方程组的一个特解，basis为齐次方程组解空间的一组基。
//该算法针对稀疏矩阵进行了优化，在矩阵中有大量0元素时，时间复杂度会小于O(n^3)。
int row_simplify(matrix A, int n, int m, vect& ans, vector<vect>& basis) {
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
		for (int j = m; j >= c; --j)
			A[r][j] /= A[r][c];
		for (int k = r + 1; k < n; ++k) if (fabs(A[k][c]) > eps)
			for (int j = m; j >= c; --j)
				A[k][j] -= A[k][c] * A[r][j];
		var[r] = c;
		row[c] = r++;
	}
	for (int i = r; i < n; ++i) if (fabs(A[i][m]) > eps)
		return -1;
	for (int c = m - 1; c >= 0; --c) {
		int x = row[c];
		if (x < 0)
			ans[c] = 0;
		else {
			for (int i = x - 1; i >= 0; --i) if (fabs(A[i][c]) > eps)
				for (int j = m; j >= c; --j)
					A[i][j] -= A[i][c] * A[x][j];
			ans[c] = A[x][m];
		}
	}
	//求出基础解系
	for (int c = m - 1; c >= 0; --c) if (row[c] < 0) {
		vect now = {};
		for (int i = 0; i < r; ++i)
			now[var[i]] = -A[i][c];
		now[c] = 1;
		basis.push_back(now);
	}
	return r < m;
}
namespace rectangle_mod {
	const int mod = 998244353;
	using matrix = int[maxn][maxn];
	inline int pow_mod(int a, int n) {
		int ans = 1;
		while (n) {
			if (n & 1)
				ans = 1LL * ans * a % mod;
			n >>= 1;
			a = 1LL * a * a % mod;
		}
		return ans;
	}
	inline int inv(int n) {
		return pow_mod(n, mod - 2);
	}
	//对n行m列的矩阵进行取模意义下的高斯消元，必须保证矩阵行满秩
	inline void gauss(matrix A, int n, int m) {
		for (int i = 0; i < n; ++i) {
			int r = i;
			for (int j = i; j < n; ++j) {
				if (A[j][i] != 0) {
					r = j;
					break;
				}
			}
			if (r != i) for (int j = 0; j < m; ++j)
				swap(A[r][j], A[i][j]);
			int pivot = inv(A[i][i]);
			for (int k = i + 1; k < n; ++k) {
				int val = 1LL * A[k][i] * pivot % mod;
				for (int j = m - 1; j >= i; --j)
					A[k][j] = (A[k][j] - 1LL * A[i][j] * val % mod + mod) % mod;
			}
		}
		for (int i = n - 1; i >= 0; --i) {
			for (int j = i + 1; j < n; ++j) {
				for (int k = n; k < m; ++k)
					A[i][k] = (A[i][k] - 1LL * A[j][k] * A[i][j] % mod + mod) % mod;
			}
			int pivot = inv(A[i][i]);
			for (int k = n; k < m; ++k)
				A[i][k] = 1LL * A[i][k] * pivot % mod;
		}
	}
}
int main() { //3 -5 -2
	matrix A = {
		{1, 1, 0, 0, -3},
		{1, 1, 0, 0, -1},
		{1, 0, 1, 0, 0},
		{1, 0, 1, 0, 2},
		{1, 0, 0, 1, 5},
		{1, 0, 0, 1, 1}
	};
	vect ans;
	vector<vect> basis;
	least_square(A, 6, 4, ans);
	for (int i = 0; i < 4; ++i)
		printf("%f\n", ans[i]);
	printf("\n");
	return 0;
}