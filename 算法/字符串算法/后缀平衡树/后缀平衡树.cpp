#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
const int maxn = 1110000;
const double alpha = 0.6;
struct SuffixBalancedTree {
	string data;
	double val[maxn], left, right;
	int sz[maxn], ch[maxn][2], root;
	int* pos, length, A[maxn];
	void init() {
		data.resize(1);
		root = 0;
	}
	void pushup(int x) {
		sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + 1;
	}
	void dfs(int x) {
		if (!x) return;
		dfs(ch[x][0]);
		A[++length] = x;
		dfs(ch[x][1]);
	}
	int build(int a, int b, double L, double R) {
		if (a > b) return 0;
		int mid = (a + b) >> 1;
		int x = A[mid];
		sz[x] = b - a + 1;
		val[x] = (L + R) / 2;
		ch[x][0] = build(a, mid - 1, L, val[x]);
		ch[x][1] = build(mid + 1, b, val[x], R);
		return x;
	}
	void add(int& x, double L, double R) {
		if (!x) {
			x = data.size() - 1;
			ch[x][0] = ch[x][1] = 0;
			val[x] = (L + R) / 2;
			sz[x] = 1;
			return;
		}
		int y = data.size() - 1;
		if (data[y] < data[x] || data[y] == data[x] && val[y - 1] < val[x - 1])
			add(ch[x][0], L, val[x]);
		else
			add(ch[x][1], val[x], R);
		pushup(x);
		if (sz[ch[x][0]] > sz[x] * alpha || sz[ch[x][1]] > sz[x] * alpha)
			pos = &x, left = L, right = R;
	}
	void push_front(char c) { //向开头添加一个新的字符之后会改变其他位置的下标
		data.push_back(c);
		pos = nullptr;
		add(root, 0, 1);
		if (pos) {
			length = 0;
			dfs(*pos);
			*pos = build(1, length, left, right);
		}
	}
	int merge(int x, int y) {
		if (!x || !y) 
			return x | y;
		if (sz[x] > sz[y]) {
			ch[x][1] = merge(ch[x][1], y);
			pushup(x);
			return x;
		}
		else {
			ch[y][0] = merge(x, ch[y][0]);
			pushup(y);
			return y;
		}
	}
	void del(int& x) {
		const int y = data.size() - 1;
		sz[x] -= 1;
		if (x == y)
			x = merge(ch[x][0], ch[x][1]);
		else if (val[y] < val[x])
			del(ch[x][0]);
		else
			del(ch[x][1]);
	}
	void pop_front() {
		del(root);
		data.pop_back();
	}
	double weight(int index) { //返回下标index的权值，权值越小，字典序越小（下标从1开始）
		return val[data.size() - index];
	}
	int rank(int index) { //返回下标index的排名（下标从1开始）
		double key = weight(index);
		int ret = 0, x = root;
		while (x) {
			if (key < val[x])
				x = ch[x][0];
			else
				ret += sz[ch[x][0]] + 1, x = ch[x][1];
		}
		return ret;
	}
	int rank(const char *s) { //返回字典序小于s的后缀的个数
		int ret = 0, x = root, n = strlen(s);
		while (x) {
			int L = min(x, n) + 1;
			int flag = 0;
			for (int i = 0; i < L; ++i) {
				if (s[i] != data[x - i]) {
					flag = s[i] - data[x - i];
					break;
				}
			}
			if (flag <= 0)
				x = ch[x][0];
			else
				ret += sz[ch[x][0]] + 1, x = ch[x][1];
		}
		return ret;
	}
}tree;
int main() {

	return 0;
}