#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
using namespace std;
template<typename T, int maxsize> struct ModifiablePriorityQueue { // 大根堆
	int data[maxsize * 2];
	int pos[maxsize * 2];
	T value[maxsize * 2];
	int sz;
	ModifiablePriorityQueue() : pos(), sz(0) {}
	void up(int i) { // 值变大
		auto index = data[i];
		auto val = value[index];
		while (i > 1) {
			int fa = i / 2;
			if (val > value[data[fa]]) {
				data[i] = data[fa];
				pos[data[i]] = i;
				i = fa;
			}
			else {
				break;
			}
		}
		data[i] = index;
		pos[index] = i;
	}
	void down(int i) { // 值变小
		auto index = data[i];
		auto val = value[index];
		while (i * 2 <= sz) {
			int child = i * 2;
			if (i * 2 + 1 <= sz && value[data[i * 2 + 1]] > value[data[child]]) {
				child = i * 2 + 1;
			}
			if (val < value[data[child]]) {
				data[i] = data[child];
				pos[data[i]] = i;
				i = child;
			}
			else {
				break;
			}
		}
		data[i] = index;
		pos[index] = i;
	}
	void push(int index, const T& val) {
		sz += 1;
		data[sz] = index;
		value[index] = val;
		up(sz);
	}
	void pop() {
		pos[data[1]] = 0;
		data[1] = data[sz];
		sz -= 1;
		if (sz > 0) {
			down(1);
		}
	}
	void erase(int index) {
		index = pos[index];
		pos[data[index]] = 0;
		data[index] = data[sz];
		sz -= 1;
		if (index <= sz) {
			down(index);
		}
	}
	void modify(int index, const T& val) {
		if (pos[index] == 0) {
			push(index, val);
		}
		else if (val > value[index]) {
			value[index] = val;
			up(pos[index]);
		}
		else {
			value[index] = val;
			down(pos[index]);
		}
	}
	int top() const {
		return data[1];
	}
	T get(int index) const {
		return value[index];
	}
	T maximum() const {
		return value[data[1]];
	}
	bool contains(int index) const {
		return pos[index] != 0;
	}
	int size() const {
		return sz;
	}
	bool empty() const {
		return sz == 0;
	}
};
int main() { // 洛谷 P4779
	static int d[210000];
	static vector<pair<int, int>> G[210000];
	int n, m, s;
	scanf("%d %d %d", &n, &m, &s);
	for (int i = 0; i < m; ++i) {
		int x, y, z;
		scanf("%d %d %d", &x, &y, &z);
		G[x].emplace_back(y, z);
	}
	static ModifiablePriorityQueue<int, 210000> Q;
	memset(d, 0x3f, sizeof(d));
	d[s] = 0;
	Q.push(s, 0);
	while (!Q.empty()) {
		auto x = Q.top(); Q.pop();
		for (auto [y, w] : G[x]) {
			if (d[y] > d[x] + w) {
				d[y] = d[x] + w;
				Q.modify(y, -d[y]);
			}
		}
	}
	for (int i = 1; i <= n; ++i) {
		printf("%d ", d[i] == 0x3f3f3f3f ? INT_MAX : d[i]);
	}
	printf("\n");
	return 0;
}
