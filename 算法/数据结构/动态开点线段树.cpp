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
	int lchild, rchild;
	int sum, min, max, set, add;
}t[maxn * 4];
void init() {
	cur = 0;
}
inline void newnode(int& p) {
	if (p == 0) {
		p = ++cur;
		memset(&t[p], 0, sizeof(t[p]));
		t[p].set = -1;
	}
}
inline void maintain(int p) {
	t[p].sum = t[lc].sum + t[rc].sum;
	t[p].min = min(t[lc].min, t[rc].min);
	t[p].max = max(t[lc].max, t[rc].max);
}
inline void mark(int &p, int setv, int addv, int x, int y) { //给结点打标记
	newnode(p);
	if (setv >= 0) {
		t[p].set = setv; t[p].add = 0;
		t[p].min = t[p].max = setv;
		t[p].sum = setv * (y - x + 1);
	}
	if (addv) {
		t[p].add += addv;
		t[p].min += addv;
		t[p].max += addv;
		t[p].sum += addv * (y - x + 1);
	}
}
inline void pushdown(int p, int x, int y) { //pushdown将标记传递给子结点，不影响当前结点的信息。
	int mid = (x + y) >> 1;
	mark(lc, t[p].set, t[p].add, x, mid);
	mark(rc, t[p].set, t[p].add, mid+1, y);
	t[p].set = -1;
	t[p].add = 0;
}
void update(int &p, int L, int R, int op, int v, int x, int y) {
	newnode(p);
	if (L <= x && R >= y) {
		if (op == 0)
			mark(p, -1, v, x, y);
		else
			mark(p, v, 0, x, y);
	}
	else {
		pushdown(p, x, y); //如果没有pushdown只需要在最后调用一次maintain即可。
		int mid = (x + y) >> 1;
		if (L <= mid)
			update(lc, L, R, op, v, x, mid);
		if (R > mid)
			update(rc, L, R, op, v, mid+1, y);
		maintain(p);
	}
}
void query(int &p, int L, int R, int x, int y) { //调用之前要设置：mn = inf; mx = -inf; tot = 0;							
	newnode(p);
	if (L <= x && R >= y) {
		tot += t[p].sum;
		mn = min(mn, t[p].min);
		mx = max(mx, t[p].max);
	}
	else {
		pushdown(p, x, y);
		int mid = (x + y) >> 1;
		if (L <= mid)
			query(lc, L, R, x, mid);
		if (R > mid)
			query(rc, L, R, mid+1, y);
	}
}
int main() {
	default_random_engine e;
	int n = 100000, m = 100000;
	uniform_int_distribution<int> d(-n, n);
	for (int i = -n; i <= n; ++i)
		A[i] = 0;
	init();
	int root = 0;
	for (int i = 1; i <= m; ++i) {
		int op = rand() % 3, a = d(e), b = d(e), v = rand();
		int L = min(a, b), R = max(a, b);
		if (op == 0) {
			for (int i = L; i <= R; ++i)
				A[i] += v;
			update(root, L, R, op, v, -n, n);
		}
		else if (op == 1) {
			for (int i = L; i <= R; ++i)
				A[i] = v;
			update(root, L, R, op, v, -n, n);
		}
		else {
			mn = inf; mx = -inf; tot = 0;
			query(root, L, R, -n, n);
			if (mn != *min_element(A.data + offset + L, A.data + offset + R + 1))
				abort();
			if (mx != *max_element(A.data + offset + L, A.data + offset + R + 1))
				abort();
			if (tot != accumulate(A.data + offset + L, A.data + offset + R + 1, 0))
				abort();
		}
	}
	return 0;
}