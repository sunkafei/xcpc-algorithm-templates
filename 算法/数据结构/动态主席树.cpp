#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <utility>
#define lowbit(x) ((x) & (-x))
using namespace std;
const int inf = 1 << 30;
struct Dynamic_ZXTree {
	static const int maxn = 51000;
	struct node {
		int l, r, v; //左节点编号 右节点编号 值
	} T[maxn * 100]; //size(T) = maxn * logmaxn * logmaxn, size(data) = maxn + maxq
	int n, sz, length, nx, ny, data[maxn * 3], seq[maxn], X[maxn], Y[maxn], C[maxn]; //n是值域线段树的长度
	void sum(int x, int y) {
		nx = ny = 0;
		for (int i = x; i > 0; i -= lowbit(i))
			X[nx++] = C[i];
		for (int i = y; i > 0; i -= lowbit(i))
			Y[ny++] = C[i];
	}
	void add(int x, int value, int v) { //内部函数，不要在类外访问！
		int p = rank(value);
		for (int i = x; i <= length; i += lowbit(i))
			ins(C[i], 1, n, p, v);
	}
	void set(int x, int value) { //将序列中下标为x的位置的数修改为value
		add(x, seq[x], -1);
		add(x, value, 1);
		seq[x] = value;
	}
	void ins(int& i, int l, int r, int p, int v = 1) {
		int m = (l + r) / 2;
		if (i == 0) { 
			T[++sz] = T[i];
			i = sz; 
		}
		T[i].v += v;
		if (l == r) return;
		if (p <= m) ins(T[i].l, l, m, p, v);
		else ins(T[i].r, m + 1, r, p, v);
	}
	int rank(int v) {
		//如果不需要数据离散化直接return v即可，同时还要将所有的data[v]改为v，此时线段树的值域为[1, n]。
		return lower_bound(data + 1, data + n + 1, v) - data;
	}
	void init(int* A, int length, int* all, int size)	//输入数组A的下标从1开始，length表示A的长度
	{													//数组all记录所有可能出现的值（包括可能修改为的值），下标从1开始，size为数组all的长度
		this->length = length;
		copy(A + 1, A + length + 1, seq + 1);
		copy(all + 1, all + size + 1, data + 1);	//data[i]储存原序列中所有可能出现的值（包括可能修改成的值）离散化之后第i小的值
		sort(data + 1, data + size + 1);			//如果不用离散化，赋值data[i] = i
		n = unique(data + 1, data + size + 1) - data - 1;
		for (int i = 1; i <= length; ++i)
			add(i, rank(seq[i]), 1);
	}
	void left() {
		for (int i = 0; i < ny; ++i)
			Y[i] = T[Y[i]].l;
		for (int i = 0; i < nx; ++i)
			X[i] = T[X[i]].l;
	}
	void right() {
		for (int i = 0; i < ny; ++i)
			Y[i] = T[Y[i]].r;
		for (int i = 0; i < nx; ++i)
			X[i] = T[X[i]].r;
	}
	int left_value() {
		int tot = 0;
		for (int i = 0; i < ny; ++i)
			tot += T[T[Y[i]].l].v;
		for (int i = 0; i < nx; ++i)
			tot -= T[T[X[i]].l].v;
		return tot;
	}
	int value() {
		int tot = 0;
		for (int i = 0; i < ny; ++i)
			tot += T[Y[i]].v;
		for (int i = 0; i < nx; ++i)
			tot -= T[X[i]].v;
		return tot;
	}
	int kth(int x, int y, int k) { //查询原数组中区间[x, y]中的第k小的值，如果k大于区间长度会返回无效值！！！ 
		int l = 1, r = n;
		sum(x - 1, y);
		while (l < r) {
			int m = (l + r) / 2, t = left_value();
			if (k <= t)
				left(), r = m;
			else
				right(), l = m + 1, k -= t;
		}
		return data[r];
	}
	int ask(int x, int y, int v) { //查询原数组中区间[x, y]中比v小的值的个数 
		int l = 1, r = n, k = 0;
		sum(x - 1, y);
		int p = rank(v) - 1;
		if (p <= 0) return 0;
		while (l < r) {
			int m = (l + r) / 2;
			if (p <= m)
				left(), r = m;
			else {
				k += left_value();
				right();
				l = m + 1;
			}
		}
		k += value();
		return k;
	}
	int pre(int x, int y, int v) { //在区间[x, y]中查询v的前驱，找不到返回-inf
		int k = ask(x, y, v);
		if (k == 0) return -inf;
		return kth(x, y, k);
	}
	int next(int x, int y, int v) { //在区间[x, y]中查询v的后继，找不到返回inf
		int k = ask(x, y, v + 1) + 1;
		if (k > y - x + 1) return inf;
		return kth(x, y, k);
	}
	int count(int x, int y, int v) { //返回区间[x, y]中v的个数
		int l = 1, r = n;
		sum(x - 1, y);
		int p = rank(v);
		if (p > n || data[p] != v)
			return 0;
		while (l < r) {
			int m = (l + r) / 2;
			if (p <= m)
				left(), r = m;
			else
				right(), l = m + 1;
		}
		return value();
	}
	bool find(int x, int y, int v) {	//查询原数组的区间[x, y]中是否有元素v
		return count(x, y, v) >= 1;
	}
}tree;
const int maxn = 100000;
int A[maxn], B[maxn], n, q;
int main() {
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	scanf("%d %d", &n, &q);
	for (int i = 1; i <= n; ++i)
		scanf("%d", &A[i]);
	for (int i = 1; i <= n; i++)
		B[i] = i;
	tree.init(A, n, B, n);
	for (int i = 0; i < q; ++i) {
		int tp, L, R, v;
		scanf("%d %d %d %d", &tp, &L, &R, &v);
		if (tp == 0) {
			printf("%d\n", tree.pre(L, R, v));
		}
		else if (tp == 1) {
			printf("%d\n", tree.next(L, R, v));
		}
		else if (tp == 2) {
			printf("%d\n", tree.kth(L, R, v));
		}
		else if (tp == 3) {
			printf("%d\n", tree.ask(L, R, v));
		}
		else if (tp == 4) {
			printf("%d\n", tree.count(L, R, v));
		}
		else if (tp == 5) {
			bool result = tree.find(L, R, v);
			printf("%d\n", (int)result);
		}
		else {
			tree.set(L, v);
		}
	}
	return 0;
}


