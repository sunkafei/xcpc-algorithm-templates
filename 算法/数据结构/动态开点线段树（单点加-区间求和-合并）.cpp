#include <cstdio>
#include <cstring>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;
const int inf = 1 << 30;
const int maxn = 110000;
struct TreeSet {
	struct node {
		int l, r, v;
	} T[maxn * 25];
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
	int merge(int x, int y, int l, int r) {	//将集合x与集合y合并成一个新的集合后返回
		if (!x) return y;					//合并之后x、y均会失效
		if (!y) return x;
		if (l == r) 
			T[x].v += T[y].v;
		else {
			int m = (l + r) >> 1;
			T[x].l = merge(T[x].l, T[y].l, l, m);
			T[x].r = merge(T[x].r, T[y].r, m + 1, r);
			T[x].v = T[T[x].l].v + T[T[x].r].v;
		}
		return x;
	}
	int merge(int x, int y) {
		return merge(x, y, left, right);
	}
}tree;
int A[210000];
int main() {
	default_random_engine e;
	uniform_int_distribution<int> d(0, 100000), v(1, 10);
	e.seed(334);
	int n = 100000;
	tree.init(0, n);
	int root = tree.newnode();
	for (int i = 0; i < 100000; ++i) {
		int tp = rand() % 2;
		int pos = d(e), value = v(e);
		if (tp == 0) {
			int tmp = tree.newnode();
			tree.insert(tmp, pos, value);
			root = tree.merge(root, tmp);
			A[pos] += value;
		}
		else {
			int ans = tree.sum(root, pos), res = accumulate(A, A + pos + 1, 0);
			if (ans != res) {
				printf("%d %d\n", ans, res);
				abort();
			}
		}
	}
	return 0;
}












