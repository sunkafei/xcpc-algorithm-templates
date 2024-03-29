#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <random>
using namespace std;
const int maxn = 210000, offset = 210000;
const int inf = 1 << 30;
struct tmp {
	int data[maxn * 5];
	int& operator[] (int idx) {
		return data[idx + offset];
	}
}A;

#define lc t[p].lchild
#define rc t[p].rchild
int cur = 0, tot, mn, mx;
struct segment {
	int l, r, lchild, rchild;
	int sum, min, max, set, add;
}t[maxn * 4];
void init() {
	cur = 0;
}
inline void maintain(int p) {
	t[p].sum = t[lc].sum + t[rc].sum;
	t[p].min = min(t[lc].min, t[rc].min);
	t[p].max = max(t[lc].max, t[rc].max);
}
inline void mark(int p, int setv, int addv) { //给结点打标记
	if (setv >= 0) {
		t[p].set = setv; t[p].add = 0;
		t[p].min = t[p].max = setv;
		t[p].sum = setv * (t[p].r - t[p].l + 1);
	}
	if (addv) {
		t[p].add += addv;
		t[p].min += addv;
		t[p].max += addv;
		t[p].sum += addv * (t[p].r - t[p].l + 1);
	}
}
inline void pushdown(int p) { //pushdown将标记传递给子结点，不影响当前结点的信息。
	mark(lc, t[p].set, t[p].add);
	mark(rc, t[p].set, t[p].add);
	t[p].set = -1;
	t[p].add = 0;
}
int build(int L, int R) { //只要计算mid的方式是(L + R) >> 1而不是(L + R) / 2，就可以建立负坐标线段树。
	int p = ++cur;
	t[p].l = L;
	t[p].r = R;
	t[p].add = 0; t[p].set = -1; //清空结点标记
	if (t[p].l == t[p].r) {
		mark(p, 0, A[L]);
	}
	else {
		int mid = (t[p].l + t[p].r) >> 1;
		lc = build(L, mid);
		rc = build(mid + 1, R);
		maintain(p);
	}
	return p;
}
void update(int p, int L, int R, int op, int v) {
	if (L <= t[p].l && R >= t[p].r) {
		if (op == 0) 
			mark(p, -1, v);
		else 
			mark(p, v, 0);
	}
	else {
		pushdown(p); //如果没有pushdown只需要在最后调用一次maintain即可。
		int mid = (t[p].l + t[p].r) >> 1;
		if (L <= mid)
			update(lc, L, R, op, v);
		if (R > mid)
			update(rc, L, R, op, v);
		maintain(p);
	}
}
void update(int p, int pos, int v) { //单点修改
	if (t[p].l == t[p].r) {
		mark(p, -1, v);
	}
	else {
		pushdown(p);
		int mid = (t[p].l + t[p].r) >> 1;
		if (pos <= mid)
			update(lc, pos, v);
		else
			update(rc, pos, v);
		maintain(p);
	}
}
void query(int p, int L, int R) { //调用之前要设置：mn = inf; mx = -inf; tot = 0;							
	if (L <= t[p].l && R >= t[p].r) {
		tot += t[p].sum;
		mn = min(mn, t[p].min);
		mx = max(mx, t[p].max);
	}
	else {
		pushdown(p);
		int mid = (t[p].l + t[p].r) >> 1;
		if (L <= mid)
			query(lc, L, R);
		if (R > mid)
			query(rc, L, R);
	}
}
int main() {
	default_random_engine e;
	int n = 100000, m = 100000;
	uniform_int_distribution<int> d(-n, n);
	for (int i = -n; i <= n; ++i)
		A[i] = d(e);
	init();
	int root = build(-n, n);
	for (int i = 1; i <= m; ++i) {
		int op = rand() % 4, a = d(e), b = d(e), v = rand();
		int L = min(a, b), R = max(a, b);
		if (op == 0) {
			for (int i = L; i <= R; ++i)
				A[i] += v;
			update(root, L, R, op, v);
		}
		else if (op == 1) {
			for (int i = L; i <= R; ++i)
				A[i] = v;
			update(root, L, R, op, v);
		}
		else if (op == 2) {
			mn = inf; mx = -inf; tot = 0;
			query(root, L, R);
			if (mn != *min_element(A.data + offset + L, A.data + offset + R + 1))
				abort();
			if (mx != *max_element(A.data + offset + L, A.data + offset + R + 1))
				abort();
			if (tot != accumulate(A.data + offset + L, A.data + offset + R + 1, 0))
				abort();
		}
		else {
			A[L] += v;
			update(root, L, v);
		}
	}
	return 0;
}