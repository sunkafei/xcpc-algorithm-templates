/*
每次插入时间复杂度都为O(1)的回文自动机（不基于势能分析），可用于回滚莫队。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
const int maxn = 300007;
struct Palindromic_Tree {
	int cur, cnt[maxn], fail[maxn], ch[maxn][26], len[maxn], quick[maxn][26];
	int last[2], info[maxn * 2], L, R;
	void init() {
		fail[0] = cur = 1;
		len[1] = -1;
		last[0] = last[1] = 0;
		L = maxn;
		R = L - 1;
		memset(ch, 0, sizeof(ch)); //多组数据时应当在新建结点的时候清空
		memset(cnt, 0, sizeof(cnt));
		memset(info, -1, sizeof(info));
		for (int i = 0; i < 26; i++)
			quick[1][i] = quick[0][i] = 1;
	}
	void extend(int w, int back = 1) { //back为1则插入w到字符串的结尾，为0则插入到开头。
		int tp = back ? -1 : 1;
		int x = back ? ++R : --L;
		int p = last[back];
		info[x] = w;
		if (info[x + tp * (len[p] + 1)] != info[x])
			p = quick[p][w];
		if (!ch[p][w]) {
			int u = ++cur;
			len[u] = len[p] + 2;
			int now = fail[p];
			if (info[x + tp * (len[now] + 1)] != info[x])
				now = quick[now][w];
			fail[u] = ch[now][w];
			memcpy(quick[u], quick[fail[u]], sizeof(quick[u]));
			quick[u][info[x + tp * len[fail[u]]]] = fail[u];
			ch[p][w] = u;
		}
		last[back] = ch[p][w];
		if (len[last[back]] == R - L + 1)
			last[back ^ 1] = last[back];//注意这里 
		cnt[last[back]]++;
	}
} tree;
namespace Palindromic_Tree_Back {
	int info[maxn], ch[maxn][26], quick[maxn][26], fail[maxn], len[maxn];
	int last, cur, sz, n, m;
	vector<int> node;
	void init() {
		node.clear();
		memset(ch, 0, sizeof(ch[0]) * 5);
		fail[0] = 1;
		info[0] = len[1] = -1;
		cur = last = 1;
		sz = 0;
		for (int i = 0; i < 26; ++i)
			quick[1][i] = quick[0][i] = 1;
	}
	void extend(int w) { // 0 <= w <= sigma_size
		int p = last;
		info[++sz] = w;
		if (info[sz - len[p] - 1] != w)
			p = quick[p][w];
		if (!ch[p][w]) {
			int u = ++cur, x = fail[p];
			memset(ch[u], 0, sizeof(ch[u]));
			if (info[sz - len[x] - 1] != w)
				x = quick[x][w];
			len[u] = len[p] + 2;
			fail[u] = ch[x][w];
			ch[p][w] = u;
			memcpy(quick[u], quick[fail[u]], sizeof(quick[u]));
			quick[u][info[sz - len[fail[u]]]] = fail[u];
			//value[u] = (value[fail[u]] + (n - len[u] + 1) * pow_mod(26, n - len[u])) % mod;
		}
		last = ch[p][w];
		//answer = (answer + value[last]) % mod;
	}
	void rollback() { //回滚
		int x = node.back();
		node.pop_back();
		last = node.back();
		sz -= 1;
		//answer = (answer - value[x] + mod) % mod;
	}
}

char A[maxn];
int c[maxn], id[maxn];
int main() {
	scanf("%s", A + 1);
	int L = strlen(A + 1);
	tree.init();
	long long ans = 0;
	int T = L >> 1;
	for (int i = T + 1; i <= L; i++) tree.extend(A[i] - 'a', 1);
	for (int i = T; i; i--) tree.extend(A[i] - 'a', 0);
	for (int i = 2; i <= tree.cur; i++) c[tree.len[i]]++;
	for (int i = 1; i <= L; i++) c[i] += c[i - 1];
	for (int i = 2; i <= tree.cur; i++) id[c[tree.len[i]]--] = i;
	for (int i = tree.cur - 1; i; i--)  tree.cnt[tree.fail[id[i]]] += tree.cnt[id[i]];
	for (int i = 2; i <= tree.cur; i++) ans = max(ans, 1ll * tree.len[i] * tree.cnt[i]);
	printf("%lld\n", ans);
}