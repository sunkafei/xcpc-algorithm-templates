/*
HDU4085
给定n个点，前k个点属于集合A，后k个点属于集合B。点之间通过m条带权边相连。
要求选出权值和最小的边集使得A集合中的每个点都可以与B中的某个点进行配对（若a能走到b则可以考虑是使其配对）。
解决方案：将集合A与集合B中的点作为关键点求出斯坦纳树，因为最后的结果可以是一个森林，所以我们在进行一次DP。
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
const int maxn = 110;
const int maxedges = 1100;
const int maxstate = 11000;
struct SteinerTree{	
	int n, k; //n为图中点的个数，k为斯坦纳树的结点数 	
	int dp[maxn][maxstate], st[maxn]; //若点i为点集中的点则st[i]为该点对应的状态，否则为0 
	int first[maxn], nxt[maxedges * 2], to[maxedges * 2], weight[maxedges * 2], cur; //图的邻接表 
	bool inq[maxn][maxstate];
	queue<int> Q;	
	void init(int n, vector<int> c) { //n为图中的所有结点个数，c为斯坦纳树的结点集合		
		this->n = n; this->k = c.size();
		memset(dp, 0x3f, sizeof(dp));
		memset(st, 0, sizeof(st));		
		for(int i = 0; i < k; ++i)	
			st[c[i]] = (1 << i);	
		for(int i = 1; i <= n; ++i)		
			dp[i][st[i]] = 0;
		memset(inq, 0, sizeof(inq));	
		while(!Q.empty()) Q.pop();		
		memset(first, 0, sizeof(first));
		cur = 0;
	}	
	void add_edge(int u, int v, int w) {		
		nxt[++cur] = first[u];
		first[u] = cur;
		to[cur] = v;
		weight[cur] = w;
	}		
	void spfa(int s) { //对当前点集状态为s的dp值进行松弛 
		while (!Q.empty()) {
			int u = Q.front(); Q.pop();
			inq[u][s] = false;
			for (int i = first[u]; i; i = nxt[i]) {
				int v = to[i], w = weight[i];
				int state = st[v] | s;
				if (dp[v][state] > dp[u][s] + w) {
					dp[v][state] = dp[u][s] + w;
					if (state != s || inq[v][s])
						continue;
					Q.push(v);
					inq[v][s] = true;
				}
			}
		}
	}
	void solve() {	//斯坦纳树的权值和为min{dp[i][(1 << k) - 1]}, 1 <= i <= n	
		for (int j = 1; j < (1 << k); ++j) {			
			for (int i = 1; i <= n; ++i) {			
				if (st[i] && (st[i] & j) == 0)
					continue;	
				for (int sub = (j - 1) & j; sub; sub = (sub - 1) & j) {				
					int x = st[i] | sub, y = st[i] | (j - sub);												
					dp[i][j] = min(dp[i][j], dp[i][x] + dp[i][y]);						
				}			
				if (dp[i][j] != inf) {					
					Q.push(i);
					inq[i][j] = true;
				}			
			}			
			spfa(j);
		}
		/*
		int ans = inf;
		for (int i = 1; i <= n; ++i)
			ans = min(ans, tree.dp[i][(1 << k) - 1]);
		*/
	}
}tree;

int n, m, k, d[maxstate];
bool check(int s) { //当且仅当状态s中A集合与B集合结点个数相等时，才是合法状态
	int res = 0;
	for (int i = 0; s; i++, s >>= 1)
		res += (s & 1) * (i < k ? 1 : -1);
	return (res == 0 ? true : false);
}
int main() {
	//freopen("in.txt", "r", stdin);
	int T;
	scanf("%d", &T);
	while (T--) {
		scanf("%d %d %d", &n, &m, &k);
		vector<int> c;
		for (int i = 1; i <= k; ++i)
			c.push_back(i);
		for (int i = 1; i <= k; ++i)
			c.push_back(n - k + i);
		tree.init(n, c);
		for (int i = 0; i < m; ++i) {
			int u, v, w;
			scanf("%d %d %d", &u, &v, &w);
			tree.add_edge(u, v, w);
			tree.add_edge(v, u, w);
		}
		tree.solve();
		const int mask = (1 << (2 * k)) - 1;
		for (int s = 0; s <= mask; s++) {
			d[s] = inf;    
			for (int i = 1; i <= n; i++) 
				d[s] = min(d[s], tree.dp[i][s]); //初始值为状态s中的点连成一棵树的情况
		}   
		for (int s = 1; s <= mask; s++) if (check(s))
			for (int p = (s - 1) & s; p; p = (p - 1) & s) if (check(p))
				d[s] = min(d[s], d[p] + d[s - p]); //考虑将树分解为森林
		if (d[mask] >= inf)
			puts("No solution");    
		else 
			printf("%d\n", d[mask]);
	}
	return 0;
}