#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;
const int maxn = 100000 + 10;
const int inf = 1 << 30;
struct edge{
	int from, to, cap, flow;
	edge(int u, int v, int c, int f) : from(u), to(v), cap(c), flow(f) {}
};
struct Dinic {
	int n, m, s, t;
	vector<edge> edges;    // 边数的两倍
	vector<int> G[maxn];   // 邻接表，G[i][j]表示结点i的第j条边在e数组中的序号
	bool vis[maxn];        // BFS使用
	int d[maxn];           // 从起点到i的距离
	int cur[maxn];         // 当前弧指针
	void init(int n) {
		this->n = n;
		for (int i = 0; i < n; i++)
			G[i].clear();
		edges.clear();
	}
	void clear() {
		for (int i = 0; i < edges.size(); i++)
			edges[i].flow = 0;
	}
	void reduce() {
		for (int i = 0; i < edges.size(); i++)
			edges[i].cap -= edges[i].flow;
	}
	void addedge(int from, int to, int cap) {
		edges.push_back(edge(from, to, cap, 0));
		edges.push_back(edge(to, from, 0, 0));
		m = edges.size();
		G[from].push_back(m - 2);
		G[to].push_back(m - 1);
	}
	bool BFS() {
		memset(vis, 0, sizeof(vis));
		queue<int> Q;
		Q.push(s);
		vis[s] = 1;
		d[s] = 0;
		while (!Q.empty()) {
			int x = Q.front(); Q.pop();
			for (int i = 0; i < G[x].size(); i++) {
				edge& e = edges[G[x][i]];
				if (!vis[e.to] && e.cap > e.flow) {
					vis[e.to] = 1;
					d[e.to] = d[x] + 1;
					Q.push(e.to);
				}
			}
		}
		return vis[t];
	}
	int DFS(int x, int a) {
		if (x == t || a == 0) return a;
		int flow = 0, f;
		for (int& i = cur[x]; i < G[x].size(); i++) {
			edge& e = edges[G[x][i]];
			if (d[x] + 1 == d[e.to] && (f = DFS(e.to, min(a, e.cap - e.flow))) > 0) {
				e.flow += f;
				edges[G[x][i] ^ 1].flow -= f;
				flow += f;
				a -= f;
				if (a == 0) break;
			}
		}
		return flow;
	}
	int Maxflow(int s, int t) {
		this->s = s; this->t = t;
		int flow = 0;
		while (BFS()) {
			memset(cur, 0, sizeof(cur));
			flow += DFS(s, inf);
		}
		return flow;
	}
	vector<int> Mincut() { // call this after maxflow
		vector<int> ans;
		for (int i = 0; i < edges.size(); i++) {
			edge& e = edges[i];
			if (vis[e.from] && !vis[e.to] && e.cap > 0)
				ans.push_back(i);
		}
		return ans;
	}
}dinic;
int main()
{
	freopen("D:\\in.txt", "r", stdin);
	int n, m;
	scanf("%d %d", &n, &m);
	dinic.init(n + 5);
	while (m--) {
		int s, t, u;
		scanf("%d %d %d", &s, &t, &u);
		dinic.addedge(s, t, u);
	}
	auto start = clock();
	printf("%d\n", dinic.Maxflow(1, n));
	double tot = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
	printf("Dinic: %f\n", tot);
	return 0;
}
