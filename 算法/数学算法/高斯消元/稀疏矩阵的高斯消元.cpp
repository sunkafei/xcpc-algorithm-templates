#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctime>
#include <random>
#include <unordered_map>
#include <unordered_set>
using namespace std;
const int maxn = 110000;
const long long mod = 1e9 + 7;
const double eps = 1e-9;
long long pow_mod(long long a, long long n, long long p) {
	long long ans = 1;
	while (n) {
		if (n & 1)
			ans = ans * a % p;
		a = a * a % p;
		n >>= 1;
	}
	return ans;
}
long long inv(long long a) {
	return pow_mod(a, mod - 2, mod);
}
struct fast_gauss_mod { //基于十字链表的高斯消元
	static const int maxstate = 3000000;
	static const int table_size = 5110007;
	struct {
		long long val;
		int r, c, next, right, down;
	} node[maxstate];
	int first[table_size], row[maxn], column[maxn], vis[maxn], length[maxn], sz, n;
	void init(int n) { //待消元的矩阵大小是n * (n + 1)的
		this->n = n;
		sz = 0;
		memset(first, 0, sizeof(first));
		memset(row, 0, sizeof(row));
		memset(column, 0, sizeof(column));
		memset(vis, 0, sizeof(vis));
		memset(length, 0, sizeof(length));
	}
	long long& A(int r, int c) {
		const int h = ((long long)r << 20 | c) % table_size;
		for (int i = first[h]; i; i = node[i].next)
			if (node[i].r == r && node[i].c == c)
				return node[i].val;
		int i = ++sz;
		node[i].next = first[h];
		first[h] = i;
		node[i].right = row[r];
		row[r] = i;
		node[i].down = column[c];
		column[c] = i;
		node[i].r = r;
		node[i].c = c;
		node[i].val = 0;
		length[r] += 1;
		return node[i].val;
	}
	void insert(int r, int c, long long v) { //在矩阵的第r行第c列填上v
		A(r, c) = v;
	}
	vector<long long> solve() { //无解或用无穷解时返回空vector
		vector<long long> ans(n);
		for (int i = 0; i < n; ++i) {
			int r = -1;
			for (int x = column[i]; x; x = node[x].down) {
				int j = node[x].r;
				if (!vis[j] && node[x].val != 0)
					if (r < 0 || length[j] < length[r])
						r = j;
			}
			if (r == -1)
				return {};
			int* last = &row[r];
			for (int y = row[r]; y; y = node[y].right) {
				if (node[y].val == 0)
					*last = node[y].right;
				else
					last = &node[y].right;
			}
			auto pivot = mod - inv(A(r, i));
			for (int x = column[i]; x; x = node[x].down) {
				int j = node[x].r;
				if (!vis[j] && j != r) {
					auto ratio = node[x].val * pivot % mod;
					for (int y = row[r]; y; y = node[y].right) {
						int k = node[y].c;
						A(j, k) = (A(j, k) + ratio * node[y].val) % mod;
					}
					length[j] -= 1;
				}
			}
			ans[i] = r;
			vis[r] = true;
		}
		for (int i = n - 1; i >= 0; --i) {
			int r = ans[i];
			auto pivot = (mod - A(r, n)) * inv(A(r, i)) % mod;
			for (int x = column[i]; x; x = node[x].down) {
				int j = node[x].r;
				if (j != r)
					A(j, n) = (A(j, n) + node[x].val * pivot) % mod;
			}
		}
		for (int i = 0; i < n; ++i)
			ans[i] = A(ans[i], n) * inv(A(ans[i], i)) % mod;
		return ans;
	}
}gauss;
struct fast_gauss_double {
	static const int maxstate = 2000000;
	static const int table_size = 5110007;
	struct {
		double val;
		int r, c, next, right, down;
	} node[maxstate];
	int first[table_size], row[maxn], column[maxn], vis[maxn], length[maxn], sz, n;
	void init(int n) { //待消元的矩阵大小是n * (n + 1)的
		this->n = n;
		sz = 0;
		memset(first, 0, sizeof(first));
		memset(row, 0, sizeof(row));
		memset(column, 0, sizeof(column));
		memset(vis, 0, sizeof(vis));
		memset(length, 0, sizeof(length));
	}
	double& A(int r, int c) {
		const int h = ((long long)r << 20 | c) % table_size;
		for (int i = first[h]; i; i = node[i].next)
			if (node[i].r == r && node[i].c == c)
				return node[i].val;
		int i = ++sz;
		node[i].next = first[h];
		first[h] = i;
		node[i].right = row[r];
		row[r] = i;
		node[i].down = column[c];
		column[c] = i;
		node[i].r = r;
		node[i].c = c;
		node[i].val = 0;
		length[r] += 1;
		return node[i].val;
	}
	void insert(int r, int c, double v) { //在矩阵的第r行第c列填上v
		A(r, c) = v;
	}
	vector<double> solve() { //无解或用无穷解时返回空vector
		vector<double> ans(n);
		vector<int> res(n);
		for (int i = 0; i < n; ++i) {
			int r = -1;
			for (int x = column[i]; x; x = node[x].down) {
				int j = node[x].r;
				if (!vis[j] && fabs(node[x].val) > eps)
					if (r < 0 || length[j] < length[r])
						r = j;
			}
			if (r == -1)
				return {};
			int* last = &row[r];
			for (int y = row[r]; y; y = node[y].right) {
				if (fabs(node[y].val) < eps)
					*last = node[y].right;
				else
					last = &node[y].right;
			}
			auto pivot = -A(r, i);
			for (int x = column[i]; x; x = node[x].down) {
				int j = node[x].r;
				if (!vis[j] && j != r) {
					auto ratio = node[x].val / pivot;
					for (int y = row[r]; y; y = node[y].right) {
						int k = node[y].c;
						A(j, k) += ratio * node[y].val;
					}
					length[j] -= 1;
				}
			}
			res[i] = r;
			vis[r] = true;
		}
		for (int i = n - 1; i >= 0; --i) {
			int r = res[i];
			auto pivot = -A(r, n) / A(r, i);
			for (int x = column[i]; x; x = node[x].down) {
				int j = node[x].r;
				if (j != r)
					A(j, n) += node[x].val * pivot;
			}
		}
		for (int i = 0; i < n; ++i)
			ans[i] = A(res[i], n) / A(res[i], i);
		return ans;
	}
}gas;
namespace gauss_mod { //稀疏矩阵时，复杂度接近O(n^2)
	const int maxn = 1000;
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
	inline void gauss(matrix A, int n) {
		int column[maxn];
		for (int i = 0; i < n; ++i) {
			int r = i;
			for (int j = i; j < n; ++j) {
				if (A[j][i] != 0) {
					r = j;
					break;
				}
			}
			if (r != i) for (int j = 0; j <= n; ++j)
				swap(A[r][j], A[i][j]);
			int pivot = inv(A[i][i]), sz = 0;
			for (int j = n; j >= i; --j) if (A[i][j])
				column[sz++] = j;
			for (int k = i + 1; k < n; ++k) if (A[k][i]) {
				int val = 1LL * A[k][i] * pivot % mod;
				for (int t = 0; t < sz; ++t) {
					int j = column[t];
					A[k][j] = (A[k][j] - 1LL * A[i][j] * val % mod + mod) % mod;
				}
			}
		}
		for (int i = n - 1; i >= 0; --i) {
			for (int j = i + 1; j < n; ++j) if (A[i][j]) {
				A[i][n] = (A[i][n] - 1LL * A[j][n] * A[i][j] % mod + mod) % mod;
			}
			A[i][n] = 1LL * A[i][n] * inv(A[i][i]) % mod;
		}
	}
}
int main() {
	int n;
	scanf("%d", &n);
	gas.init(n);
	for (int i = 0; i < n; ++i) {
		for (int v, j = 0; j <= n; ++j) {
			scanf("%d", &v);
			if (v) gas.insert(i, j, v);
		}
	}
	auto ans = gas.solve();
	if (ans.empty()) {
		puts("No Solution");
		return 0;
	}
	for (int i = 0; i < n; ++i)
		printf("%.2f\n", ans[i]);
	return 0;
}