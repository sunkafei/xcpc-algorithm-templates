#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 2010000;
int par[maxn], len[maxn], cur, root, last; //par[x]是后缀树上x的父结点
int ch[maxn][26]; //ch[x][w]表示结点x在后缀树中沿着字符w走到达的结点。
bool issuf[maxn]; //issuf[x]记录结点x是否表示一个后缀，若是则pos[x]就是后缀的下标。
int pos[maxn]; //pos[x]表示结点x对应的字符串在原串中的起始下标。
int node[maxn]; //node[i]表示下标i开始的后缀在树中的结点编号。
int newstate(int length, int index) {
	len[cur] = length;
	pos[cur] = index;
	return cur++;
}
void init() {
	memset(ch, 0, sizeof(ch));
	cur = 1;
	root = last = newstate(0, -1);
}
void extend(int w, int index) {
	int x = last;
	int nx = newstate(len[x] + 1, index);
	node[index] = nx;
	issuf[nx] = true;
	while (x && !ch[x][w])
		ch[x][w] = nx, x = par[x];
	if (!x)
		par[nx] = root;
	else {
		int y = ch[x][w];
		if (len[x] + 1 == len[y])
			par[nx] = y;
		else {
			int ny = newstate(len[x] + 1, pos[y]);
			memcpy(ch[ny], ch[y], sizeof(ch[y]));
			par[ny] = par[y];
			par[nx] = par[y] = ny;
			while (x && ch[x][w] == y)
				ch[x][w] = ny, x = par[x];
		}
	}
	last = nx;
}
void build(const char* s) {
	int n = strlen(s);
	for (int i = n - 1; i >= 0; --i)
		extend(s[i] - 'a', i);
	memset(ch, 0, sizeof(ch[0]) * (cur + 10));
	for (int i = 2; i < cur; ++i) {
		int w = s[pos[i] + len[par[i]]] - 'a';
		ch[par[i]][w] = i;
	}
}

const int maxlog = 23;
int anc[maxn][maxlog]; //anc[i][j]表示结点i往上走2^j个点到达的点
void preprocess() { //后缀树上倍增初始化
	for (int i = 1; i < cur; ++i) {
		anc[i][0] = par[i];
		for (int j = 1; (1 << (j - 1)) < cur; ++j)
			anc[i][j] = 0;
	}
	for (int j = 1; (1 << j) < cur; ++j) {
		for (int i = 1; i < cur; ++i) {
			if (anc[i][j - 1] != 0) {
				int a = anc[i][j - 1];
				anc[i][j] = anc[a][j - 1];
			}
		}
	}
}
int query(int index, int length) { //从下标index的后缀往上走到len[p] >= length的深度最小的结点
	int p = node[index];		   //若要识别字符串的子串[L, R]，则调用query(L, R-L+1)
	for (int i = maxlog - 1; i >= 0; --i)
		if (len[anc[p][i]] >= length)
			p = anc[p][i];
	return p;
}

int main() {

	return 0;
}