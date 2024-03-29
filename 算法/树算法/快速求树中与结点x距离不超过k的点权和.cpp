#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 110000;
const int inf = 1 << 30;
int head[maxn * 2], nxt[maxn * 2], to[maxn * 2];
int vis[maxn], pa[maxn], pos, sz, cur; //pa[x]表示在点分树上结点x的父亲
int value[maxn], root[maxn], root2[maxn];
int dep[maxn], Size[maxn], father[maxn], son[maxn], top[maxn];
void DFS1(int u, int fa, int d) {
	dep[u] = d;
	Size[u] = 1;
	son[u] = 0;
	father[u] = fa;
	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		if (v != fa) {
			DFS1(v, u, d + 1);
			Size[u] += Size[v];
			if (Size[son[u]] < Size[v])
				son[u] = v;
		}
	}
}
void DFS2(int u, int tp) {
	top[u] = tp;
	if (son[u])
		DFS2(son[u], tp);
	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		if (v != father[u] && v != son[u])
			DFS2(v, v);
	}
}
int lca(int x, int y) {
	while (top[x] != top[y]) {
		if (dep[top[x]] < dep[top[y]])
			swap(x, y);
		x = father[top[x]];
	}
	return dep[x] < dep[y] ? x : y;
}
int dist(int x, int y) {
	return dep[x] + dep[y] - 2 * dep[lca(x, y)];
}
struct TreeSet {
	struct node {
		int l, r, v;
	} T[maxn * 150];
	int left, right, sz; //空的线段树标号为0
	void init(int L, int R) { //所有线段树对应区间都为[L, R]
		left = L;
		right = R;
		sz = 1;
	}
	int newnode() {
		T[sz].l = T[sz].r = T[sz].v = 0;
		return sz++;
	}
	void insert(int x, int pos, int value) { //编号为x的线段树的pos位置上的值加上value
		int L = left, R = right;
		while (L < R) {
			int M = (L + R) >> 1;
			T[x].v += value;
			if (pos <= M) {
				if (!T[x].l)
					T[x].l = newnode();
				x = T[x].l;
				R = M;
			}
			else {
				if (!T[x].r)
					T[x].r = newnode();
				x = T[x].r;
				L = M + 1;
			}
		}
		T[x].v += value;
	}
	int sum(int x, int pos) { //在编号为x的线段树上计算pos位置的前缀和
		int L = left, R = right, res = 0;
		while (x && L < R) {
			int M = (L + R) >> 1, t = T[T[x].l].v;
			if (pos <= M)
				x = T[x].l, R = M;
			else
				x = T[x].r, L = M + 1, res += t;
		}
		res += T[x].v;
		return res;
	}
}tree;
void init() {
	cur = 0;
	memset(head, 0, sizeof(head));
	memset(vis, 0, sizeof(vis));
}
void insert(int u, int v) {
	nxt[++cur] = head[u];
	head[u] = cur;
	to[cur] = v;
}
int dfs0(int u, int fa) { //求出以u为根的子树的大小 
	int tot = 1;
	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		if (v != fa && !vis[v])
			tot += dfs0(v, u);
	}
	return tot;
}
int dfs1(int u, int fa, int cnt) { //求出以u为根的子树的重心 
	int tot = 1, num = 0;
	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		if (v != fa && !vis[v]) {
			int result = dfs1(v, u, cnt);
			tot += result;
			num = max(num, result);
		}
	}
	num = max(num, cnt - tot);
	if (num < sz) {
		sz = num;
		pos = u;
	}
	return tot;
}
void dfs2(int u, int fa, int host) {
	tree.insert(root[host], dist(u, host), value[u]);
	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		if (v != fa && !vis[v]) {
			dfs2(v, u, host);
		}
	}
}
void dfs3(int u, int fa, int host) {
	tree.insert(root2[host], dist(u, pa[host]), value[u]);
	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		if (v != fa && !vis[v]) {
			dfs3(v, u, host);
		}
	}
}
void build(int u, int cnt, int pre = 0) { //递归处理子树u，cnt为子树u中的结点总数
	sz = inf;
	dfs1(u, -1, cnt); //求出以u为根的子树的重心root，作为新的根结点
	int root = pos;
	vis[root] = 1; //标记root结点不能再被访问
	pa[root] = pre;
	dfs2(root, -1, root);
	if (pre) dfs3(root, -1, root);
	for (int i = head[root]; i; i = nxt[i]) {
		int v = to[i];
		if (!vis[v]) {
			build(v, dfs0(v, -1), root);
		}
	}
}
long long query(int x, int k) {
	long long res = tree.sum(root[x], k);
	for (int i = x; pa[i]; i = pa[i]) {
		int d = dist(x, pa[i]);
		if (k - d >= 0) {
			res += tree.sum(root[pa[i]], k - d);
			res -= tree.sum(root2[i], k - d);
		}
	}
	return res;
}
void update(int x, int delta) {
	tree.insert(root[x], 0, delta);
	for (int i = x; pa[i]; i = pa[i]) {
		int d = dist(pa[i], x);
		tree.insert(root[pa[i]], d, delta);
		tree.insert(root2[i], d, delta);
	}
}
int main() {
//	freopen("in.txt", "r", stdin);
	int n, m;
	scanf("%d %d", &n, &m);
	init();
	tree.init(0, 110000);
	for (int i = 1; i <= n; ++i)
		root[i] = tree.newnode();
	for (int i = 1; i <= n; ++i)
		root2[i] = tree.newnode();
	for (int i = 1; i <= n; ++i)
		scanf("%d", &value[i]);
	for (int i = 1; i < n; ++i) {
		int u, v;
		scanf("%d %d", &u, &v);
		insert(u, v);
		insert(v, u);
	}
	DFS1(1, 0, 1);
	DFS2(1, 1);
	build(1, n);
	long long ans = 0;
	while (m--) {
		long long tp, x, y;
		scanf("%lld %lld %lld", &tp, &x, &y);
		x ^= ans; y ^= ans;
		if (tp == 0) {
			ans = query(x, y);
			printf("%lld\n", ans);
		}
		else {
			int delta = y - value[x];
			value[x] = y;
			update(x, delta);
		}
	}
	return 0;
}
