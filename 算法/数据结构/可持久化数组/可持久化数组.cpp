#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#define lc t[p].lchild
#define rc t[p].rchild
template<typename T, int begin, int end, int factor = 30>
struct Array {
	static const int size = end - begin + 1;
	struct segment {
		int lchild, rchild;
		T val;
	}t[size * factor];
	int sz;
	void ins(int& p, int x, int y, T A[]) {
		p = ++sz;
		if (x == y) 
			t[p].val = A[x];
		else {
			int mid = (x + y) >> 1;
			ins(lc, x, mid, A);
			ins(rc, mid + 1, y, A);
		}
	}
	//将数组A中的值作为初始值，并返回初始数组的版本号
	int init(T A[]) {
		int root = sz = 0;
		ins(root, begin, end, A);
		return root;
	}
	//将val作为可持久化数组的初始值，并返回初始数组的版本号（0）
	//采用此初始化方案时，数组的区间范围可以很大，
	//且可以在get函数中加入p==0就直接返回t[0].val的优化
	int init(T val) {
		t[0].val = val;
		return 0;
	}
	//获取版本号p的下标为index的值
	T get(int p, int index, int x = begin, int y = end) {
		if (x == y)
			return t[p].val;
		int mid = (x + y) >> 1;
		if (index <= mid)
			return get(lc, index, x, mid);
		else
			return get(rc, index, mid + 1, y);
	}
	//修改版本号为p的数组的下标index位置的值为v
	//使用方法为：root[i] = root[i-1]; set(root[i], idx, v);
	void set(int &p, int index, const T& v, int x = begin, int y = end) {
		t[++sz] = t[p]; p = sz; //如果类型T的复制开销较大，此处可以考虑只复制左右子树的编号
		if (x == y) 
			t[p].val = v;
		else {
			int mid = (x + y) >> 1;
			if (index <= mid)
				set(lc, index, v, x, mid);
			else
				set(rc, index, v, mid + 1, y);
		}
	}
};

const int maxn = 1000;
Array<int, 0, maxn, 100> arr;
int A[maxn];
int main() {
	using namespace std;
	for (int i = 0; i < maxn; ++i)
		A[i] = -1;
	int p = arr.init(-1);
	for (int i = 0; i < maxn; ++i)
		if (arr.get(p, i) != A[i])
			abort();
	vector<int> total[maxn];
	vector<int> root; root.push_back(p);
	total[0] = vector<int>(A, A + maxn);
	int T = 10000;
	while (T--) {
		int tp = rand() % 10;
		int pos = rand() % maxn, nd = rand() % root.size();
		int value = rand();
		if (tp != 0) {
			if (arr.get(root[nd], pos) != total[nd][pos]) {
				abort();
			}
		}
		else {
			int sz = root.size();
			total[sz] = total[nd];
			total[sz][pos] = value;
			root.push_back(root[nd]);
			arr.set(root.back(), pos, value);
		}
	}
	return 0;
}