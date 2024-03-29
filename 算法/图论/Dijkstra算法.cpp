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
namespace simple_dijkstra { // O(n^2)
	const int maxn = 1001;
	int vis[maxn], d[maxn], w[maxn][maxn];
	void dijkstra() {
		memset(vis, 0, sizeof(vis));
		memset(d, 0x3f, sizeof(d));
		d[s] = 0;
		for (int i = 1; i <= n; ++i) {
			int x = -1;
			for (int y = 1; y <= n; ++y) if (!vis[y])
				if (x == -1 || d[y] < d[x])
					x = y;
			vis[x] = true;
			for (int y = 1; y <= n; ++y)
				d[y] = min(d[y], d[x] + w[x][y]);
		}
	}
	void solve() {
		scanf("%d %d %d", &n, &m, &s);
		memset(w, 0x3f, sizeof(w));
		for (int i = 0; i < m; ++i) {
			int x, y, z;
			scanf("%d %d %d", &x, &y, &z);
			w[x][y] = min(w[x][y], z);
		}
		dijkstra();
		for (int i = 1; i <= n; ++i)
			printf("%d ", d[i] == 0x3f3f3f3f ? INT_MAX : d[i]);
		printf("\n");
	}
}
namespace fast_dijkstra {
	const int maxn = 210000;
	vector<pair<int, int>> G[maxn];
	int d[maxn];
	void dijkstra() {
		using node = pair<int, int>;
		priority_queue<node, vector<node>, greater<node>> Q;
		memset(d, 0x3f, sizeof(d));
		d[s] = 0;
		Q.emplace(0, s);
		while (!Q.empty()) {
			auto [dist, x] = Q.top(); Q.pop();
			if (dist != d[x])
				continue;
			for (auto [y, w] : G[x]) {
				if (d[y] > d[x] + w) {
					d[y] = d[x] + w;
					Q.emplace(d[y], y);
					//p[y] = x;
				}
			}
		}
	}
	void solve() {
		scanf("%d %d %d", &n, &m, &s);
		for (int i = 0; i < m; ++i) {
			int x, y, z;
			scanf("%d %d %d", &x, &y, &z);
			G[x].emplace_back(y, z);
		}
		dijkstra();
		for (int i = 1; i <= n; ++i)
			printf("%d ", d[i] == 0x3f3f3f3f ? INT_MAX : d[i]);
		printf("\n");
	}
}
int main() {
	//freopen("in.txt", "r", stdin);
	fast_dijkstra::solve();
	return 0;
}