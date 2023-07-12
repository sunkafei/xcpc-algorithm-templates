#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
using namespace std;
const int maxn = 210000; //maxn应当大于最大字符串长度的2倍
int par[maxn], len[maxn], go[maxn][26];
int ch[maxn][2], fa[maxn], stk[maxn], pos[maxn];
int cur, root, last;
struct stnode {
	int l, r;
	int lc, rc;
	long long a, d;
};
struct segment {
	stnode tree[maxn * 4];
	int tc;
	int root;
	int init_tree(int l, int r) {
		int mid = (l + r) / 2;
		int pos = ++tc;
		tree[pos].l = l;
		tree[pos].r = r;
		tree[pos].a = tree[pos].d = 0;
		if (l != r) {
			tree[pos].lc = init_tree(l, mid);
			tree[pos].rc = init_tree(mid + 1, r);
		}
		return pos;
	}
	long long query_tree(int pos, int p) {
		int mid = (tree[pos].l + tree[pos].r) / 2;
		if (tree[pos].l == tree[pos].r) {
			return tree[pos].a;
		}
		else {
			if (p <= mid) {
				return query_tree(tree[pos].lc, p) + tree[pos].a + (p - tree[pos].l) * tree[pos].d;
			}
			else {
				return query_tree(tree[pos].rc, p) + tree[pos].a + (p - tree[pos].l) * tree[pos].d;
			}
		}
	}
	void update_tree(int pos, int l, int r, long long a, long long d) {
		int mid = (tree[pos].l + tree[pos].r) / 2;
		if (tree[pos].l == l and r == tree[pos].r) {
			tree[pos].a += a;
			tree[pos].d += d;
		}
		else {
			if (r <= mid) {
				update_tree(tree[pos].lc, l, r, a, d);
			}
			else if (l <= mid) {
				update_tree(tree[pos].lc, l, mid, a, d);
				update_tree(tree[pos].rc, mid + 1, r, a + (mid + 1 - l) * d, d);
			}
			else {
				update_tree(tree[pos].rc, l, r, a, d);
			}
		}
	}
	long long query(int p) {
		return query_tree(root, p + 1);
	}
	void update(int l, int r, long long a, long long d) {
		if (l <= r)
			update_tree(root, l + 1, r + 1, a, d); //下标从1开始
	}
	void init(int x) {
		tc = 0;
		root = init_tree(1, x);
	}
} tree;
void solve(int L, int R, int x, int index) {
	if (x == -1) {
		tree.update(index - R + 1, index - L + 1, R - L + 1, -1);
	}
	else {
		int length = min(R - L + 1, index - x);
		tree.update(x - R + 2, x - R + length, 1, 1);
		tree.update(index - L - length + 3, index - L + 1, length - 1, -1);
		tree.update(x - R + length + 1, index - L - length + 2, length, 0);
	}
}
inline bool son(int x) {
	return ch[fa[x]][1] == x;
}
inline bool isroot(int x) {
	return ch[fa[x]][1] != x && ch[fa[x]][0] != x;
}
inline void pushdown(int x) {
	pos[ch[x][0]] = pos[ch[x][1]] = pos[x];
}
void rotate(int x) {
	int y = fa[x], z = fa[y], c = son(x);
	if (!isroot(y))
		ch[z][son(y)] = x;
	fa[x] = z;
	ch[y][c] = ch[x][!c];
	fa[ch[y][c]] = y;
	ch[x][!c] = y;
	fa[y] = x;
}
void splay(int x) {
	int top = 0;
	stk[++top] = x;
	for (int i = x; !isroot(i); i = fa[i])
		stk[++top] = fa[i];
	while (top)
		pushdown(stk[top--]);
	for (int y = fa[x]; !isroot(x); rotate(x), y = fa[x]) if (!isroot(y))
		son(x) ^ son(y) ? rotate(x) : rotate(y);
}
void access(int nd, int index) {
	for (int y = nd, x = fa[nd]; y > 1; y = x, x = fa[x]) {
		if (x) {
			splay(x);
			ch[x][1] = y;
		}
		solve(len[x] + 1, len[y], pos[y], index);
	}
}
void cut(int x) { //断开结点x与它的父结点之间的边
	splay(x);
	fa[ch[x][0]] = fa[x];
	fa[x] = ch[x][0] = 0;
}
int newstate(int length) {
	len[cur] = length;
	return cur++;
}
void init() {
	memset(par, 0, sizeof(par));
	memset(ch, 0, sizeof(ch));
	memset(fa, 0, sizeof(fa));
	memset(go, 0, sizeof(go));
	memset(pos, -1, sizeof(pos));
	cur = 1;
	root = last = newstate(0);
}
void extend(int w, int index) {
	int x = last;
	int nx = newstate(len[x] + 1);
	while (x && !go[x][w])
		go[x][w] = nx, x = par[x];
	if (!x)
		par[nx] = root, fa[nx] = root;
	else {
		int y = go[x][w];
		if (len[x] + 1 == len[y])
			par[nx] = y, fa[nx] = y;
		else {
			int ny = newstate(len[x] + 1);
			memcpy(go[ny], go[y], sizeof(go[y]));
			fa[ny] = par[y], par[ny] = par[y];
			cut(y);
			pos[ny] = pos[y];
			fa[y] = ny, par[y] = ny;
			fa[nx] = ny, par[nx] = ny;
			while (x && go[x][w] == y)
				go[x][w] = ny, x = par[x];
		}
	}
	last = nx;
	access(last, index);
	splay(last);
	pos[last] = index;
}
char s[maxn];
vector<pair<int, int>> Q[maxn]; //多组数据时要清空
long long answer[maxn];
int main() {
	//freopen("in.txt", "r", stdin);
	scanf("%s", s);
	int n = strlen(s);
	init();
	tree.init(n + 10);
	int m;
	scanf("%d", &m);
	for (int i = 0; i < m; ++i) {
		int L, R;
		scanf("%d %d", &L, &R);
		--L; --R;
		Q[R].emplace_back(L, i);
	}
	for (int i = 0; i < n; ++i) {
		extend(s[i] - 'a', i);
		for (auto [L, id] : Q[i])
			answer[id] = tree.query(L);
	}
	for (int i = 0; i < m; ++i)
		printf("%lld\n", answer[i]);
	return 0;
}