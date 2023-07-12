/*
定义dif[x] = len[x] - len[fail[x]]，
slink[x]为x后缀链接路径上第一个dif[x]≠dif[fail[x]]的祖先。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 110000;
const int sigma_size = 26;
int info[maxn], ch[maxn][sigma_size], fail[maxn], len[maxn], dif[maxn], slink[maxn];
int node[maxn], last, cur, sz;
void init() {
	memset(ch, 0, sizeof(ch));
	fail[0] = 1;
	info[0] = -1;
	len[1] = -1;
	cur = last = 1;
	sz = 0;
}
void extend(int w) {
	int p = last;
	info[++sz] = w;
	while (info[sz - len[p] - 1] != w)
		p = fail[p];
	if (!ch[p][w]) {
		int u = ++cur, x = fail[p];
		while (info[sz - len[x] - 1] != w)
			x = fail[x];
		len[u] = len[p] + 2;
		fail[u] = ch[x][w]; //(*)
		ch[p][w] = u;
		dif[u] = len[u] - len[fail[u]];
		if (dif[u] != dif[fail[u]]) 
			slink[u] = fail[u];
		else 
			slink[u] = slink[fail[u]];
	}
	last = ch[p][w];
}
void insert(char* str) { //输入字符串下标从0开始
	int n = strlen(str);
	for (int i = 0; i < n; ++i) {
		extend(str[i] - 'a');
		node[i] = last;
	}
}
int main() {
	
	return 0;
}