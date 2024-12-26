#include <cstdio>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;
constexpr int maxn = 210000;
vector<int> G[maxn];
int pre[maxn], iscut[maxn], dfs_clock;
int dfs(int x, int fa = -1) {
	int lowx = pre[x] = ++dfs_clock;
	int child = 0;
	for (auto y : G[x]) {
		if (!pre[y]) {
			child += 1;
			int lowy = dfs(y, x);
			lowx = min(lowx, lowy);
			if (lowy >= pre[x] && fa != -1) {
				iscut[x] = true;
			}
			if (lowy > pre[x]) {
				// 边(x, y)是桥
				// 如果图中存在重边应当修改下面的条件：改为判断是否为反向边
			}
		}
		else if (y != fa) {
			lowx = min(lowx, pre[y]);
		}
	}
	if (fa == -1 && child >= 2) {
		iscut[x] = true;
	}
	return lowx;
}
void find_cut(int n) {
	for (int x = 0; x <= n; ++x) {
		pre[x] = iscut[x] = 0;
	}
	dfs_clock = 0;
	for (int x = 1; x <= n; ++x) { // 下标从1开始
		if (!pre[x]) {
			dfs(x, -1);
		}
	}
}
int main() { // 洛谷P3388
	//freopen("in.txt", "r", stdin);
	int n, m;
	scanf("%d %d", &n, &m);
	for (int i = 0; i < m; ++i) {
		int x, y;
		scanf("%d %d", &x, &y);
		G[x].push_back(y);
		G[y].push_back(x);
	}
	find_cut(n);
	printf("%d\n", accumulate(iscut, iscut + n + 1, 0));
	for (int x = 1; x <= n; ++x) if (iscut[x]) {
		printf("%d ", x);
	}
	printf("\n");
	return 0;
}