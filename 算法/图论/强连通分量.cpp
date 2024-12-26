#include <vector>
#include <cstdio>
#include <stack>
#include <ranges>
#include <algorithm>
using namespace std;
constexpr int maxn = 11000;
vector<int> G[maxn];
int pre[maxn], lowlink[maxn], sccno[maxn], scc_cnt, dfs_clock;
stack<int> S;
void tarjan(int x) {
	pre[x] = lowlink[x] = ++dfs_clock;
	S.push(x);
	for (auto y : G[x]) {
		if (!pre[y]) {
			tarjan(y);
			lowlink[x] = min(lowlink[x], lowlink[y]);
		}
		else if (!sccno[y]) {
			lowlink[x] = min(lowlink[x], pre[y]);
		}
	}
	if (lowlink[x] == pre[x]) {
		scc_cnt += 1;
		for (;;) {
			int y = S.top(); S.pop();
			sccno[y] = scc_cnt;
			if (y == x) {
				break;
			}
		}
	}
}
void find_scc(int n) {
	dfs_clock = scc_cnt = 0;
	for (int x = 0; x <= n; ++x) {
		sccno[x] = pre[x] = 0;
	}
	for (int x = 1; x <= n; ++x) { //点的编号从1开始
		if (!pre[x]) {
			tarjan(x);
		}
	}
}
int main() { // 洛谷B3609
	int n, m;
	scanf("%d %d", &n, &m);
	for (int i = 0; i < m; ++i) {
		int x, y;
		scanf("%d %d", &x, &y);
		G[x].push_back(y);
	}
	find_scc(n);
	printf("%d\n", scc_cnt);
	vector<int> scc[maxn];
	for (int i = 1; i <= n; ++i) {
		scc[sccno[i]].push_back(i);
	}
	for (int i = 1; i <= scc_cnt; ++i) {
		std::ranges::sort(scc[i]);
	}
	bool vis[maxn] {};
	for (int i = 1; i <= n; ++i) {
		if (!vis[sccno[i]]) {
			for (auto x : scc[sccno[i]]) {
				printf("%d ", x);
			}
			printf("\n");
			vis[sccno[i]] = true;
		}
	}
	return 0;
}