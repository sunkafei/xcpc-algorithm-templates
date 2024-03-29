/*
动态区间颜色数：
	把主席树第i个位置的权值设为这个数上一次出现的位置，然后查询区间[L, R]中的颜色数转化为
	查询区间[L, R]中比L小的值有多少个。（可以直接用ask函数查询）
	更快的方法：直接在主席树的第R个版本中查询比L小的数有多少个，然后减去(L - 1)即可。
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <utility>
using namespace std;
const int inf = 1 << 30;
struct ZXTree
{
	static const int maxn = 100010;
	struct node
	{
		int l, r, v; //左节点编号 右节点编号 值
	} T[maxn * 25];
	int n, sz, root[maxn], data[maxn]; //n是值域线段树的长度
	void ins(int &i, int l, int r, int p) //在区间[l, r]中插入p 
	{
		int m = (l + r) / 2;
		T[++sz] = T[i]; i = sz;
		T[i].v++; //递增当前区间内值的个数 
		if (l == r) return;
		if (p <= m) ins(T[i].l, l, m, p);
		else ins(T[i].r, m + 1, r, p);
	}
	int rank(int v)
	{
		//如果不需要数据离散化直接return v即可，同时还要将所有的data[v]改为v，此时主席树的值域为[1, n]。
		return lower_bound(data + 1, data + n + 1, v) - data;
	}
	void init(int *A, int length) //输入数组A的下标从1开始，length表示A的长度
	{
		root[0] = sz = 0;
		copy(A + 1, A + length + 1, data + 1);
		sort(data + 1, data + length + 1); //如果没有离散化需要对n进行赋值来指明主席树的值域。
		this->n = unique(data + 1, data + length + 1) - data - 1; //对data数组进行排序去重
		for (int i = 1; i <= length; ++i) //依次把原数组中每个位置的排名插入到可持久化线段树中
			ins(root[i] = root[i - 1], 1, n, rank(A[i])); //插入rank[i]，如果没有对A数组进行离散化，此处直接插入A[i]即可。 
														  //root[i]表示用A[1]到A[i]的所有值构建的权值线段树。
	}
	int kth(int x, int y, int k)  //查询原数组中区间[x, y]中的第k小的值，如果k大于区间长度会返回无效值！！！ 
	{
		int l = 1, r = n;
		x = root[x - 1], y = root[y];
		while (l < r)
		{
			int m = (l + r) / 2, t = T[T[y].l].v - T[T[x].l].v;
			if (k <= t)
				x = T[x].l, y = T[y].l, r = m;
			else
				x = T[x].r, y = T[y].r, l = m + 1, k -= t;
		}
		return data[r];
	}
	int ask(int x, int y, int v) //查询原数组中区间[x, y]中比v小的值的个数 
	{
		int l = 1, r = n, k = 0;
		x = root[x - 1], y = root[y];
		int p = rank(v) - 1;
		if (p <= 0) return 0;
		while (l < r)
		{
			int m = (l + r) / 2, t = T[T[y].l].v - T[T[x].l].v;
			if (p <= m)
				x = T[x].l, y = T[y].l, r = m;
			else
				x = T[x].r, y = T[y].r, l = m + 1, k += t;
		}
		k += T[y].v - T[x].v;
		return k;
	}
	int pre(int x, int y, int l, int r, int p)
	{
		int m = (l + r) / 2, v = T[y].v - T[x].v;
		if (l == r) return v > 0 ? data[r] : -inf;
		int t = T[T[y].r].v - T[T[x].r].v;
		if (p <= m || t == 0) return pre(T[x].l, T[y].l, l, m, p); //如果p在中点的左侧或者右子树为空，则直接在左子树查找 
		int k = pre(T[x].r, T[y].r, m + 1, r, p);
		if (k != -inf) return k;
		return pre(T[x].l, T[y].l, l, m, p);
	}
	int pre(int x, int y, int v) //在区间[x, y]中查询v的前驱，找不到返回-inf
	{
		int p = rank(v) - 1;
		if (p <= 0) return -inf;
		return pre(root[x - 1], root[y], 1, n, p);
	}
	int pre2(int x, int y, int v) //在区间[x, y]中查询v的前驱，找不到返回-inf
	{
		int k = ask(x, y, v);
		if (k == 0) return -inf;
		return kth(x, y, k);
	}
	int next(int x, int y, int l, int r, int p)
	{
		int m = (l + r) / 2, v = T[y].v - T[x].v;
		if (l == r) return v > 0 ? data[r] : inf;
		int t = T[T[y].l].v - T[T[x].l].v;
		if (p > m || t == 0) return next(T[x].r, T[y].r, m + 1, r, p);
		int k = next(T[x].l, T[y].l, l, m, p);
		if (k != inf) return k;
		return next(T[x].r, T[y].r, m + 1, r, p);
	}
	int next(int x, int y, int v) //在区间[x, y]中查询v的后继，找不到返回inf
	{
		int p = rank(v + 1);
		if (p > n) return inf;
		return next(root[x - 1], root[y], 1, n, p);
	}
	int next2(int x, int y, int v) //在区间[x, y]中查询v的后继，找不到返回inf
	{
		int k = ask(x, y, v + 1) + 1;
		if (k > y - x + 1) return inf;
		return kth(x, y, k);
	}
	int count(int x, int y, int v)	//返回区间[x, y]中v的个数
	{
		int l = 1, r = n;
		x = root[x - 1], y = root[y];
		int p = rank(v);
		if (p > n || data[p] != v)
			return 0;
		while (l < r && T[y].v - T[x].v > 0)
		{
			int m = (l + r) / 2;
			if (p <= m)
				x = T[x].l, y = T[y].l, r = m;
			else
				x = T[x].r, y = T[y].r, l = m + 1;
		}
		return T[y].v - T[x].v;
	}
	bool find(int x, int y, int v)	//查询原数组的区间[x, y]中是否有元素v
	{
		return count(x, y, v) >= 1;
	}
}zxtree;

const int maxn = 100000;
int A[maxn], n, q;
int main()
{
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	scanf("%d %d", &n, &q);
	for (int i = 1; i <= n; ++i)
		scanf("%d", &A[i]);
	zxtree.init(A, n);
	for (int i = 0; i < q; ++i)
	{
		int tp, L, R, v;
		scanf("%d %d %d %d", &tp, &L, &R, &v);
		if (tp == 0)
		{
			printf("%d\n", zxtree.pre(L, R, v));
		}
		else if (tp == 1)
		{
			printf("%d\n", zxtree.next(L, R, v));
		}
		else if (tp == 2)
		{
			printf("%d\n", zxtree.kth(L, R, v));
		}
		else if (tp == 3)
		{
			printf("%d\n", zxtree.ask(L, R, v));
		}
		else if (tp == 4)
		{
			printf("%d\n", zxtree.count(L, R, v));
		}
		else
		{
			bool result = zxtree.find(L, R, v);
			printf("%d\n", (int)result);
		}
	}
	return 0;
}















