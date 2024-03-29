/*
在调用dominator_tree::add_edge之前要先调用init函数进行初始化，init的参数n表示支配树中有n的点，编号为1-n。
idom[x]表示点x的最近支配点, idom[root] == 0。
head保存为原图的链表。
back保存为反图的链表。
dfn[x]表示结点x的DFS序。
id[i]表示DFS序为i的结点编号。
tarjan(s)可以求出以s为起点的支配树。
性质：dfn[x] > dfn[idom[x]]，即结点x的DFS序一定大于它的支配点的DFS序
	因此可以按照DFS序从小到大枚举点x，此时它在支配树上的父结点已经计算完毕。
	for (int i = 1; i <= n; ++i) {
		int x = id[i];
		ans[x] = F(ans[idom[x]], x);
	}
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 210000;
const int maxedges = 310000;
struct dominator_tree {
	int n, cnt, tot, head[maxn], fa[maxn], p[maxn], top[maxn], back[maxn], val[maxn], dfn[maxn], id[maxn], semi[maxn], idom[maxn];
	struct edge {
		int to, next;
	}e[maxedges * 3]; //min: maxedges * 2 + maxn
	inline bool cmp(int x, int y) {
		return dfn[semi[x]] > dfn[semi[y]];
	}
	inline void ins(int* first, int from, int to) {
		e[++cnt].to = to;
		e[cnt].next = first[from];
		first[from] = cnt;
	}
	void add_edge(int x, int y) {
		ins(head, x, y);
		ins(back, y, x);
	}
	void dfs(int x) {
		id[dfn[x] = ++tot] = x;
		for (int i = head[x]; i; i = e[i].next)
			if (!dfn[e[i].to])
				fa[e[i].to] = x, dfs(e[i].to);
	}
	int find(int x) {
		if (p[x] == x) return x;
		int y = find(p[x]);
		if (cmp(val[x], val[p[x]]))
			val[x] = val[p[x]];
		return p[x] = y;
	}
	void tarjan(int s) {
		dfs(s);
		for (int i = tot; i > 1; --i) {
			int u = id[i];
			for (int j = back[u]; j; j = e[j].next) {
				if (dfn[e[j].to]) {
					find(e[j].to);
					if (cmp(u, val[e[j].to]))
						semi[u] = semi[val[e[j].to]];
				}
			}
			ins(top, semi[u], u);
			p[u] = fa[u];
			u = fa[u];
			for (int j = top[u]; j; j = e[j].next) {
				find(e[j].to);
				if (semi[val[e[j].to]] == semi[u])
					idom[e[j].to] = u;
				else
					idom[e[j].to] = val[e[j].to];
			}
			top[u] = 0;
		}
		for (int i = 2; i <= tot; ++i) {
			int x = id[i];
			if (idom[x] != semi[x])
				idom[x] = idom[idom[x]];
		}
	}
	void init(int n) {
		this->n = n;
		cnt = tot = 0;
		memset(head, 0, sizeof(int) * (n + 2));
		memset(back, 0, sizeof(int) * (n + 2));
		memset(top, 0, sizeof(int) * (n + 2));
		memset(dfn, 0, sizeof(int) * (n + 2));
		memset(idom, 0, sizeof(int) * (n + 2));
		for (int i = 1; i <= n; ++i)
			val[i] = semi[i] = p[i] = i;
	}
}tree;
long long ans[maxn];
int main() {
	//freopen("in.txt", "r", stdin);
	int n, m;
	scanf("%d %d", &n, &m);
	tree.init(n);
	for (int i = 1; i <= m; ++i) {
		int u, v;
		scanf("%d %d", &u, &v);
		tree.add_edge(u, v);
	}
	tree.tarjan(1);
	for (int i = 1; i <= n; ++i)
		ans[i] = 1;
	for (int i = n; i >= 1; --i) {
		int x = tree.id[i];
		ans[tree.idom[x]] += ans[x];
	}
	for (int i = 1; i <= n; ++i)
		printf("%d ", ans[i]);
	printf("\n");
	return 0;
}