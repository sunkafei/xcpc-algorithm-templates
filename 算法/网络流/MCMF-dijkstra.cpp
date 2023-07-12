#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <ctime>
using namespace std;
const int maxn = 21000;
const int inf = 1 << 30;
struct edge {
	int to, cap, cost, rev;
	edge() {}
	edge(int to, int cap, int cost, int rev) : to(to), cap(cap), cost(cost), rev(rev) {}
};
struct MCMF {
	int n, h[maxn], d[maxn], pre[maxn], num[maxn];
	vector<edge> G[maxn];
	void init(int n) {
		this->n = n;
		for (int i = 0; i <= n; ++i)
			G[i].clear();
	}
	void add_edge(int from, int to, int cap, int cost) {
		G[from].push_back(edge(to, cap, cost, G[to].size()));
		G[to].push_back(edge(from, 0, -cost, G[from].size() - 1));
	}
	//flow是自己传进去的变量，就是最后的最大流，返回的是最小费用
	int solve(int s, int t, int &flow, int limit = inf) {
		int cost = 0; 
		memset(h, 0, sizeof(h));
		while (limit) {
			priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > Q;
			for (int i = 0; i <= n; ++i)
				d[i] = inf;
			d[s] = 0; 
			Q.emplace(0, s);
			while (!Q.empty()) {
				auto now = Q.top(); Q.pop();
				int u = now.second;
				if (d[u] < now.first) continue;
				for (int i = 0; i < G[u].size(); ++i) {
					edge &e = G[u][i];
					if (e.cap > 0 && d[e.to] > d[u] + e.cost + h[u] - h[e.to]) {
						d[e.to] = d[u] + e.cost + h[u] - h[e.to];
						pre[e.to] = u;
						num[e.to] = i;
						Q.emplace(d[e.to], e.to);
					}
				}
			}
			if (d[t] == inf) break;
			for (int i = 0; i <= n; ++i)
				h[i] += d[i];
			int a = limit;
			for (int u = t; u != s; u = pre[u])
				a = min(a, G[pre[u]][num[u]].cap);
			limit -= a; flow += a; cost += a * h[t];
			for (int u = t; u != s; u = pre[u]) {
				edge &e = G[pre[u]][num[u]];
				e.cap -= a;
				G[u][e.rev].cap += a;
			}
		}
		return cost;
	}
}mcmf;
int main()
{
	freopen("D:\\in.txt", "r", stdin);
	int n, m, k, flow = 0;
	scanf("%d %d %d", &n, &m, &k);
	mcmf.init(n + 10);
	for (int i = 1; i <= m; ++i) {
		int x, y, c, w;
		scanf("%d %d %d %d", &x, &y, &c, &w);
		mcmf.add_edge(x, y, c, w);
	}
	auto start = clock();
	printf("%d\n", mcmf.solve(1, n, flow, k));
	printf("flow: %d\n", flow);
	double tot = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
	printf("MCMF-dijkstra: %f\n", tot);
	return 0;
}
