#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
const long long inf = 1LL << 60;
const int maxn = 505;
//若要求不完美匹配，需要把所有的-inf替换成0。
struct KM {
	int n, py[maxn], vy[maxn], pre[maxn];
	long long G[maxn][maxn], slk[maxn], kx[maxn], ky[maxn];
	void init(int n) { //左右两侧各有n个结点，结点编号从1开始
		this->n = n;
		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= n; ++j)
				G[i][j] = -inf;
		for (int i = 1; i <= n; ++i)
			ky[i] = py[i] = 0;
	}
	void add_edge(int x, int y, long long w) {
		G[y][x] = max(G[y][x], w);
	}
	long long solve() {
		int k, p;
		for (int i = 1; i <= n; ++i)
			kx[i] = *max_element(G[i] + 1, G[i] + n + 1);
		for (int i = 1; i <= n; i++) {
			for (int j = 0; j <= n; ++j)
				vy[j] = pre[j] = 0, slk[j] = inf;
			for (py[k = 0] = i; py[k]; k = p) {
				long long d = inf;
				int x = py[k];
				vy[k] = 1;
				for (int j = 1; j <= n; j++) if (!vy[j]) {
					long long t = kx[x] + ky[j] - G[x][j];
					if (t < slk[j])
						slk[j] = t, pre[j] = k;
					if (slk[j] < d)
						d = slk[j], p = j;
				}
				for (int j = 0; j <= n; j++) {
					if (vy[j])
						kx[py[j]] -= d, ky[j] += d;
					else
						slk[j] -= d;
				}
			}
			for (; k; k = pre[k])
				py[k] = py[pre[k]];
		}
		long long ans = 0;
		for (int i = 1; i <= n; i++) if (G[py[i]][i] > -inf)
			ans += kx[i] + ky[i];
		return ans;
	}
	vector<int> result() { //返回左侧每个结点对应的右侧匹配点，没有则为0
		vector<int> res(1);//调用该函数之前，应当先调用solve函数
		for (int i = 1; i <= n; ++i)
			res.push_back(G[py[i]][i] > -inf ? py[i] : 0);
		return res;
	}
}km;
struct KM_old {
	static const int inf = 1 << 30;
	int n;
	vector<int> G[maxn];
	int W[maxn][maxn];
	int lx[maxn], ly[maxn];
	int left[maxn];
	bool S[maxn], T[maxn];
	void init(int n) {
		this->n = n;
		for (int i = 0; i < n; ++i)
			G[i].clear();
		memset(W, 0, sizeof(W));
	}
	void add_edge(int u, int v, int w) {
		G[u].push_back(v);
		W[u][v] = w;
	}
	bool match(int u) {
		S[u] = true;
		for (unsigned i = 0; i < G[u].size(); ++i) {
			int v = G[u][i];
			if (lx[u] + ly[v] == W[u][v] && !T[v]) {
				T[v] = true;
				if (left[v] == -1 || match(left[v])) {
					left[v] = u;
					return true;
				}
			}
		}
		return false;
	}
	void update() {
		int a = inf;
		for (int u = 0; u < n; ++u) if (S[u]) {
			for (unsigned i = 0; i < G[u].size(); ++i) {
				int v = G[u][i];
				if (!T[v]) a = min(a, lx[u] + ly[v] - W[u][v]);
			}
		}
		for (int i = 0; i < n; ++i) {
			if (S[i]) lx[i] -= a;
			if (T[i]) ly[i] += a;
		}
	}
	int solve() {
		for (int i = 0; i < n; ++i) {
			lx[i] = *max_element(W[i], W[i] + n);
			left[i] = -1;
			ly[i] = 0;
		}
		for (int u = 0; u < n; ++u) {
			for (;;) {
				for (int i = 0; i < n; ++i) S[i] = T[i] = false;
				if (match(u)) break; else update();
			}
		}
		int ans = 0;
		for (int i = 0; i < n; ++i) if (left[i] != -1)
			ans += W[left[i]][i];
		return ans;
	}
};
int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	km.init(n);
	for (int i = 1; i <= m; ++i) {
		int x, y, w;
		scanf("%d %d %d", &x, &y, &w);
		km.add_edge(y, x, w);
	}
	printf("%lld\n", km.solve());
	auto res = km.result();
	for (int i = 1; i <= n; ++i)
		printf("%d ", res[i]);
	printf("\n");
	return 0;
}