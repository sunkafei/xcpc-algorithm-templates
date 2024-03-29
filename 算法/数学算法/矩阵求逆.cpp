#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;
const int maxn = 555;
namespace inverse_double {
	using matrix = double[maxn][maxn];
	double temp[maxn][maxn * 2];
	void inverse(const matrix& A, matrix& res, int n) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)
				temp[i][j] = A[i][j], temp[i][n + j] = 0;
			temp[i][n + i] = 1;
		}
		for (int i = 0; i < n; ++i) {
			int r = i;
			for (int j = i + 1; j < n; ++j)
				if (fabs(temp[j][i]) > fabs(temp[r][i]))
					r = j;
			if (r != i)
				for (int j = 0; j < 2 * n; ++j)
					swap(temp[i][j], temp[r][j]);
			assert(fabs(temp[i][i]) > 1e-10);
			for (int k = i + 1; k < n; ++k)
				for (int j = 2 * n - 1; j >= i; --j)
					temp[k][j] -= temp[k][i] / temp[i][i] * temp[i][j];
		}
		for (int i = n - 1; i >= 0; --i) {
			for (int j = i + 1; j < n; ++j)
				for (int k = n; k < 2 * n; ++k)
					temp[i][k] -= temp[j][k] * temp[i][j];
			for (int k = n; k < 2 * n; ++k)
				temp[i][k] /= temp[i][i];
		}
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				res[i][j] = temp[i][n + j];
	}
}
namespace inverse_mod {
	const long long mod = 998244353;
	using matrix = long long[maxn][maxn];
	matrix temp;
	long long pow_mod(long long a, long long n) {
		long long ans = 1;
		while (n) {
			if (n & 1)
				ans = ans * a % mod;
			n >>= 1;
			a = a * a % mod;
		}
		return ans;
	}
	long long inv(long long x) {
		return pow_mod(x, mod - 2);
	}
	void inverse(const matrix& A, matrix& res, int n) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)
				temp[i][j] = A[i][j], temp[i][n + j] = 0;
			temp[i][n + i] = 1;
		}
		for (int i = 0; i < n; ++i) {
			int r = i;
			for (int j = i; j < n; ++j) {
				if (temp[j][i] != 0) {
					r = j;
					break;
				}
			}
			if (r != i) for (int j = 0; j < n * 2; ++j)
				swap(temp[i][j], temp[r][j]);
			auto pivot = inv(temp[i][i]);
			for (int k = i + 1; k < n; ++k)
				for (int j = 2 * n - 1; j >= i; --j)
					temp[k][j] = (temp[k][j] - temp[k][i] * pivot % mod * temp[i][j] % mod + mod) % mod;
		}
		for (int i = n - 1; i >= 0; --i) {
			for (int j = i + 1; j < n; ++j) {
				for (int k = n; k < n * 2; ++k) {
					temp[i][k] = (temp[i][k] - temp[j][k] * temp[i][j] % mod + mod) % mod;
				}
			}
			auto pivot = inv(temp[i][i]);
			for (int k = n; k < n * 2; ++k)
				temp[i][k] = temp[i][k] * pivot % mod;
		}
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				res[i][j] = temp[i][n + j];
	}
}
using namespace inverse_double;
int main() {
	matrix res, A = { {0, 1, 2}, {1, 0, 3}, {4, -3, 8} };
	inverse(A, res, 3);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j)
			printf("%.4f ", res[i][j]);
		printf("\n");
	}
	return 0;
}