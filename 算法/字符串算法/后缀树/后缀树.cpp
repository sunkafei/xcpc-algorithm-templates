/*
1.	ch[u][c]表示从结点u出发走字符c后到达的结点（0为空结点）。
2.	len[u]表示从结点u的父结点走到u的这条边的长度（可能为0）。
3.	从结点u的父结点走到u的字符串的下标区间为[start[u], start[u] + len[u] - 1]。
4.	link[u]记录结点u的后缀链接，若内部结点u表示的字符串为[s, t]，则其后缀链接的结点对应字符串为[s+1, t]。
5.	depth[u]表示结点u的加权深度，dep[u]表示结点的不加权深度（根结点到u经过的边数）。
6.	s数组记录输入的字符串。
7.	对于叶结点pos[i]表示结点i对应的后缀的下标，非叶结点pos[i]为-1。
8.	node是pos的反函数，node[i]表示后缀s[i, m]对应的结点（下标从1开始）。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <cassert>
using namespace std;
const int sigma_size = 26;
const int maxlog = 20;
const int maxn = 4e5 + 10;
const int inf = 1e9;
struct Suffix_Tree {
public: //后缀树基本结构
	int ch[maxn][sigma_size + 1], len[maxn], start[maxn], link[maxn], depth[maxn], s[maxn], cur, n, rem, now;
	int pos[maxn], node[maxn];
	void init() {
		memset(ch, 0, sizeof(ch));
		cur = now = 1;
		n = rem = 0;
		len[0] = inf;
		depth[0] = 0;
	}
	int newnode(int p, int w, int d) {
		start[++cur] = p;
		len[cur] = w;
		depth[cur] = d;
		link[cur] = 1;
		return cur;
	}
	void extend(int x) { //0 <= x <= sigma_size
		s[++n] = x, ++rem;
		for (int last = 1; rem; now == 1 ? --rem : now = link[now]) {
			while (rem > len[ch[now][s[n - rem + 1]]])
				rem -= len[now = ch[now][s[n - rem + 1]]];
			int ed = s[n - rem + 1];
			int& v = ch[now][ed];
			int c = s[start[v] + rem - 1];
			link[last] = now;
			if (!v) {
				v = newnode(n - rem + 1, inf, n - rem + 1 - depth[now]);
				last = now;
			}
			else if (x == c) {
				last = now;
				break;
			}
			else {
				int u = newnode(start[v], rem - 1, depth[now] + rem - 1);
				ch[u][x] = newnode(n, inf, n - depth[u]);
				ch[u][c] = v;
				start[v] += rem - 1;
				if (len[v] != inf)
					len[v] -= rem - 1;
				link[last] = v = u;
				last = v;
			}
		}
	}
	void maintain(int m) { //m为输入字符串的长度
	//对后缀树进行调整，并消除最后extend(sigma_size)对后缀树结构的影响。
	//调整后的后缀树的最后没有字符sigma_size，但是树边的长度可能为0，
	//并且遍历的时候要枚举所有的字符：0 <= c <= sigma_size
		ch[1][sigma_size] = 0;
		for (int i = 1; i <= cur; ++i) {
			if (len[i] == inf) {
				len[i] = m - start[i] + 1;
				pos[i] = depth[i];
				node[pos[i]] = i;
				depth[i] = m - depth[i] + 1;
			}
			else
				pos[i] = -1;
		}
	}
	void insert(const char* str) { //插入的字符串的下标从1开始
		assert(str[0] == 0);
		int m = strlen(str + 1);
		for (int i = 1; i <= m; ++i)
			extend(str[i] - 'a');
		extend(sigma_size);
		maintain(m);
	}
	int go(const char* str, int nd = 1) { //如果终止在边内则会到子结点
		int m = strlen(str);
		for (int i = 0; i < m; i += len[nd]) {
			int c = str[i] - 'a';
			if (!ch[nd][c])
				return 0;
			nd = ch[nd][c];
			for (int j = 0; j < len[nd] && i + j < m; ++j)
				if (str[i + j] - 'a' != s[start[nd] + j])
					return 0;
		}
		//如果有长度为0的边则直接走过去。在处理子树问题时应当直接返回nd。
		return ch[nd][sigma_size] != 0 ? ch[nd][sigma_size] : nd;
	}
public: //后缀树上倍增
	int L[maxn], R[maxn], dep[maxn], anc[maxn][maxlog]; //anc[i][j]表示结点i往上走2^j个点到达的点
	vector<int> seq;
	void dfs(int x, int fa) {
		dep[x] = dep[fa] + 1;
		anc[x][0] = fa;
		L[x] = seq.size();
		if (pos[x] > 0)
			seq.push_back(pos[x]);
		for (int i = 0; i <= sigma_size; ++i) if (ch[x][i])
			dfs(ch[x][i], x);
		R[x] = seq.size() - 1;
	}
	void preprocess() { //后缀树上倍增初始化
		for (int i = 1; i <= cur; ++i) {
			for (int j = 1; (1 << (j - 1)) <= cur; ++j)
				anc[i][j] = 0;
		}
		for (int j = 1; (1 << j) <= cur; ++j) {
			for (int i = 1; i <= cur; ++i) {
				if (anc[i][j - 1] != 0) {
					int a = anc[i][j - 1];
					anc[i][j] = anc[a][j - 1];
				}
			}
		}
	}
	void build() {		//求出后缀树的DFS序
		seq.clear();	//可以在此处向seq中加入一个元素0，从而使得DFS序区间从1开始
		dep[0] = -1;
		dfs(1, 0);		//求后缀树的DFS序
		preprocess();	//后缀树上倍增初始化
	}
	int ascend(int x, int length) { //将结点上升到加权深度为length的位置。
		for (int i = maxlog - 1; i >= 0; --i)
			if (depth[anc[x][i]] >= length) //调用该函数之前需要先调用preprocess初始化
				x = anc[x][i];
		return x;
	}
	int query(int L, int R) { //返回从根结点开始沿着字符串s[L, R]走到的结点（若最后结束在边上，则走到子结点）
		int x = node[L], length = R - L + 1;
		return ascend(x, length);
	}
	int lca(int x, int y) { //返回后缀树中结点x与y的最近公共祖先。
		if (dep[x] < dep[y])
			swap(x, y);
		for (int i = maxlog - 1; i >= 0; --i)
			if (dep[x] - (1 << i) >= dep[y])
				x = anc[x][i];
		if (x != y) {
			for (int i = maxlog - 1; i >= 0; --i)
				if (anc[x][i] && anc[x][i] != anc[y][i])
					x = anc[x][i], y = anc[y][i];
			x = anc[x][0];
		}
		return x;
	}
	int lcp(int i, int j) { //返回后缀s[i, m]与后缀s[j, m]的最长公共前缀。
		return depth[lca(node[i], node[j])];
	}
public: //后缀树上快速下降
	int sz[maxn], leaf[maxn];
	void travel(int x) { //调用travel(1)对后缀树进行树剖，之后才能调用go(x, L, R)。
		sz[x] = 1;
		leaf[x] = pos[x];
		for (int z = 0, i = 0; i <= sigma_size; ++i) {
			int y = ch[x][i];
			if (y) travel(y);
			sz[x] += sz[y];
			if (sz[y] > sz[z]) {
				z = y;
				leaf[x] = leaf[y];
			}
		}
	}
	int go(int x, int L, int R) { //从结点x开始往下走字符串s[L, R]，时间复杂度O(log^2n)
		while (x && L <= R) {
			if (s[L] == s[leaf[x] + depth[x]]) {
				int length = min(R - L + 1, lcp(leaf[x] + depth[x], L));
				L += length;
				int y = ascend(node[leaf[x]], depth[x] + length);
				if (L <= R && depth[y] != depth[x] + length)
					return 0;
				x = y;
			}
			else {
				int y = ch[x][s[L]];
				if (lcp(start[y], L) < min(len[y], R - L + 1))
					return 0;
				L += len[y];
				x = y;
			}
		}
		return x;
		//return ch[x][sigma_size] != 0 ? ch[x][sigma_size] : x;
	}
	int recognise(int l, int r, int L, int R) { //返回从根结点开始先走s[l, r]再走s[L, R]所到的结点。
		int x = query(l, r);
		int diff = depth[x] - (r - l + 1);
		if (diff > 0 && lcp(start[x] + len[x] - diff, L) < min(diff, R - L + 1))
			return 0;
		else
			return go(x, L + diff, R);
		//因为树中有长度为0的边，所以即使要识别的字符串为原串的后缀也不一定走到叶结点，
		//若要保证在这种情况下走到叶结点则应有：x = (ch[x][sigma_size] != 0 ? ch[x][sigma_size] : x);
	}
} st;

int main() {
	static char s[maxn];
	srand(time(0));
	int n = 200000;
	for (int i = 1; i <= n; ++i)
		s[i] = 'a' + (rand() % 2 == 0);
	auto start = clock();
	st.init();
	st.insert(s);
	st.build();
	st.travel(1);
	for (int i = 1; i <= n - 10; ++i) {
		int mid = (i + n) / 2;
		int x = st.recognise(i, mid, mid + 1, n);
		x = (st.ch[x][sigma_size] != 0 ? st.ch[x][sigma_size] : x);
		auto j = st.pos[x];
		if (j != i || st.depth[x] != n - i + 1) {
			printf("%d %d (%d)\n", j, i, x);
			abort();
		}
	}
	auto end = clock();
	printf("time: %f\n", double(end - start) / CLOCKS_PER_SEC);
	return 0;
}