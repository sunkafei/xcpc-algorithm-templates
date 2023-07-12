#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <ctime>
using namespace std;
const int maxn = 20100;
const int inf = 1 << 30;
struct edge {
	int from, to, cap, flow, cost;
	edge(int u, int v, int c, int f, int w) : from(u), to(v), cap(c), flow(f), cost(w) {}
};
struct MCMF {
	int n, m;
	vector<edge> edges;
	vector<int> G[maxn];
	int inq[maxn];
	int d[maxn];
	int p[maxn];
	int a[maxn];
	void init(int n) {
		this->n = n;
		for (int i = 0; i < n; ++i)
			G[i].clear();
		edges.clear();
	}
	void add_edge(int from, int to, int cap, int cost) {
		edges.push_back(edge(from, to, cap, 0, cost));
		edges.push_back(edge(to, from, 0, 0, -cost));
		m = edges.size();
		G[from].push_back(m - 2);
		G[to].push_back(m - 1);
	}
	bool BellmanFord(int s, int t, int &flow, int &cost, int limit) {
		for (int i = 0; i < n; ++i)
			d[i] = inf;
		memset(inq, 0, sizeof(inq));
		d[s] = 0; inq[s] = 1; p[s] = 0; a[s] = inf;
		queue<int> Q;
		Q.push(s);
		while (!Q.empty()) {
			int u = Q.front(); Q.pop();
			inq[u] = false;
			for (unsigned i = 0; i < G[u].size(); ++i) {
				edge &e = edges[G[u][i]];
				if (e.cap > e.flow && d[e.to] > d[u] + e.cost) {
					d[e.to] = d[u] + e.cost;
					p[e.to] = G[u][i];
					a[e.to] = min(a[u], e.cap - e.flow);
					if (!inq[e.to]) {
						Q.push(e.to);
						inq[e.to] = true;
					}
				}
			}
		}
		if (d[t] == inf)
			return false;
		a[t] = min(a[t], limit - flow);
		flow += a[t];
		cost += d[t] * a[t];
		for (int u = t; u != s; u = edges[p[u]].from) {
			edges[p[u]].flow += a[t];
			edges[p[u] ^ 1].flow -= a[t];
		}
		return true;
	}
	int solve(int s, int t, int limit = inf) {
		int flow = 0, cost = 0;
		while (flow < limit && BellmanFord(s, t, flow, cost, limit));
		return cost;
	}
}mcmf;
int main()
{
	freopen("D:\\in.txt", "r", stdin);
	int n, m, k;
	scanf("%d %d %d", &n, &m, &k);
	mcmf.init(n + 10);
	for (int i = 1; i <= m; ++i) {
		int x, y, c, w;
		scanf("%d %d %d %d", &x, &y, &c, &w);
		mcmf.add_edge(x, y, c, w);
	}
	auto start = clock();
	printf("%d\n", mcmf.solve(1, n, k));
	double tot = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
	printf("MCMF-spfa: %f\n", tot);
	return 0;
}




















