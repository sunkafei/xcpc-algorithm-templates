#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;
const int maxn = 110000;
int n, m, degree[maxn];
vector<int> G[maxn];
vector<int> toposort1() {
	queue<int> Q;
	for (int i = 1; i <= n; ++i)
		degree[i] = 0;
	for (int x = 1; x <= n; ++x)
		for (auto y : G[x])
			degree[y]++;
	for (int i = 1; i <= n; ++i) if (degree[i] == 0)
		Q.push(i);
	vector<int> res;
	while (!Q.empty()) {
		int x = Q.front(); Q.pop();
		res.push_back(x);
		for (auto y : G[x]) {
			degree[y]--;
			if (degree[y] == 0)
				Q.push(y);
		}
	}
	return res;
}
vector<int> result;
int vis[maxn];
void dfs(int x) {
	vis[x] = true;
	for (auto y : G[x]) if (!vis[y])
		dfs(y);
	result.push_back(x);
}
vector<int> toposort2() { //必须保证是有向无环图
	memset(vis, 0, sizeof(vis));
	for (int i = 1; i <= n; ++i) if (!vis[i])
		dfs(i);
	reverse(result.begin(), result.end());
	return result;
}
int main() { //uva 10305
	//freopen("in.txt", "r", stdin);
	while (scanf("%d %d", &n, &m) == 2) {
		if (n == 0 && m == 0)
			break;
		for (int i = 1; i <= n; ++i)
			G[i].clear();
		for (int i = 0; i < m; ++i) {
			int x, y;
			scanf("%d %d", &x, &y);
			G[x].push_back(y);
		}
		auto ans = toposort2();
		printf("%d", ans[0]);
		for (int i = 1; i < n; ++i)
			printf(" %d", ans[i]);
		printf("\n");
	}
	return 0;
}