/*
在Trie树中建立后缀自动机：
只需DFS一遍Trie树，对于Trie中的当前结点u，它的父结点为p，p在后缀自动机中对应的结点为v。
则将v作为last，将u对应的字符插入后缀自动机中即可。（一定要先插入结点u，再插入结点u在Trie树中的子结点）
在建立广义后缀自动机时：
对于当前插入的字符串s，若s的前缀s[0, i]也是之前某个字符串的前缀，则在插入s[0, i]时，字符s[0]对应的结点出度为0，
而对于s[1, i]中的每个字符s[t]，它对应的结点在后缀自动机中入度为1，来源于s[t-1]。所以若从根结点开始走，不会走到这些结点，
故在后缀自动机中插入多个字符串不会破坏自动机的结构。同时对于s[0, i]中的每一个字符s[t]，它对应的结点虽然不可达，
但是会在parent树中出现，所以广义后缀自动机中记录的信息是完全的。
设在自动机中输入字符串str后到达结点u，并设u的right集合为R（一个结点right集合为其子结点right集合的并集），
则R记录了str在每个字符串中的所有的结束位置。
*/
//结论：从空串开始不断随机字符放到结尾直到s作为子串出现的期望次数=对s的所有border i计算f(i)的和，f(i)表示从头开始直接随机出i的概率的倒数
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <set>
#include <map>
using namespace std;
const int maxn = 210000; //maxn应当大于最大字符串长度的2倍
//后缀自动机的基本数据
int par[maxn], len[maxn], go[maxn][26]; //min[s] = len[par[s]] + 1，当前结点表示的最小长度等于父结点的最大长度加一
int cur, root, last;
//用于求parent树的DFS序
int first[maxn], nxt[maxn], info[maxn], id[maxn], L[maxn], R[maxn]; //right集合中的下标是子串**右**端点的位置
vector<int> seq;
//用于对后缀自动机进行拓扑排序
int cnt[maxn], arr[maxn];
int newstate(int length, int index = -1) {
	len[cur] = length;
	id[cur] = index;
	return cur++;
}
void init() {
	memset(go, 0, sizeof(go)); //如果数据组数很多，可能超时，应当改为memset(go, 0, sizeof(go[0]) * (2 * n + 10));
	cur = 1;
	root = last = newstate(0);
}
void extend(int w, int index) {
	int x = last;
	int nx = newstate(len[x] + 1, index);
	while (x && !go[x][w])
		go[x][w] = nx, x = par[x];
	if (!x)
		par[nx] = root;
	else {
		int y = go[x][w];
		if (len[x] + 1 == len[y])
			par[nx] = y;
		else {
			int ny = newstate(len[x] + 1);
			memcpy(go[ny], go[y], sizeof(go[y]));
			par[ny] = par[y];
			par[y] = ny;
			par[nx] = ny;
			while (x && go[x][w] == y)
				go[x][w] = ny, x = par[x];
		}
	}
	last = nx;
	//每加入一个新的字符，新产生的本质不同的子串个数为len[last] - len[par[last]]
}
set<int> travel(int x) { //采用按秩合并的方式处理得到结点x的right集合，最坏时间复杂度nlog^2n
	set<int> right;
	vector<set<int> > child;
	for (int i = first[x]; i != -1; i = nxt[i]) {
		child.push_back(travel(info[i]));
	}
	sort(child.begin(), child.end(), [](const auto& A, const auto& B) {
		return A.size() > B.size();
		});
	if (!child.empty())
		right = std::move(child[0]);
	for (int i = 1; i < child.size(); ++i)
		for (auto item : child[i])
			right.insert(item);
	if (id[x] != -1)
		right.insert(id[x]);
	//此处通过前面得到的right集合来计算答案，注意此处代码的时间复杂度不能达到O(n)，
	//否则总的时间复杂度可能会到O(n^2)甚至更高。
	return right;
}
void dfs(int x) {
	L[x] = seq.size();
	if (id[x] != -1)
		seq.push_back(id[x]);
	for (int i = first[x]; i != -1; i = nxt[i])
		dfs(info[i]);
	R[x] = seq.size() - 1;
}
void build() { //建树
	int sz = 0;
	seq.clear(); //可以在此处向seq中加入一个元素0，从而使得DFS序区间从1开始
	memset(first, -1, sizeof(first));
	for (int i = 1; i < cur; ++i) {
		int p = par[i];
		info[sz] = i;
		nxt[sz] = first[p];
		first[p] = sz++;
	}
	//travel(root); //遍历子树求解问题
	dfs(root); //求parent树的DFS序
}
void topsort() { //对后缀自动机进行拓扑排序
	for (int i = 1; i < cur; ++i)
		cnt[i] = 0;
	for (int i = 1; i < cur; ++i)
		cnt[len[i]]++;
	for (int i = 1; i < cur; ++i)
		cnt[i] += cnt[i - 1];
	for (int i = cur - 1; i >= 1; --i) //arr: [1, cur)
		arr[cnt[len[i]]--] = i;
	for (int i = 2; i < cur; ++i) //只有建立广义后缀自动机的时候才需要这个循环
		if (par[arr[i - 1]] == arr[i])
			swap(arr[i - 1], arr[i]);
	/*
		拓扑排序后从叶结点开始递推更新id数组，注意循环完成后id[0]为所有结点id的或。
		for (int i = cur - 1; i >= 1; --i) {
			int j = arr[i];
			if (id[par[j]] == -1)
				id[par[j]] = id[j];
			else
				id[par[j]] |= id[j];
		}
	*/
}
void insert(char* str, int id) { //通过多次调用insert函数向自动机中插入多个字符串，来建立广义后缀自动机。
	int n = strlen(str);
	last = root; //!!!
	for (int i = 0; i < n; ++i)
		extend(str[i] - 'a', id * 100 + i); //可以将第二个参数改为pair，来记录插入字符来源于的字符串编号和下标。
}
const int maxlog = 25;
int anc[maxn][maxlog]; //anc[i][j]表示结点i往上走2^j个点到达的点
map<int, int> trans; //id函数的反函数
void preprocess() { //parent树上倍增初始化
	trans.clear();
	for (int i = 1; i < cur; ++i)
		if (id[i] != -1)
			trans[id[i]] = i;
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
int query(int id, int length) { //从标号id的结点往上走到len[p] >= length的深度最小的结点
	int p = trans[id];			//若要识别字符串的子串[L, R]，则调用query(id(R), R-L+1)，其中id(R)表示下标R对应的id
	for (int i = maxlog - 1; i >= 0; --i)
		if (len[anc[p][i]] >= length)
			p = anc[p][i];
	return p;
}
namespace Suffix_Tree { //建立反串的后缀树（后缀自动机的parent树是反串的后缀树）
	int pos[maxn]; //pos[x]表示结点x对应的字符串在原串中的结束下标。
	int ch[maxn][26]; //ch[x][w]表示结点x在后缀树中沿着字符w走到达的结点。
	char s[maxn];
	void extend(int w, int index) { //比之前的extend多了维护pos数组的代码。
		int x = last;
		int nx = newstate(len[x] + 1, index);
		pos[nx] = index;
		while (x && !go[x][w])
			go[x][w] = nx, x = par[x];
		if (!x)
			par[nx] = root;
		else {
			int y = go[x][w];
			if (len[x] + 1 == len[y])
				par[nx] = y;
			else {
				int ny = newstate(len[x] + 1);
				memcpy(go[ny], go[y], sizeof(go[y]));
				pos[ny] = pos[y];
				par[ny] = par[y];
				par[nx] = par[y] = ny;
				while (x && go[x][w] == y)
					go[x][w] = ny, x = par[x];
			}
		}
		last = nx;
	}
	void build(const char* str) { //对字符串str建立后缀自动机和反串的后缀树。
		strcpy(s, str);
		int n = strlen(s);
		last = root;
		for (int i = 0; i < n; ++i)
			extend(s[i] - 'a', i);
		for (int i = 2; i < cur; ++i) {
			int w = s[pos[i] - len[par[i]]] - 'a';
			ch[par[i]][w] = i;
		}
	}
	int walk(int x, int length, const char* str) { //从结点x开始沿着后缀树走字符串str，如果终止在边内则会到子结点。
		int m = strlen(str);
		for (int i = 0; i < m && i < len[x] - length; ++i)
			if (str[i] != s[pos[x] - length - i])
				return 0;
		for (int i = len[x] - length; i < m; i += len[x] - len[par[x]]) {
			int c = str[i] - 'a';
			if (!ch[x][c])
				return 0;
			x = ch[x][c];
			for (int j = 0; j < len[x] - len[par[x]] && i + j < m; ++j)
				if (str[i + j] != s[pos[x] - len[par[x]] - j])
					return 0;
		}
		return x;
	}
	int expand(int L, int R, const char* str) {
		int length = R - L + 1;
		int x = query(R, length);
		return walk(x, length, str);
	}
	/*
	建立的后缀树是后缀自动机的辅助数据结构，假设对字符串s建立后缀自动机，令结点x表示字符串s[L, R]，
	则go[x][c]表示字符串s[L, R] + c对应的结点，ch[x][c]表示字符串c + s[L, R]对应的结点。
	注意因为ch对应的边是后缀树中的边，边上是压缩的字符串而不是一个字符，所以连续向左扩展的时候应该
	调用walk函数或expand函数，而不是直接走ch。
	walk函数用字符串str向左扩展状态x，因为一个状态可能对应多个字符串，所以用length来明确指明字符串的长度，
	length应满足条件：len[par[x]] < length <= len[x]
	注意该函数会先扩展str[0]然后str[1]、str[2]...，最终结果是将str的反串拼接在x所表示的字符串的左边得到的结果。
	expand函数返回将str反向拼接在s[L, R]的左边所得到的状态。
	*/
}
namespace MultiString {
	//label[i]表示结点i的标记，times[i]记录结点i在多少个字符串中出现
	//real[i]表示结点i所能表示的最大长度（所有字符串在结点i所能表示的最大长度的最小值）
	int label[maxn], mx[maxn], times[maxn], real[maxn];
	void preprocess() { //初始化数据结构，要在match之前extend之后调用（必须在后缀自动机建立完成后调用）
		memset(label, -1, sizeof(label));
		memset(times, 0, sizeof(times));
		for (int i = 0; i <= cur; ++i)
			real[i] = len[i];
	}
	void match(const char* str, int index) {
		vector<pair<int, int>> nodes;
		int x = root, length = 0;
		for (const char* s = str; *s; ++s) {
			int c = *s - 'a';
			while (x != root && !go[x][c]) {
				x = par[x];
				length = len[x];
			}
			if (go[x][c]) {
				x = go[x][c];
				length += 1;
			}
			nodes.emplace_back(x, length);
		}
		sort(nodes.begin(), nodes.end(), [](auto a, auto b) {
			return len[a.first] > len[b.first];
		});
		vector<int> vec;
		for (auto pr : nodes) {
			int x = pr.first;
			while (x && label[x] != index) {
				vec.push_back(x);
				label[x] = index;
				mx[x] = 0;
				x = par[x];
			}
			mx[pr.first] = max(mx[pr.first], pr.second);
		}
		sort(vec.begin(), vec.end(), [](int a, int b) {
			return len[a] > len[b];
		});
		for (auto x : vec) {
			mx[par[x]] = max(mx[par[x]], len[par[x]]);
			real[x] = min(real[x], mx[x]);
			times[x] += 1;
		}
	}
	int longest_common_string(int n) {
		//设有n个字符串，任选一个构建后缀自动机，对其余的串调用match函数，
		//然后调用该函数，返回这些字符串的最长公共子串长度。
		int ans = 0;
		for (int i = 1; i < cur; ++i) if (times[i] == n - 1)
			ans = max(ans, real[i]);
		return ans;
	}
	long long number_of_common_string(int n) {
		//设有n个字符串，任选一个构建后缀自动机，对其余的串调用match函数，
		//然后调用该函数，返回这些字符串的本质不同的公共子串个数。
		long long ans = 0;
		for (int i = 1; i < cur; ++i) if (times[i] == n - 1)
			ans += max(0, real[i] - len[par[i]]);
		return ans;
	}
}
namespace Generalized_Suffix_Automaton { //广义后缀自动机
	//pos[i]表示结点i对应的子串在index==0的字符串中最左侧出现的下标
	//times[i]表示在parent树中结点i的子树中包含的类型数
	int par[maxn], len[maxn], go[maxn][26];
	int label[maxn], times[maxn];
	int n, cur, root, last, pos[maxn];
	int newstate(int length, int index = maxn) {
		len[cur] = length;
		pos[cur] = index;
		return cur++;
	}
	void init() { //n表示字符串的个数，需要在外部初始化
		memset(label, -1, sizeof(label));
		cur = 1;
		root = last = newstate(0);
	}
	void extend(int w, int index) {
		int x = last;
		int nx = newstate(len[x] + 1, index);
		while (x && !go[x][w])
			go[x][w] = nx, x = par[x];
		if (!x)
			par[nx] = root;
		else {
			int y = go[x][w];
			if (len[x] + 1 == len[y])
				par[nx] = y;
			else {
				int ny = newstate(len[x] + 1, pos[y]);
				memcpy(go[ny], go[y], sizeof(go[y]));
				times[ny] = times[y];
				par[ny] = par[y];
				par[y] = ny;
				par[nx] = ny;
				while (x && go[x][w] == y)
					go[x][w] = ny, x = par[x];
			}
		}
		last = nx;
	}
	//多次调用insert建立广义自动机，index从0开始
	void insert(const string& s, int index) {
		last = root;
		int pre = cur;
		for (int i = 0; i < s.size(); ++i)
			extend(s[i] - 'a', index == 0 ? i : maxn);
		for (int i = pre; i < cur; ++i) {
			for (int x = i; x && label[x] != index; x = par[x]) {
				label[x] = index;
				times[x] += 1;
				pos[par[x]] = min(pos[par[x]], pos[x]);
			}
		}
	}
	//dp[i]表示从结点i对应的任意字符串开始，能走到的本质不同的子串个数
	long long dp[maxn];
	long long dfs(int x) {
		if (dp[x] > 0)
			return dp[x];
		dp[x] = 0;
		for (int i = 0; i < 26; ++i) if (go[x][i] && times[go[x][i]] == n)
			dp[x] += dfs(go[x][i]) + 1;
		return dp[x];
	}
	void output(int x, long long k) { //输出第k小的本质不同公共子串
		if (k == 0) return;
		for (int i = 0; i < 26; ++i) if (go[x][i] && times[go[x][i]] == n) {
			if (k - dp[go[x][i]] - 1 > 0)
				k -= dp[go[x][i]] + 1;
			else {
				putchar('a' + i);
				output(go[x][i], k - 1);
				return;
			}
		}
	}
}
int main() {
	//freopen("in.txt", "r", stdin);
	static char s[maxn];
	init();
	scanf("%s", s);
	insert(s, 0);
	int n = 1;
	MultiString::preprocess();
	while (scanf("%s", s) == 1) 
		MultiString::match(s, ++n);
	printf("%d\n", MultiString::longest_common_string(n));
	return 0;
}

