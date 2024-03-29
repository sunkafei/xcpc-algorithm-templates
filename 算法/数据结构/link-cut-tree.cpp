#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 110000;
//若要修改一个点的点权，应当先将其splay到根，然后修改，最后还要调用pushup维护。
namespace lct {
	int ch[maxn][2], fa[maxn], stk[maxn], rev[maxn];
	void init() { //初始化link-cut-tree
		memset(ch, 0, sizeof(ch));
		memset(fa, 0, sizeof(fa));
		memset(rev, 0, sizeof(rev));
	}
	inline bool son(int x) { 
		return ch[fa[x]][1] == x; 
	}
	inline bool isroot(int x) { 
		return ch[fa[x]][1] != x && ch[fa[x]][0] != x; 
	}
	inline void reverse(int x) { //给结点x打上反转标记
		swap(ch[x][1], ch[x][0]);
		rev[x] ^= 1;
	}
	inline void pushup(int x) { }
	inline void pushdown(int x) {
		if (rev[x]) {
			reverse(ch[x][0]);
			reverse(ch[x][1]);
			rev[x] = 0;
		}
	}
	void rotate(int x) {
		int y = fa[x], z = fa[y], c = son(x);
		if (!isroot(y))
			ch[z][son(y)] = x;
		fa[x] = z;
		ch[y][c] = ch[x][!c]; 
		fa[ch[y][c]] = y;
		ch[x][!c] = y; 
		fa[y] = x; 
		pushup(y);
	}
	void splay(int x) {
		int top = 0;
		stk[++top] = x;
		for (int i = x; !isroot(i); i = fa[i])
			stk[++top] = fa[i];
		while (top)
			pushdown(stk[top--]);
		for (int y = fa[x]; !isroot(x); rotate(x), y = fa[x]) if (!isroot(y))
			son(x) ^ son(y) ? rotate(x) : rotate(y);
		pushup(x);
	}
	void access(int x) {
		for (int y = 0; x; y = x, x = fa[x]) {
			splay(x);
			ch[x][1] = y;
			pushup(x);
		}
	}
	void makeroot(int x) { //将x变为树的新的根结点
		access(x);
		splay(x); 
		reverse(x); 
	}
	int findroot(int x) { //返回x所在树的根结点
		access(x); 
		splay(x); 
		while (ch[x][0])
			pushdown(x), x = ch[x][0];
		return x; 
	}
	void split(int x, int y) { //提取出来y到x之间的路径，并将y作为根结点
		makeroot(x);
		access(y);
		splay(y); 
	}
	void cut(int x) { //断开结点x与它的父结点之间的边
		access(x);
		splay(x);
		ch[x][0] = fa[ch[x][0]] = 0;
		pushup(x);
	}
	void cut(int x, int y) { //切断x与y相连的边（必须保证x与y在一棵树中）
		makeroot(x); //将x置为整棵树的根
		cut(y); //删除y与其父结点之间的边
	}
	void link(int x, int y) { //连接x与y（必须保证x和y属于不同的树）
		makeroot(x);
		fa[x] = y;
	}
	bool sametree(int x, int y) { //判断结点x与y是否属于同一棵树
		makeroot(x);
		return findroot(y) == x;
	}
}
int main() {
	return 0;
}