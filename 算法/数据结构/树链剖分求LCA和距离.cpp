#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
const int maxn = 510000;
const int inf = 1 << 30;
int dep[maxn], sz[maxn], pa[maxn], son[maxn], top[maxn];
vector<int> G[maxn];
void dfs1(int u, int fa, int d) {
	dep[u] = d;
	sz[u] = 1;
	son[u] = 0;
	pa[u] = fa;
	for (auto v : G[u]) if (v != fa) {
		dfs1(v, u, d + 1);
		sz[u] += sz[v];
		if (sz[son[u]] < sz[v])
			son[u] = v;
	}
}
void dfs2(int u, int tp) {
	top[u] = tp;
	if (son[u])
		dfs2(son[u], tp);
	for (auto v : G[u]) if (v != pa[u] && v != son[u])
		dfs2(v, v);
}
int lca(int x, int y) {
	while (top[x] != top[y]) {
		if (dep[top[x]] < dep[top[y]])
			swap(x, y);
		x = pa[top[x]];
	}
	return dep[x] < dep[y] ? x : y;
}
int dist(int x, int y) {
	return dep[x] + dep[y] - 2 * dep[lca(x, y)];
}
void init(int root) {
	dfs1(root, 0, 1);
	dfs2(root, root);
}
int main() {
	int n, m, s;
	scanf("%d %d %d", &n, &m, &s);
	for (int i = 0; i < n - 1; ++i) {
		int x, y;
		scanf("%d %d", &x, &y);
		G[x].push_back(y);
		G[y].push_back(x);
	}
	init(s);
	while (m--) {
		int x, y;
		scanf("%d %d", &x, &y);
		printf("%d\n", dist(x, y));
	}
	return 0;
}