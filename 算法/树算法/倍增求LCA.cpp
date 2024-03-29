#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
const int maxn = 510000;
const int maxlog = 20;
vector<int> G[maxn];
int anc[maxn][maxlog], dep[maxn];
void dfs(int x, int fa, int d) {
	anc[x][0] = fa;
	dep[x] = d;
	for (auto y : G[x]) if (y != fa)
		dfs(y, x, d + 1);
}
void preprocess(int n) { //点的编号从1开始
	for (int j = 1; j < maxlog; ++j)
		for (int i = 0; i <= n; ++i)
			anc[i][j] = 0;
	dfs(1, 0, 0);
	for (int j = 1; j < maxlog; ++j)
		for (int i = 1; i <= n; ++i)
			anc[i][j] = anc[anc[i][j - 1]][j - 1];
}
//返回结点x向上走d步到达的结点
int moveup(int x, int d) {
	for (int i = 0; d >> i; ++i) 
		if (d >> i & 1)
			x = anc[x][i];
	return x;
}
int lca(int x, int y) {
	if (dep[x] < dep[y])
		swap(x, y);
	x = moveup(x, dep[x] - dep[y]);
	if (x == y)
		return x;
	for (int i = maxlog - 1; i >= 0; --i) 
		if (anc[x][i] != anc[y][i])
			x = anc[x][i], y = anc[y][i];
	return anc[x][0];
}
int dist(int x, int y) { //返回结点x和y之间的距离
	return dep[x] + dep[y] - 2 * dep[lca(x, y)];
}
int move(int x, int y, int d) { //返回从结点x向结点y走d步到达的结点
	int p = lca(x, y);
	int h = dep[x] - dep[p];
	if (h >= d)
		return moveup(x, d);
	else
		return moveup(y, dep[x] + dep[y] - d - 2 * dep[p]);
}
int main() {
	//freopen("in.txt", "r", stdin);
	int n, m, s;
	scanf("%d %d %d", &n, &m, &s);
	for (int i = 1; i < n; ++i) {
		int x, y;
		scanf("%d %d", &x, &y);
		G[x].push_back(y);
		G[y].push_back(x);
	}
	dfs(s, 0, 0);
	for (int j = 1; j < maxlog; ++j)
		for (int i = 1; i <= n; ++i)
			anc[i][j] = anc[anc[i][j - 1]][j - 1];
	for (int i = 0; i < m; ++i) {
		int x, y;
		scanf("%d %d", &x, &y);
		printf("%d\n", lca(x, y));
	}
	return 0;
}