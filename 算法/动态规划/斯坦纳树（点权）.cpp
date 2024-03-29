/*
BZOJ2595
输入一张带有点权的图，求将所有权值为0的结点连接起来所需要的最小费用（最小权值和）。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;
const int inf = 0x3f3f3f3f; //inf + inf 必须小于 INT_MAX
							//如果此处修改inf，那么也应当在init函数中修改dp数组的初始值。
const int maxn = 130;
const int maxedges = 1100;
const int maxstate = 11000;
struct SteinerTree {
	int n, k; //n为图中点的个数，k为斯坦纳树的结点数 	
	int dp[maxn][maxstate], st[maxn], val[maxn]; //若点i为点集中的点则st[i]为该点对应的状态，否则为0 
	int first[maxn], nxt[maxedges * 2], to[maxedges * 2], cur; //图的邻接表 
	pair<int, int> pre[maxn][maxstate];
	bool inq[maxn][maxstate], vis[maxn];
	queue<int> Q;
	void init(int n, int* w, vector<int> c) { //n为图中的所有结点个数，c为斯坦纳树的结点集合		
		this->n = n; this->k = c.size();	  //w为点权
		memset(dp, 0x3f, sizeof(dp));
		memset(pre, 0x3f, sizeof(pre));
		memset(st, 0, sizeof(st));
		for (int i = 0; i < k; ++i)
			st[c[i]] = (1 << i);
		for (int i = 1; i <= n; ++i)
			dp[i][st[i]] = 0;
		for (int i = 1; i <= n; ++i)
			val[i] = w[i];
		memset(inq, 0, sizeof(inq));
		memset(vis, 0, sizeof(vis));
		while (!Q.empty()) Q.pop();
		memset(first, 0, sizeof(first));
		cur = 0;
	}
	void add_edge(int u, int v) {
		nxt[++cur] = first[u];
		first[u] = cur;
		to[cur] = v;
	}
	void spfa(int s) { //对当前点集状态为s的dp值进行松弛 
		while (!Q.empty()) {
			int u = Q.front(); Q.pop();
			for (int i = first[u]; i; i = nxt[i]) {
				int v = to[i];
				if (dp[v][s] > dp[u][s] + val[v]) {
					dp[v][s] = dp[u][s] + val[v];
					pre[v][s] = make_pair(u, s);
					if (!inq[v][s]) {
						Q.push(v);
						inq[v][s] = true;
					}
				}
			}
			inq[u][s] = false;
		}
	}
	void solve() {	//斯坦纳树的权值和为min{dp[i][(1 << k) - 1]}, 1 <= i <= n
		for (int j = 1; j < (1 << k); ++j) {
			for (int i = 1; i <= n; ++i) {
				for (int sub = (j - 1) & j; sub; sub = (sub - 1) & j) {
					int x = sub, y = j - sub;
					int t = dp[i][x] + dp[i][y] - val[i];
					if (dp[i][j] > t) {
						dp[i][j] = t;
						pre[i][j] = make_pair(i, sub);
					}
				}
				if (dp[i][j] < inf) {
					Q.push(i);
					inq[i][j] = true;
				}
			}
			spfa(j);
		}
	}
	void dfs(int i, int state) {
		if (i == inf || pre[i][state].second == 0)
			return;
		vis[i] = 1; //vis[i]表示结点i存在于斯坦纳树中
		pair<int, int> pr = pre[i][state];
		dfs(pr.first, pr.second);
		if (pr.first == i)
			dfs(i, state - pr.second);
	}

}tree;
int A[maxn][maxn], weight[maxn], dr[] = { -1, 1, 0, 0 }, dc[] = { 0, 0, -1, 1 };
int main() {
#define node(i, j) ((i) * m + (j) + 1)
	//freopen("in.txt", "r", stdin);
	int n, m, nd = -1;
	scanf("%d %d", &n, &m);
	vector<int> c;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			scanf("%d", &A[i][j]);
			weight[node(i, j)] = A[i][j];
			if (A[i][j] == 0) {
				c.push_back(node(i, j));
				if (nd == -1) nd = node(i, j);
			}
		}
	}
	tree.init(n * m, weight, c);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			for (int k = 0; k < 4; ++k) {
				int x = i + dr[k], y = j + dc[k];
				if (x < 0 || y < 0 || x >= n || y >= m) continue;
				tree.add_edge(node(i, j), node(x, y));
			}
		}
	}
	tree.solve();
	printf("%d\n", tree.dp[nd][(1 << c.size()) - 1]);
	tree.dfs(nd, (1 << c.size()) - 1);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (!A[i][j]) printf("x");
			else if (tree.vis[node(i, j)]) printf("o");
			else printf("_");
			if (j == m - 1) printf("\n");
		}
	}
	return 0;
}