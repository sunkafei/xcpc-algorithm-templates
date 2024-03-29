/*
1. 标记(a, b)表示对区间中的所有值v执行：v = max(v + a, b)
2. 将标记(c, d)合并到(a, b)上得到：(a + c, max(b + c, d))
3. 标记(a, b)与(c, d)取最大得到：(max(a, c), max(b, d))
4. 必须满足inf > 操作次数 * 单次修改的最大增加值
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#define lc t[p].lchild
#define rc t[p].rchild
using namespace std;
const int maxn = 510000;
const long long inf = 1LL << 50;
long long A[maxn], mx, hmax;
int cur = 0;
struct segment {
	int l, r, lchild, rchild;
	long long max, hmax; //最大值、历史最大值
	long long a, b, x, y; //(a, b)为当前标记，(x, y)为历史最大标记	
}t[maxn * 4];
inline void maintain(int p) {
	t[p].max = max(t[lc].max, t[rc].max);
	t[p].hmax = max(t[p].hmax, t[p].max);
}
inline void mark(int p, long long c, long long d) {
	t[p].a += c;
	t[p].b = max(t[p].b + c, d);
	t[p].max = max(t[p].max + c, d);
	t[p].x = max(t[p].x, t[p].a);
	t[p].y = max(t[p].y, t[p].b);
	t[p].hmax = max(t[p].hmax, t[p].max);
	t[p].a = max(t[p].a, -inf); //防止出现：(操作次数 * inf) > LLONG_MAX
}
inline void change(int p, long long c, long long d) {
	long long i = t[p].a + c, j = max(t[p].b + c, d);
	t[p].x = max(t[p].x, i);
	t[p].y = max(t[p].y, j);
	t[p].hmax = max(t[p].hmax, max(t[p].max + c, d));
}
inline void pushdown(int p) {
	change(lc, t[p].x, t[p].y);
	change(rc, t[p].x, t[p].y);
	mark(lc, t[p].a, t[p].b);
	mark(rc, t[p].a, t[p].b);
	t[p].x = t[p].a = 0;
	t[p].y = t[p].b = -inf;
}
int build(int L, int R) {
	int p = ++cur;
	t[p].l = L;
	t[p].r = R;
	t[p].x = t[p].a = 0;
	t[p].y = t[p].b = -inf;
	t[p].hmax = -inf;
	if (t[p].l == t[p].r) {
		t[p].hmax = t[p].max = A[L];
	}
	else {
		int mid = (t[p].l + t[p].r) >> 1;
		lc = build(L, mid);
		rc = build(mid + 1, R);
		maintain(p);
	}
	return p;
}
void query(int p, int L, int R) {
	if (L <= t[p].l && R >= t[p].r) {
		mx = max(mx, t[p].max);
		hmax = max(hmax, t[p].hmax);
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
void update(int p, int L, int R, long long a, long long b) {
	if (L <= t[p].l && R >= t[p].r) {
		mark(p, a, b);
	}
	else {
		pushdown(p);
		int mid = (t[p].l + t[p].r) >> 1;
		if (L <= mid)
			update(lc, L, R, a, b);
		if (R > mid)
			update(rc, L, R, a, b);
		maintain(p);
	}
}
int main() {
	//freopen("in.txt", "r", stdin);
	int n, m;
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i)
		scanf("%lld", &A[i]);
	int root = build(1, n);
	scanf("%d", &m);
	while (m--) {
		char tp;
		int L, R;
		long long v;
		scanf(" %c %d %d\n", &tp, &L, &R);
		if (tp == 'Q') { //查询区间最大值
			mx = -inf;
			query(root, L, R);
			printf("%lld\n", mx);
		}
		else if (tp == 'A') { //查询区间历史最大值
			hmax = -inf;
			query(root, L, R);
			printf("%lld\n", hmax);
		}
		else if (tp == 'P') { //区间add
			scanf("%lld", &v);
			update(root, L, R, v, -inf);
		}
		else { //区间set
			scanf("%lld", &v);
			update(root, L, R, -inf, v);
		}
	}
	return 0;
}