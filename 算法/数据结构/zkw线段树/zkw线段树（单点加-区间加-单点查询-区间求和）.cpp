#include <cstdio>
#include <cstring>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <random>
using namespace std;
const int maxn = 110000;
long long A[maxn];
struct {
	int n, m;
	long long sum[maxn * 4], add[maxn * 4];
	void build(int size) {	
		this->n = size;
		for (m = 1; m <= n; m <<= 1);
		memset(add, 0, sizeof(long long) * (n + m + 1));
		for (int i = 1; i <= n; ++i)
			sum[m + i] = A[i];
		for (int i = m - 1; i; --i)
			sum[i] = sum[i << 1] + sum[i << 1 | 1];
	}
	void update(int pos, int v) { //单点加v
		for (int x = pos + m; x > 1; x >>= 1)
			sum[x] += v;
	}
	void update(int s, int t, int v) { //区间加v
		long long lc = 0, rc = 0, len = 1;
		for (s += m - 1, t += m + 1; s ^ t ^ 1; s >>= 1, t >>= 1, len <<= 1) {
			if (~s & 1)
				add[s ^ 1] += v, lc += len;
			if (t & 1)   
				add[t ^ 1] += v, rc += len;
			sum[s >> 1] += v * lc, sum[t >> 1] += v * rc;
		}
		for (lc += rc; s; s >>= 1)
			sum[s >> 1] += v * lc;
	}
	long long query(int pos) { //单点查询
		long long ans = sum[pos + m];
		for (int x = pos + m; x; x >>= 1) 
			ans += add[x];
		return ans;
	}
	long long query(int s, int t) { //计算区间和
		long long ans = 0, lc = 0, rc = 0, len = 1;
		for (s += m - 1, t += m + 1; s ^ t ^ 1; s >>= 1, t >>= 1, len <<= 1) {
			if (s & 1 ^ 1)
				ans += sum[s ^ 1] + len * add[s ^ 1], lc += len;
			if (t & 1)
				ans += sum[t ^ 1] + len * add[t ^ 1], rc += len;
			ans += add[s >> 1] * lc;
			ans += add[t >> 1] * rc;
		}
		for (lc += rc, s >>= 1; s; s >>= 1) 
			ans += add[s] * lc;
		return ans;
	}
}tree;
int main() {
	default_random_engine e;
	uniform_int_distribution<int> d(1, 100000);
	e.seed(35345);
	int n = 100000;
	for (int i = 1; i <= n; ++i)
		A[i] = d(e);
	auto start = clock();
	tree.build(n);
	for (int i = 0; i < 1000000; ++i) {
		int tp = d(e) % 4, a = d(e) % n + 1, b = d(e) % n + 1;
		int L = min(a, b), R = max(a, b);
		if (tp == 0) {
			tree.update(L, R, 1);
			for (int i = L; i <= R; ++i)
				A[i] += 1;
		}
		else if (tp == 1) {
			long long ans = tree.query(L, R), res = accumulate(A + L, A + R + 1, 0LL);
			if (ans != res)
				abort();
		}
		else if (tp == 2) {
			tree.update(L, 7);
			A[L] += 7;
		}
		else {
			long long ans = tree.query(L);
			if (ans != A[L])
				abort();
		}
	}
	auto end = clock();
	printf("time: %.2f\n", static_cast<double>(end - start) / CLOCKS_PER_SEC);
	for (int i = 1; i <= 100000; ++i)
		if (A[i] != tree.query(i))
			abort();
	return 0;
}
