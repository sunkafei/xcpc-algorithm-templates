#include <cstdio>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 1100;
const int inf = 1 << 30;
int A[2000][2000];
int cur = 0, tot, mn, mx;
struct area {
	int x1, x2, y1, y2;
	int a, b, c, d;
	int sum, min, max, set, add; //set必须是非负数
}t[maxn * maxn * 4];
void init() {
	cur = 0;
	t[0].min = inf;
	t[0].max = -inf;
	t[0].sum = 0;
}
void maintain(int p) { //maintain函数用来维护结点信息，应当保证多次对一个结点调用maintain结果不变
	if (p == 0) return;
	int a = t[p].a, b = t[p].b, c = t[p].c, d = t[p].d;
	t[p].sum = t[p].min = t[p].max = 0;
	if (t[p].x1 != t[p].x2 || t[p].y1 != t[p].y2) {
		t[p].sum = t[a].sum + t[b].sum + t[c].sum + t[d].sum;
		t[p].min = min({ t[a].min, t[b].min, t[c].min, t[d].min });
		t[p].max = max({ t[a].max, t[b].max, t[c].max, t[d].max });
	}
	if (t[p].set >= 0) {
		t[p].min = t[p].max = t[p].set;
		t[p].sum = t[p].set * (t[p].x2 - t[p].x1 + 1) * (t[p].y2 - t[p].y1 + 1);
	}
	if (t[p].add) {
		t[p].min += t[p].add;
		t[p].max += t[p].add;
		t[p].sum += t[p].add * (t[p].x2 - t[p].x1 + 1) * (t[p].y2 - t[p].y1 + 1);
	}
}
void pushdown(int p) { //pushdown将标记传递给子结点，不影响当前结点的信息。
	int a = t[p].a, b = t[p].b, c = t[p].c, d = t[p].d;
	if (t[p].set >= 0) {
		t[a].set = t[b].set = t[c].set = t[d].set = t[p].set;
		t[a].add = t[b].add = t[c].add = t[d].add = 0;
		t[p].set = -1;
	}
	if (t[p].add) {
		t[a].add += t[p].add;
		t[b].add += t[p].add;
		t[c].add += t[p].add;
		t[d].add += t[p].add;
		t[p].add = 0;
	}
}
int build(int x1, int x2, int y1, int y2) {
	int p = ++cur;
	t[p].x1 = x1;
	t[p].x2 = x2;
	t[p].y1 = y1;
	t[p].y2 = y2;
	t[p].a = t[p].b = t[p].c = t[p].d = 0;
	t[p].add = 0; t[p].set = -1; //清空结点标记
	if (x1 == x2 && y1 == y2) {
		t[p].add = A[x1][y1];
	}
	else {
		int xm = (x1 + x2) >> 1;
		int ym = (y1 + y2) >> 1;
		t[p].a = build(x1, xm, y1, ym);
		if (ym < y2)
			t[p].b = build(x1, xm, ym + 1, y2);
		if (xm < x2)
			t[p].c = build(xm + 1, x2, y1, ym);
		if (xm < x2 && ym < y2)
			t[p].d = build(xm + 1, x2, ym + 1, y2);
	}
	maintain(p);
	return p;
}
void update(int p, int U, int D, int L, int R, int op, int v) { //[U, D]对应行号区间，[L, R]对应列号区间。
	if (U <= t[p].x1 && D >= t[p].x2 && L <= t[p].y1 && R >= t[p].y2) {
		if (op == 0) t[p].add += v;
		else {
			t[p].set = v;
			t[p].add = 0;
		}
	}
	else {
		pushdown(p);
		int xm = (t[p].x1 + t[p].x2) >> 1;
		int ym = (t[p].y1 + t[p].y2) >> 1;
		if (U <= xm && L <= ym)
			update(t[p].a, U, D, L, R, op, v);
		else
			maintain(t[p].a);
		if (U <= xm && ym < R && ym < t[p].y2)
			update(t[p].b, U, D, L, R, op, v);
		else
			maintain(t[p].b);
		if (xm < D && L <= ym && xm < t[p].x2)
			update(t[p].c, U, D, L, R, op, v);
		else
			maintain(t[p].c);
		if (xm < D && ym < R && xm < t[p].x2 && ym < t[p].y2)
			update(t[p].d, U, D, L, R, op, v);
		else
			maintain(t[p].d);
	}
	maintain(p);
}
void query(int p, int U, int D, int L, int R) { //调用之前要设置：mn = inf; mx = -inf; tot = 0;	
	if (U <= t[p].x1 && D >= t[p].x2 && L <= t[p].y1 && R >= t[p].y2) {
		tot += t[p].sum;
		mn = min(mn, t[p].min);
		mx = max(mx, t[p].max);
	}
	else {
		pushdown(p);
		maintain(t[p].a);
		maintain(t[p].b);
		maintain(t[p].c);
		maintain(t[p].d);
		int xm = (t[p].x1 + t[p].x2) >> 1;
		int ym = (t[p].y1 + t[p].y2) >> 1;
		if (U <= xm && L <= ym)
			query(t[p].a, U, D, L, R);
		if (U <= xm && ym < R && ym < t[p].y2)
			query(t[p].b, U, D, L, R);
		if (xm < D && L <= ym && xm < t[p].x2)
			query(t[p].c, U, D, L, R);
		if (xm < D && ym < R && xm < t[p].x2 && ym < t[p].y2)
			query(t[p].d, U, D, L, R);
	}
}
int main() {
#define ran engine(e)
	int n = 500, m = 500, q = 10000;
	default_random_engine e;
	e.seed(123);
	uniform_int_distribution<unsigned> engine(1, n);
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= m; ++j)
			A[i][j] = ran;
	init();
	int root = build(1, n, 1, m);
	while (q--) {
		int a = ran, b = ran, c = ran, d = ran;
		int x1 = min(a, b), x2 = max(a, b), y1 = min(c, d), y2 = max(c, d);
		int tp = rand() % 3, v = ran;
		if (tp == 0) {
			for (int i = x1; i <= x2; ++i)
				for (int j = y1; j <= y2; ++j)
					A[i][j] += v;
			update(root, x1, x2, y1, y2, 0, v);
		}
		else if (tp == 1) {
			for (int i = x1; i <= x2; ++i)
				for (int j = y1; j <= y2; ++j)
					A[i][j] = v;
			update(root, x1, x2, y1, y2, 1, v);
		}
		else {
			mn = inf; mx = -inf; tot = 0;
			int MN = inf, MX = -inf, TOT = 0;
			for (int i = x1; i <= x2; ++i)
				for (int j = y1; j <= y2; ++j)
					MN = min(MN, A[i][j]), MX = max(MX, A[i][j]), TOT += A[i][j];
			query(root, x1, x2, y1, y2);
			if (MN != mn)
				printf("mn: %d %d\n", MN, mn);
			if (mx != MX)
				printf("mx: %d %d\n", MX, mx);
			if (tot != TOT)
				printf("sum: %d %d\n", TOT, tot);
		}
	}
	return 0;
}