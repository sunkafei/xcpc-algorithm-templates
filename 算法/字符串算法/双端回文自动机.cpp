/*
len[i]表示编号为i的节点表示的回文串的长度（一个节点表示一个回文串）
fail[i]表示结点i失配以后跳转到的最长后缀回文串对应的结点
cnt[i]表示结点i对应的字符串出现的次数（建树时求出的不是完全的，最后calc()函数跑一遍以后才是正确的）
num[i]表示以结点i表示的最长回文串的最右端点为回文串结尾的回文串个数
num[i]也表示结点i在fail树中的深度
若要判断回文自动机中是否包含回文串P[0, m - 1]，则根据P的长度是奇数还是偶数来选择根结点（长度为偶数的回文串以0为根结点，长度为奇数的回文串以1为根结点），
然后将字符串P[m / 2, m - 1]输入自动机，到达了结点node，自动机包含回文串P当且仅当len[node] == m。
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 300010; //maxn至少为字符串最大可能长度的两倍
long long ans = 0; //统计当前的字符串中有多少个回文串（位置不同即不同）
struct Palindromic_Tree {
	int ch[maxn][26], fail[maxn], info[maxn];
	int cnt[maxn], num[maxn], len[maxn];
	int last[2], cur, L, R;
	void init() {
		memset(ch, 0, sizeof(ch));
		memset(cnt, 0, sizeof(cnt));
		memset(info, -1, sizeof(info)); //info的初始值为不会出现在输入字符串中的值即可
		len[1] = -1;
		cur = fail[0] = last[1] = 1;
		last[0] = 0;
		L = maxn / 2;
		R = L - 1;
	}
	int get_fail(int x, int back) {
		int k = back ? R : L;
		int tp = back ? 1 : -1;
		while (info[k - (len[x] + 1) * tp] != info[k])
			x = fail[x];
		return x;
	}
	void insert(int w, int back) { //back为1则插入w到字符串的结尾，为0则插入到开头。
		if (back) info[++R] = w;
		else info[--L] = w;
		int p = get_fail(last[back], back);
		if (!ch[p][w]) {
			int u = ++cur;
			fail[u] = ch[get_fail(fail[p], back)][w];
			len[u] = len[p] + 2;
			ch[p][w] = u;
			num[u] = num[fail[u]] + 1;
		}      
		last[back] = ch[p][w];
		if (len[last[back]] == R - L + 1)
			last[back ^ 1] = last[back];
		ans += num[last[back]]; //当前结点的插入会使得字符串中新增num[last[back]]个新的回文串
		cnt[last[back]]++;
	}
	void calc() {
		for (int i = cur; i >= 2; --i)
			cnt[fail[i]] += cnt[i];
	}
}tree;
int main(){
	//freopen("in.txt", "r", stdin);
	int n;
	while (scanf("%d", &n) == 1) {
		tree.init(); ans = 0;
		while (n--) {
			int tp;
			scanf("%d", &tp);
			if (tp == 1) {
				char c;
				scanf(" %c", &c);
				tree.insert(c - 'a', false);
			}
			else if (tp == 2) {
				char c;
				scanf(" %c", &c);
				tree.insert(c - 'a', true);
			}
			else if (tp == 3) {
				printf("%d\n", tree.cur - 1);
			}
			else {
				printf("%lld\n", ans);
			}
		}
	}
}
