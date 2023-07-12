#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;
const int maxn = 310000; //maxn至少为结点数 + 操作数的两倍
namespace ett {
	//--------------------------Splay Tree------------------------------
	//若要修改一个点的点权，应当先将其splay到根，然后修改，最后还要调用pushup维护。
	//调用完splay之后根结点会改变，应该用splay的返回值更新根结点。
	int ch[maxn][2], fa[maxn], stk[maxn], sum[maxn], key[maxn], cur;
	int alloc(int val) {
		int x = ++cur;
		ch[x][0] = ch[x][1] = fa[x] = 0;
		sum[x] = key[x] = val;
		return x;
	}
	inline bool son(int x) {
		return ch[fa[x]][1] == x;
	}
	inline void pushup(int x) {
		sum[x] = sum[ch[x][0]] + sum[ch[x][1]] + key[x];
	}
	inline void pushdown(int x) {}
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
	template<typename ...T> int merge(int x, int y, T... args) {
		if constexpr (sizeof...(args) == 0) {
			if (x == 0) return y;
			while (pushdown(x), ch[x][1])
				x = ch[x][1];
			ascend(x);
			ch[x][1] = y; fa[y] = x;
			pushup(x);
			return x;
		}
		else {
			return merge(merge(x, y), args...);
		}
	}
	inline int split(int x, int d) {
		int y = ch[x][d];
		fa[y] = ch[x][d] = 0;
		pushup(x);
		return y;
	}
	//--------------------------Euler Tour Tree----------------------------
	int idx;
	unordered_map<long long, int> locate;
	#define edge(x, y) (((1LL * (x)) << 30) + (y))
	void init() {
		cur = idx = 0;
		locate.clear();
	}
	inline int newnode(int val) {
		int x = ++idx;
		locate[edge(x, x)] = alloc(val);
		return x;
	}
	inline int newedge(int x, int y) {
		return locate[edge(x, y)] = alloc(0);
	}
	inline int changeroot(int x) {
		int a = locate[edge(x, x)];
		splay(a);
		int b = split(a, 0);
		return merge(a, b);
	}
	inline int gettree(int x) {
		return splay(locate[edge(x, x)]);
	}
	inline bool sametree(int x, int y) {
		if (x == y) return true;
		auto a = locate[edge(x, x)];
		auto b = locate[edge(y, y)];
		splay(a);
		splay(b);
		return fa[a] != 0;
	}
	void link(int x, int y) {
		auto a = newedge(x, y);
		auto b = newedge(y, x);
		merge(changeroot(x), a, changeroot(y), b);
	}
	void cut(int x, int y) {
		auto a = locate[edge(x, y)];
		auto b = locate[edge(y, x)];
		splay(a);
		auto i = split(a, 0), j = split(a, 1);
		splay(b);
		bool flag = (i != 0 && fa[i] != 0) || b == i;
		auto L = split(b, 0), R = split(b, 1);
		if (flag)
			merge(L, j);
		else 
			merge(i, R);
	}
}
using namespace ett;
int node[maxn];
//访问结点node[x]所在树的权值和：sum[gettree(node[x])]
//修改结点node[x]的权值为v：int a = gettree(node[x]); key[a] = v; pushup(a);
int main() { //洛谷 P4219
	//freopen("in.txt", "r", stdin);
	int n, q;
	scanf("%d %d", &n, &q);
	init();
	for (int i = 1; i <= n; ++i)
		node[i] = newnode(1);
	for (int i = 0; i < q; ++i) {
		char tp;
		int x, y;
		scanf(" %c %d %d", &tp, &x, &y);
		if (tp == 'A') {
			link(node[x], node[y]);
		}
		else {
			cut(node[x], node[y]);
			long long a = sum[gettree(node[x])];
			long long b = sum[gettree(node[y])];
			printf("%lld\n", a * b);
			link(node[x], node[y]);
		}
	}
	return 0;
}