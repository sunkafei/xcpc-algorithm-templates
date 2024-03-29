/*
ins函数会在每个结点i第一次入栈的时候，将E[i]清空。这样在最坏情况下空间复杂度可达O(nlogn)，
可以改成在每次询问后用一个DFS清空E，这样也E就不会占用额外内存了。
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
const int maxn = 1110000;
int dep[maxn], sz[maxn], pa[maxn], son[maxn], top[maxn], stk[maxn], dfn[maxn], clk, tp;
vector<int> G[maxn], E[maxn];
void dfs1(int u, int fa, int d) {
	dfn[u] = ++clk;
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
	clk = 0; stk[0] = -1;
	dfs1(root, 0, 1);
	dfs2(root, root);
}
void ins(int x) {
	if (tp > 0) {
		int fa = lca(stk[tp], x);
		if (fa != stk[tp]) {
			while (tp > 1 && dep[fa] < dep[stk[tp - 1]]) {
				E[stk[tp - 1]].push_back(stk[tp]);
				--tp;
			}
			int last = stk[tp--];
			if (fa != stk[tp]) {
				E[fa].clear();
				stk[++tp] = fa;
			}
			E[fa].push_back(last);
		}
	}
	E[x].clear();
	stk[++tp] = x;
}
int build(vector<int> nodes) { //对集合nodes中的结点建立虚树，并返回根结点。
	sort(nodes.begin(), nodes.end(), [](int x, int y) {
		return dfn[x] < dfn[y];
	});
	for (auto i : nodes)
		ins(i);
	while (--tp) 
		E[stk[tp]].push_back(stk[tp + 1]);
	return stk[1];
}

int main() {
	return 0;
}