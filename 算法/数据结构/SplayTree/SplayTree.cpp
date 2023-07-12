#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <tuple>
using namespace std;
const int maxn = 110000;
//若要修改一个点的点权，应当先将其splay到根，然后修改，最后还要调用pushup维护。
//调用完splay之后根结点会改变，应该用splay的返回值更新根结点。
namespace splay_tree {
	int ch[maxn][2], fa[maxn], stk[maxn], rev[maxn], sz[maxn], key[maxn], cur;
	void init() {
		cur = 0;
	}
	int newnode(int val) {
		int x = ++cur;
		ch[x][0] = ch[x][1] = fa[x] = rev[x] = 0;
		sz[x] = 1;
		key[x] = val;
		return x;
	}
	inline bool son(int x) {
		return ch[fa[x]][1] == x;
	}
	inline void pushup(int x) {
		sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + 1;
	}
	inline void pushdown(int x) {
		if (rev[x]) {
			rev[x] = 0;
			swap(ch[x][0], ch[x][1]);
			rev[ch[x][0]] ^= 1;
			rev[ch[x][1]] ^= 1;
		}
	}
	void rotate(int x) {
		int y = fa[x], z = fa[y], c = son(x);
		if (fa[y])
			ch[z][son(y)] = x;
		fa[x] = z;
		ch[y][c] = ch[x][!c];
		fa[ch[y][c]] = y;
		ch[x][!c] = y;
		fa[y] = x;
		pushup(y);
	}
	void ascend(int x) {
		for (int y = fa[x]; y; rotate(x), y = fa[x]) if (fa[y])
			son(x) ^ son(y) ? rotate(x) : rotate(y);
		pushup(x);
	}
	int splay(int x) { //没有pushdown操作时，可以直接用ascend替换splay
		int top = 0;
		for (int i = x; i; i = fa[i])
			stk[++top] = i;
		while (top)
			pushdown(stk[top--]);
		ascend(x);
		return x;
	}
	int splay(int x, int k) { //将以x为根的子树中的第k个结点旋转到根结点
		while (pushdown(x), k != sz[ch[x][0]] + 1) {
			if (k <= sz[ch[x][0]])
				x = ch[x][0];
			else
				k -= sz[ch[x][0]] + 1, x = ch[x][1];
		}
		if (x) ascend(x);
		return x;
	}
	template<typename ...T> int merge(int x, int y, T... args) {
		if constexpr (sizeof...(args) == 0) {
			if (x == 0) return y; //swap(x, y);
			x = splay(x, sz[x]);
			ch[x][1] = y; fa[y] = x;
			pushup(x);
			return x;
		}
		else {
			return merge(merge(x, y), args...);
		}
	}
	auto split(int x, int pos) { //分成两个区间[1, pos - 1]和[pos, n]
		if (pos == sz[x] + 1)
			return make_pair(x, 0);
		x = splay(x, pos);
		int y = ch[x][0];
		fa[y] = ch[x][0] = 0;
		pushup(x);
		return make_pair(y, x);
	}
	auto extract(int x, int L, int R) {
		auto [left, y] = split(x, L);
		auto [mid, right] = split(y, R - L + 2);
		return make_tuple(left, mid, right);
	}
	void traverse(int x) {
		if (x != 0) {
			traverse(ch[x][0]);
			printf("%d ", key[x]);
			//printf("%d (left: %d, right: %d) sz(%d) key(%d)\n", x, ch[x][0], ch[x][1], sz[x], key[x]);
			traverse(ch[x][1]);
		}
	}
}

using namespace splay_tree;
int main() {
	init();
	int nd[50], root = 0;
	for (int i = 1; i <= 10; ++i) {
		nd[i] = newnode(i);
		root = merge(root, nd[i]);
	}
	traverse(get<1>(extract(root, 3, 10))); printf("\n");
	return 0;
}