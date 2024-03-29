/*
1.	len[i]表示编号为i的节点对应的回文串的长度（一个节点表示一个回文串）。
2.	fail[i]表示结点i失配以后跳转到的最长后缀回文串对应的结点。
3.	par[i]表示结点i删除掉最外层的一个字符后得到的回文串对应的结点。
	例如结点i表示的回文串为cabbac，则结点par[i]对应的回文串为abba。
4.	node[i]表示输入字符串中以下标i结尾的最长回文串对应的回文树结点。
5.	cnt[i]表示结点i对应的字符串出现的次数（建树时求出的不是完全的，最后calc()函数跑一遍以后才是正确的）。
6.	num[i]表示 以结点i表示的最长回文串的最右端点 为回文串结尾的回文串个数。
7.	若要判断回文自动机中是否包含回文串P[0, m - 1]，则根据P的长度是奇数还是偶数来选择根结点（长度为偶数的回文串以0为根结点，长度为奇数的回文串以1为根结点），
	然后将字符串P[m / 2, m - 1]输入自动机，到达了结点node，自动机包含回文串P当且仅当len[node] == m。
8.	一般来说不需要对回文自动机进行dfs，因为循环for (int i = 2; i <= cur; ++i)就可以按照fail树的拓扑序
	（也是par树的拓扑序）访问结点。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 110000;
const int sigma_size = 26;
int info[maxn], ch[maxn][sigma_size], fail[maxn], len[maxn];
int cnt[maxn], num[maxn], par[maxn], node[maxn], last, cur, sz;
void init() {
	memset(ch, 0, sizeof(ch));
	memset(cnt, 0, sizeof(cnt));
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
		par[u] = p;
		len[u] = len[p] + 2;
		fail[u] = ch[x][w]; //(*)
		ch[p][w] = u;
		num[u] = num[fail[u]] + 1;
	}
	last = ch[p][w];
	cnt[last]++;
}
void insert(char* str) { //输入字符串下标从0开始
	int n = strlen(str);
	for (int i = 0; i < n; ++i) {
		extend(str[i] - 'a');
		node[i] = last;
	}
}
void calc() {
	//更新fail的地方只有(*)处，此时将新建的结点u连接到之前的结点上而不改变之前的结点的连接状态，
	//这样，对于从cur到2的循环，就是对fail树按拓扑序逆序循环（也是对par树按拓扑序逆序循环）。
	for (int i = cur; i >= 2; --i)
		cnt[fail[i]] += cnt[i];
}
int main() {
	int A[] = { 1, 2, 3, 2, 1, 3, 3 }, B[] = { 3, 2, 1 };
	init();
	for (int i = 0; i < sizeof(A) / sizeof(*A); ++i)
		extend(A[i]);
	int st = 0; //0偶数，1奇数
	for (int i = 0; i < sizeof(B) / sizeof(*B); ++i)
		st = ch[st][B[i]];
	printf("%d\n", len[st]);
	return 0;
}