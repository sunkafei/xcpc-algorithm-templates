#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
using namespace std;
int n, m, s; //点数、边数、起点
namespace BellmanFord { // O(nm)
	const int maxn = 1001;
	const int maxedges = 110000;
	int d[maxn], x[maxedges], y[maxedges], w[maxedges];
	void BellmanFord() {
		memset(d, 0x3f, sizeof(d));
		d[s] = 0;
		for (int k = 1; k < n; ++k)
			for (int i = 1; i <= m; ++i)
				d[y[i]] = min(d[y[i]], d[x[i]] + w[i]);
	}
	void solve() {
		scanf("%d %d %d", &n, &m, &s);
		for (int i = 1; i <= m; ++i)
			scanf("%d %d %d", &x[i], &y[i], &w[i]);
		BellmanFord();
		for (int i = 1; i <= n; ++i)
			printf("%d ", d[i] == 0x3f3f3f3f ? INT_MAX : d[i]);
		printf("\n");
	}
}
namespace SPFA {
	const int maxn = 210000;
	int inq[maxn], cnt[maxn], d[maxn];
	vector<pair<int, int>> G[maxn];
	bool spfa() {
		queue<int> Q;
		memset(inq, 0, sizeof(inq));
		memset(cnt, 0, sizeof(cnt));
		memset(d, 0x3f, sizeof(d));
		d[s] = 0;
		inq[s] = true;
		Q.push(s);
		while (!Q.empty()) {
			int x = Q.front(); Q.pop();
			inq[x] = false;
			for (auto [y, w] : G[x]) {
				if (d[y] > d[x] + w) {
					d[y] = d[x] + w;
					//p[y] = x;
					if (!inq[y]) {
						Q.push(y);
						inq[y] = true;
						if (++cnt[y] > n)
							return false;
					}
				}
			}
		}
		return true;
	}
	void solve() {
		scanf("%d %d %d", &n, &m, &s);
		for (int i = 1; i <= m; ++i) {
			int x, y, w;
			scanf("%d %d %d", &x, &y, &w);
			G[x].emplace_back(y, w);
		}
		spfa();
		for (int i = 1; i <= n; ++i)
			printf("%d ", d[i] == 0x3f3f3f3f ? INT_MAX : d[i]);
		printf("\n");
	}
}
int main() {
	//freopen("in.txt", "r", stdin);
	SPFA::solve();
	return 0;
}