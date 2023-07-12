/*
在每次调用extend函数构建后缀自动机的时候，extend函数会logn次调用solve函数，
这些次调用的[L, R]构成了连续区间[1, index]。solve函数的参数意思是对于以下标index结尾的字符串，
长度区间从[L, R]的这些子串上一次出现的位置都是x（x == -1表示之前没有出现过）。
可以通过在LCT上多维护一个标记，来维护上上次出现的位置。
对于询问母串中一个区间子串的问题，可以考虑用这个模型来解决。每次extend(w, index)的时候，
处理所有下标以index结尾的询问，并且动态维护左端点的询问值。
当前以index结尾的询问应该以index - 1的询问为基础，通过solve函数更新产生。
solve函数每次会对一个左端点区间的询问值进行更新，可以考虑用线段树维护。
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
const int maxn = 210000; //maxn应当大于最大字符串长度的2倍
int par[maxn], len[maxn], go[maxn][26]; 
int ch[maxn][2], fa[maxn], stk[maxn], pos[maxn];
int cur, root, last;
long long ans = 0, base = 0;
void solve(int L, int R, int x, int index) {
	//当前算法对于一个字符串中的所有子串 求出了其本质不同的子串个数 的和。
	if (x == -1) {
		int t = R - L + 1;
		base += t * (t + 1) / 2;
	}
	else {
		base += (index - x) * (R - L + 1);
	}
}
inline bool son(int x) {
	return ch[fa[x]][1] == x;
}
inline bool isroot(int x) {
	return ch[fa[x]][1] != x && ch[fa[x]][0] != x;
}
inline void pushdown(int x) {
	pos[ch[x][0]] = pos[ch[x][1]] = pos[x];
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
}
void access(int nd, int index) {
	for (int y = nd, x = fa[nd]; y > 1; y = x, x = fa[x]) {
		if (x) {
			splay(x);
			ch[x][1] = y;
		}
		solve(len[x] + 1, len[y], pos[y], index);
	}
}
void cut(int x) { //断开结点x与它的父结点之间的边
	splay(x);
	fa[ch[x][0]] = fa[x];
	fa[x] = ch[x][0] = 0;
}
int newstate(int length) {
	len[cur] = length;
	return cur++;
}
void init() {
	memset(ch, 0, sizeof(ch));
	memset(fa, 0, sizeof(fa));
	memset(go, 0, sizeof(go));
	memset(pos, -1, sizeof(pos));
	cur = 1;
	root = last = newstate(0);
}
void extend(int w, int index) {
	int x = last;
	int nx = newstate(len[x] + 1);
	while (x && !go[x][w])
		go[x][w] = nx, x = par[x];
	if (!x)
		par[nx] = root, fa[nx] = root;
	else {
		int y = go[x][w];
		if (len[x] + 1 == len[y])
			par[nx] = y, fa[nx] = y;
		else {
			int ny = newstate(len[x] + 1);
			memcpy(go[ny], go[y], sizeof(go[y]));
			fa[ny] = par[y], par[ny] = par[y];
			cut(y); 
			pos[ny] = pos[y];
			fa[y] = ny, par[y] = ny;
			fa[nx] = ny, par[nx] = ny;
			while (x && go[x][w] == y)
				go[x][w] = ny, x = par[x];
		}
	}
	last = nx;
	access(last, index);
	splay(last);
	pos[last] = index;
}

char A[maxn];
int main() {
	freopen("in.txt", "r", stdin);
	init();
	scanf("%s", A);
	int n = strlen(A);
	for (int i = 0; i < n; ++i) {
		extend(A[i] - 'A', i);
		ans += base;
	}
	printf("ans: %lld\n", ans);
	return 0;
}