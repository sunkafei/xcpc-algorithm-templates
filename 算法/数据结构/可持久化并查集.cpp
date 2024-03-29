#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 210000;
struct node {
	int lc, rc, fa, size;
} t[maxn * 40]; //若内存不够，可以考虑改成按深度合并
int cur, length;
void init(int n) {
	cur = 0;
	length = n;
}
int build(int L = 1, int R = length) {
	int p = ++cur;
	t[p].fa = L;
	t[p].size = 1;
	if (L < R) {
		int mid = (L + R) >> 1;
		t[p].lc = build(L, mid);
		t[p].rc = build(mid + 1, R);
	}
	return p;
}
int link(int i, int pos, int fa, int L = 1, int R = length) {
	int p = ++cur; t[p] = t[i];
	t[p].fa = fa;
	if (L < R) {
		int mid = (L + R) >> 1;
		if (pos <= mid)
			t[p].lc = link(t[p].lc, pos, fa, L, mid);
		else
			t[p].rc = link(t[p].rc, pos, fa, mid + 1, R);
	}
	return p;
}
int update(int i, int pos, int size, int L = 1, int R = length) {
	int p = ++cur; t[p] = t[i];
	if (L == R)
		t[p].size += size;
	else {
		int mid = (L + R) >> 1;
		if (pos <= mid)
			t[p].lc = update(t[p].lc, pos, size, L, mid);
		else
			t[p].rc = update(t[p].rc, pos, size, mid + 1, R);
	}
	return p;
}
int query(int p, int pos, int L = 1, int R = length) {
	if (L == R)
		return p;
	else {
		int mid = (L + R) >> 1;
		if (pos <= mid)
			return query(t[p].lc, pos, L, mid);
		else
			return query(t[p].rc, pos, mid + 1, R);
	}
}
int find(int i, int pos) {
	int p = query(i, pos);
	return t[p].fa == pos ? p : find(i, t[p].fa);
}
int join(int i, int x, int y) { //在版本为i的并查集中连接点x和点y，返回新的并查集
	int a = find(i, x);
	int b = find(i, y);
	if (t[a].fa != t[b].fa) { //如果x和y已经在一个集合中了，则不修改直接返回
		if (t[a].size > t[b].size)
			swap(a, b);
		int p = link(i, t[a].fa, t[b].fa);
		p = update(p, t[b].fa, t[a].size);
		return p;
	}
	return i;
}
bool same(int i, int x, int y) { //判断在版本为i的并查集中，点x和点y是否属于相同的集合
	int a = find(i, x);
	int b = find(i, y);
	return t[a].fa == t[b].fa;
}
int main() {
	//freopen("in.txt", "r", stdin);
	static int root[maxn];
	int n, m;
	scanf("%d %d", &n, &m);
	init(n);
	int r = root[0] = build();
	for (int i = 1; i <= m; ++i) {
		int tp, x, y, k;
		scanf("%d", &tp);
		if (tp == 1) {
			scanf("%d %d", &x, &y);
			r = join(r, x, y);
		}
		else if (tp == 2) {
			scanf("%d", &k);
			r = root[k];
		}
		else {
			scanf("%d %d", &x, &y);
			int ans = same(r, x, y);
			printf("%d\n", ans);
		}
		root[i] = r;
	}
	return 0;
}