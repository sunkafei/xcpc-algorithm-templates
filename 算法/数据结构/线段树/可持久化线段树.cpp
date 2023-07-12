/*
可持久化线段树：区间取min
每次update之后返回新版本的线段树的根结点。
查询操作传入相应版本的线段树的根结点即可。
因为要可持久化，所以必须标记永久化，不能pushdown。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 110000;
const int inf = 1 << 30;
struct node {
	int l, r, lc, rc;
	int minv, tag;
} t[maxn * 20];
int cur = 0, mn;
void init() {
	cur = 0;
}
void maintain(int p) {
	int lc = t[p].lc, rc = t[p].rc;
	t[p].minv = t[p].tag;
	if (t[p].r > t[p].l) {
		t[p].minv = min({ t[lc].minv, t[rc].minv, t[p].tag });
	}
}
int build(int L, int R) { //只要计算mid的方式是(L + R) >> 1而不是(L + R) / 2，就可以建立负坐标线段树。
	int p = ++cur;
	t[p].l = L;
	t[p].r = R;
	t[p].tag = inf; //重置结点标记
	if (L < R) {
		int mid = (L + R) >> 1;
		t[p].lc = build(L, mid);
		t[p].rc = build(mid + 1, R);
	}
	maintain(p);
	return p;
}
int update(int i, int L, int R, int v) { //区间取min
	int p = ++cur; t[p] = t[i];
	if (L <= t[p].l && R >= t[p].r) {
		t[p].tag = min(t[p].tag, v);
	}
	else {
		int mid = (t[p].l + t[p].r) >> 1;
		if (L <= mid)
			t[p].lc = update(t[p].lc, L, R, v);
		if (R > mid)
			t[p].rc = update(t[p].rc, L, R, v);
	}
	maintain(p);
	return p;
}
void query(int p, int L, int R) { //调用之前设置mn = inf，查询区间最小值
	mn = min(mn, t[p].tag);
	if (L <= t[p].l && R >= t[p].r) {
		mn = min(mn, t[p].minv);
	}
	else {
		int mid = (t[p].l + t[p].r) >> 1;
		if (L <= mid) 
			query(t[p].lc, L, R);
		if (R > mid) 
			query(t[p].rc, L, R);
	}
}
int main() {
	init();
	int root[10], n = 10;
	root[0] = build(1, n);
	root[1] = update(root[0], 1, 7, 10);
	root[2] = update(root[1], 5, 8, 0);
	mn = inf; query(root[2], 7, 7);
	printf("%d\n", mn);
	return 0;
}