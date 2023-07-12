#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>
#include <iterator>
using namespace std;
const int inf = 1 << 30;
static const int maxn = 210000; //maxn应当大于所有集合大小的总和
struct node {		//该算法实现的整数集合为可重集合（multiset）
	int l, r, v;	//左节点编号 右节点编号 值
} T[maxn * 25];
int len, sz, info[maxn]; //len是值域线段树的长度
int ranking(int v) {
	//如果不需要数据离散化直接return v即可，同时还要将所有的info[v]改为v，此时线段树的值域为[1, len]。
	return lower_bound(info + 1, info + len + 1, v) - info;
}
void ins(int& i, int l, int r, int p) { //在区间[l, r]中插入p 
	T[++sz] = T[i]; i = sz;
	T[i].v++; //递增当前区间内值的个数 
	if (l == r) return;
	int m = (l + r) / 2;
	if (p <= m) ins(T[i].l, l, m, p);
	else ins(T[i].r, m + 1, r, p);
}
int insert(int x, int v) { //向集合x中添加数值v，返回新的集合的编号，原来的集合不会改变
	ins(x, 1, len, ranking(v));
	return x;
}
void init(int* A, int length) { //输入数组A的下标从1开始，length表示A的长度
	sz = 0;						//A记录集合中所有可能出现的整数
	copy(A + 1, A + length + 1, info + 1);
	sort(info + 1, info + length + 1); //如果没有离散化需要对len进行赋值来指明线段树的值域。
	len = unique(info + 1, info + length + 1) - info - 1; //对info数组进行排序去重
}
int kth(int x, int k) { //查找集合x中的第k大元素
	int l = 1, r = len;
	while (l < r) {
		int m = (l + r) / 2, t = T[T[x].l].v;
		if (k <= t)
			x = T[x].l, r = m;
		else
			x = T[x].r, l = m + 1, k -= t;
	}
	if (k > T[x].v) //如果k大于集合x的大小，则返回inf
		return inf;
	return info[r];
}
int ask(int x, int v) { //返回集合x中比v小的数的个数
	int l = 1, r = len, k = 0;
	int p = ranking(v) - 1;
	if (p <= 0) return 0;
	while (l < r) {
		int m = (l + r) / 2, t = T[T[x].l].v;
		if (p <= m)
			x = T[x].l, r = m;
		else
			x = T[x].r, l = m + 1, k += t;
	}
	k += T[x].v;
	return k;
}
int pre(int x, int v) { //在集合x中查询v的前驱，找不到返回-inf
	int k = ask(x, v);
	if (k == 0) return -inf;
	return kth(x, k);
}
int next(int x, int v) { //在集合x中查询v的后继，找不到返回inf
	int k = ask(x, v + 1) + 1;
	return kth(x, k);
}
int count(int x, int v) { //返回集合x中数值v的个数
	int l = 1, r = len;
	int p = ranking(v);
	if (p > len || info[p] != v)
		return 0;
	while (l < r && T[x].v> 0) {
		int m = (l + r) / 2;
		if (p <= m)
			x = T[x].l, r = m;
		else
			x = T[x].r, l = m + 1;
	}
	return T[x].v;
}
bool find(int x, int v) { //返回集合x中是否存在v
	return count(x, v) >= 1;
}
int maximum(int x) { //返回集合x中的最大值
	int l = 1, r = len;
	if (T[x].v == 0) //如果集合为空则返回-inf
		return -inf;
	while (l < r) {
		int m = (l + r) / 2, t = T[T[x].r].v;
		if (t == 0)
			x = T[x].l, r = m;
		else
			x = T[x].r, l = m + 1;
	}
	return info[r];
}
int minimum(int x) { //返回集合x中的最小值
	int l = 1, r = len;
	if (T[x].v == 0) //如果集合为空则返回inf
		return inf;
	while (l < r) {
		int m = (l + r) / 2, t = T[T[x].l].v;
		if (t > 0)
			x = T[x].l, r = m;
		else
			x = T[x].r, l = m + 1;
	}
	return info[r];
}
int merge(int x, int y, int l = 1, int r = len) {	//将集合x与集合y合并成一个新的集合后返回
	if (!x) return y;								//不会改变x和y
	if (!y) return x;
	int i = ++sz;
	if (l == r) {
		T[i].v = T[x].v + T[y].v;
		T[i].l = T[i].r = 0;
	}
	else {
		int m = (l + r) / 2;
		T[i].l = merge(T[x].l, T[y].l, l, m);
		T[i].r = merge(T[x].r, T[y].r, m + 1, r);
		T[i].v = T[T[i].l].v + T[T[i].r].v;
	}
	return i;
}
int diff(int x, int y, int l = 1, int r = len) {	//返回集合x与集合y的差集（x - y），不会改变x和y
	int i = ++sz; T[i] = T[x];						//时间复杂度size(y)log(len)
	if (l == r) {
		T[i].v = max(0, T[x].v - T[y].v);
	}
	else {
		int m = (l + r) / 2;
		if (T[y].l)
			T[i].l = diff(T[x].l, T[y].l, l, m);
		if (T[y].r)
			T[i].r = diff(T[x].r, T[y].r, m + 1, r);
		T[i].v = T[T[i].l].v + T[T[i].r].v;
	}
	return i;
}
int intersect(int x, int y, int l = 1, int r = len) {	//返回集合x与集合y的交集（不会改变x和y）
	int i = ++sz;										//时间复杂度min{size(x),size(y)} * log(len)
	if (l == r) {
		T[i].v = min(T[x].v, T[y].v);
		T[i].l = T[i].r = 0;
	}
	else {
		int m = (l + r) / 2;
		T[i].l = (!T[x].l || !T[y].l ? 0 : intersect(T[x].l, T[y].l, l, m));
		T[i].r = (!T[x].r || !T[y].r ? 0 : intersect(T[x].r, T[y].r, m + 1, r));
		T[i].v = T[T[i].l].v + T[T[i].r].v;
	}
	return T[i].v == 0 ? 0 : i;
}
int symmetric(int x, int y, int l = 1, int r = len) {	//返回集合x与集合y的对称差分（不会改变x和y）						
	if (!x) return y;									//时间复杂度min{size(x),size(y)} * log(len)
	if (!y) return x;
	int i = ++sz;
	if (l == r) {
		T[i].v = abs(T[x].v - T[y].v);
		T[i].l = T[i].r = 0;
	}
	else {
		int m = (l + r) / 2;
		T[i].l = symmetric(T[x].l, T[y].l, l, m);
		T[i].r = symmetric(T[x].r, T[y].r, m + 1, r);
		T[i].v = T[T[i].l].v + T[T[i].r].v;
	}
	return i;
}
void left(vector<int>& X, vector<int>& Y) {
	for (auto& i : X)
		i = T[i].l;
	for (auto& i : Y)
		i = T[i].l;
}
void right(vector<int>& X, vector<int>& Y) {
	for (auto& i : X)
		i = T[i].r;
	for (auto& i : Y)
		i = T[i].r;
}
int left_value(vector<int>& X, vector<int>& Y) {
	int tot = 0;
	for (auto i : X)
		tot += T[T[i].l].v;
	for (auto i : Y)
		tot -= T[T[i].l].v;
	return tot;
}
int right_value(vector<int>& X, vector<int>& Y) {
	int tot = 0;
	for (auto i : X)
		tot += T[T[i].r].v;
	for (auto i : Y)
		tot -= T[T[i].r].v;
	return tot;
}
int value(vector<int>& X, vector<int>& Y) {
	int tot = 0;
	for (auto i : X)
		tot += T[i].v;
	for (auto i : Y)
		tot -= T[i].v;
	return tot;
}
int kth(vector<int> X, vector<int> Y, int k) {	//计算X中集合的并集 减去 Y中集合的并集 所得集合的第k大
	int l = 1, r = len;							//必须保证Y的并集 是 X的并集 的子集
	while (l < r) {
		int m = (l + r) / 2, t = left_value(X, Y);
		if (k <= t)
			left(X, Y), r = m;
		else
			right(X, Y), l = m + 1, k -= t;
	}
	if (k > value(X, Y))
		return inf;
	return info[r];
}
int ask(vector<int> X, vector<int> Y, int v) {	//计算X中集合的并集 减去 Y中集合的并集 所得集合中比v小的数的个数
	int l = 1, r = len, k = 0;					//必须保证Y的并集 是 X的并集 的子集
	int p = ranking(v) - 1;
	if (p <= 0) return 0;
	while (l < r) {
		int m = (l + r) / 2;
		if (p <= m)
			left(X, Y), r = m;
		else {
			k += left_value(X, Y);
			right(X, Y);
			l = m + 1;
		}
	}
	k += value(X, Y);
	return k;
}
int pre(vector<int> X, vector<int> Y, int v) {	//计算X中集合的并集 减去 Y中集合的并集 所得集合中v的前驱
	int k = ask(X, Y, v);						//必须保证Y的并集 是 X的并集 的子集
	if (k == 0) return -inf; //找不到返回-inf
	return kth(X, Y, k);
}
int next(vector<int> X, vector<int> Y, int v) {	//计算X中集合的并集 减去 Y中集合的并集 所得集合中v的后继
	int k = ask(X, Y, v + 1) + 1;				//必须保证Y的并集 是 X的并集 的子集
	return kth(X, Y, k); //找不到返回inf
}
int count(vector<int> X, vector<int> Y, int v) {	//计算X中集合的并集 减去 Y中集合的并集 所得集合中v的个数
	int l = 1, r = len;								//必须保证Y的并集 是 X的并集 的子集
	int p = ranking(v);
	if (p > len || info[p] != v)
		return 0;
	while (l < r) {
		int m = (l + r) / 2;
		if (p <= m)
			left(X, Y), r = m;
		else
			right(X, Y), l = m + 1;
	}
	return value(X, Y);
}
int maximum(vector<int> X, vector<int> Y) {	//计算X中集合的并集 减去 Y中集合的并集 所得集合中的最大值
	int l = 1, r = len;						//必须保证Y的并集 是 X的并集 的子集
	if (value(X, Y) == 0) //如果集合为空则返回-inf
		return -inf;
	while (l < r) {
		int m = (l + r) / 2, t = right_value(X, Y);
		if (t == 0)
			left(X, Y), r = m;
		else
			right(X, Y), l = m + 1;
	}
	return info[r];
}
int minimum(vector<int> X, vector<int> Y) {	//计算X中集合的并集 减去 Y中集合的并集 所得集合中的最小值
	int l = 1, r = len;						//必须保证Y的并集 是 X的并集 的子集
	if (value(X, Y) == 0) //如果集合为空则返回inf
		return inf;
	while (l < r) {
		int m = (l + r) / 2, t = left_value(X, Y);
		if (t > 0)
			left(X, Y), r = m;
		else
			right(X, Y), l = m + 1;
	}
	return info[r];
}

#define VALUE_TYPE 5000
int tmp[maxn], root[maxn];
vector<int> A[maxn];
int main() {
	int n = 10, q = 400000;
	default_random_engine e;
	uniform_int_distribution<int> d(1, VALUE_TYPE);
	uniform_int_distribution<int> big(-100000000, 100000000);
	//e.seed(time(0));
	for (int i = 1; i <= VALUE_TYPE; ++i)
		tmp[i] = big(e);
	init(tmp, VALUE_TYPE);
	int rt = 0;
	vector<int> vec;
	for (int i = 1; i <= 100000; ++i) {
		int val = d(e); val = tmp[val];
		if (rand() % 2)
			root[0] = insert(root[0], val), A[0].push_back(val);
		else
			root[1] = insert(root[1], val), A[1].push_back(val);
	}
	rt = symmetric(root[0], root[1]);
	sort(A[0].begin(), A[0].end());
	sort(A[1].begin(), A[1].end());
	std::set_symmetric_difference(A[0].begin(), A[0].end(), A[1].begin(), A[1].end(), back_inserter(vec));
	//rt = root[1]; vec = A[1];
	vec.push_back(-inf); vec.push_back(inf);
	sort(vec.begin(), vec.end());
	printf("%d %d\n", vec.size(), T[rt].v);
	for (int q = 0; q < 100000; ++q) {
		if (q % 1000 == 0) printf("%d\n", q);
		int tp = rand() % 8, val = d(e);
		val = tmp[val];
		if (tp == 0) {
			rt = insert(rt, val);
			vec.push_back(val);
			sort(vec.begin(), vec.end());
		}
		else if (tp == 1) {
			int k = rand() % T[rt].v + 1;
			if (vec[k] != kth(rt, k))
				abort();
		}
		else if (tp == 2) {
			if (lower_bound(vec.begin(), vec.end(), val) - vec.begin() - 1 != ask(rt, val))
				abort();
		}
		else if (tp == 3) {
			int p = lower_bound(vec.begin(), vec.end(), val) - vec.begin() - 1;
			if (vec[p] != pre(rt, val))
				abort();
		}
		else if (tp == 4) {
			int p = upper_bound(vec.begin(), vec.end(), val) - vec.begin();
			if (vec[p] != next(rt, val))
				abort();
		}
		else if (tp == 5) {
			if (count(rt, val) != count(vec.begin(), vec.end(), val))
				abort();
		}
		else if (tp == 6) {
			if (*-- --vec.end() != maximum(rt))
				abort();
		}
		else if (tp == 7) {
			if (*++vec.begin() != minimum(rt))
				abort();
		}
	}
	return 0;
}















