/*
用雅可比方法求出实对称矩阵的特征值和特征向量，注意必须是实对称矩阵。
时间复杂度O(n^3)，大常数。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <cmath>
using namespace std;
const int maxn = 210;
const double eps = 1e-8;
using matrix = double[maxn][maxn];
using vec = array<double, maxn>;
using pair_t = pair<double, vec>;
struct {
	matrix A, V;
	int column[maxn], n;
	void update(int r, int c, double v) {
		A[r][c] = v;
		if (column[r] == c || fabs(A[r][c]) > fabs(A[r][column[r]])) {
			for (int i = 0; i < n; ++i) if (i != r)
				if (fabs(A[r][i]) > fabs(A[r][column[r]]))
					column[r] = i;
		}
	}
	void Jacobi() {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)
				V[i][j] = 0;
			V[i][i] = 1;
			column[i] = (i == 0 ? 1 : 0);
		}
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				if (j != i && fabs(A[i][j]) > fabs(A[i][column[i]]))
					column[i] = j;
		for (int T = 0; ; ++T) { //迭代次数限制
			int x, y;
			double val = 0;
			for (int i = 0; i < n; ++i)
				if (fabs(A[i][column[i]]) > val)
					val = fabs(A[i][column[i]]), x = i, y = column[i];
			if (val < eps) //精度限制
				break;
			double phi = atan2(-2 * A[x][y], A[y][y] - A[x][x]) / 2;
			double sinp = sin(phi), cosp = cos(phi);
			for (int i = 0; i < n; ++i) if (i != x && i != y) {
				double a = A[x][i] * cosp + A[y][i] * sinp;
				double b = A[x][i] * -sinp + A[y][i] * cosp;
				update(x, i, a);
				update(y, i, b);
			}
			for (int i = 0; i < n; ++i) if (i != x && i != y) {
				double a = A[i][x] * cosp + A[i][y] * sinp;
				double b = A[i][x] * -sinp + A[i][y] * cosp;
				update(i, x, a);
				update(i, y, b);
			}
			for (int i = 0; i < n; ++i) {
				double a = V[i][x] * cosp + V[i][y] * sinp;
				double b = V[i][x] * -sinp + V[i][y] * cosp;
				V[i][x] = a, V[i][y] = b;
			}
			double a = A[x][x] * cosp * cosp + A[y][y] * sinp * sinp + 2 * A[x][y] * cosp * sinp;
			double b = A[x][x] * sinp * sinp + A[y][y] * cosp * cosp - 2 * A[x][y] * cosp * sinp;
			double tmp = (A[y][y] - A[x][x]) * sin(2 * phi) / 2 + A[x][y] * cos(2 * phi);
			update(x, y, tmp);
			update(y, x, tmp);
			A[x][x] = a, A[y][y] = b;
		}
	}
	//返回特征值和特征向量组成的pair，按照特征值从大到小排序
	auto solve(const matrix& input, int n) {
		this->n = n;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				A[i][j] = input[i][j];
		Jacobi();
		vector<pair_t> result;
		for (int i = 0; i < n; ++i)
			result.emplace_back(A[i][i], vec());
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				result[i].second[j] = V[j][i];
		sort(result.begin(), result.end(), greater<pair_t>());
		return result;
	}
}jacobi;
matrix tmp = { {15.980000, 3.400000, -10.370000, },
	{3.400000, 8.492000, -8.062000, },
	{-10.370000, -8.062000, 11.572000, }, };
int main() {
	auto result = jacobi.solve(tmp, 3);
	for (int i = 0; i < 3; ++i) {
		auto eigenvalue = result[i].first;
		auto eigenvector = result[i].second;
		printf("(%f)", eigenvalue);
		for (int i = 0; i < 3; ++i)
			printf(" %f", eigenvector[i]);
		printf("\n");
	}
	return 0;
}