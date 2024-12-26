#include <cstdio>
#include <vector>
#include <numeric>
#include <algorithm>
#include <stack>
using namespace std;
constexpr int maxn = 510000;
vector<int> G[maxn], bcc[maxn];
stack<int> S;
int pre[maxn], dfs_clock, bcc_cnt;
int dfs(int x, int fa = -1) {
	int lowx = pre[x] = ++dfs_clock;
	S.push(x);
	for (auto y : G[x]) {
		if (!pre[y]) {
			int lowy = dfs(y, x);
			lowx = min(lowx, lowy);
		}
		else if (y != fa) { //如果图中存在重边应当修改这个条件：改为判断是否为反向边
			lowx = min(lowx, pre[y]);
		}
	}
	if (lowx == pre[x]) {
		bcc_cnt += 1;
		for (;;) {
			int s = S.top(); S.pop();
			bcc[bcc_cnt].push_back(s);
			if (s == x) {
				break;
			}
		}
	}
	return lowx;
}
void find_bcc(int n) {
	for (int x = 0; x <= n; ++x) {
		pre[x] = 0;
		bcc[x].clear();
	}
	while (S.size()) S.pop();
	dfs_clock = bcc_cnt = 0;
	for (int x = 1; x <= n; ++x) { // 下标从1开始
		if (!pre[x]) {
			dfs(x, -1);
		}
	}
}
int main() { // 洛谷P8436
	//freopen("in.txt", "r", stdin);
	int n, m;
	scanf("%d %d", &n, &m);
	for (int i = 0; i < m; ++i) {
		int x, y;
		scanf("%d %d", &x, &y);
		G[x].push_back(y);
		G[y].push_back(x);
	}
	find_bcc(n);
	printf("%d\n", bcc_cnt);
	for (int i = 1; i <= bcc_cnt; ++i) {
		printf("%d", (int)bcc[i].size());
		for (auto x : bcc[i]) {
			printf(" %d", x);
		}
		printf("\n");
	}
	return 0;
}