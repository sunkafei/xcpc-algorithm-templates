#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
const int maxn = 1110000; //maxn至少为最大结点数的两倍
vector<int> G[maxn], seq;
int pos[maxn], dep[maxn], lg[maxn], a[maxn][20];
void dfs(int x, int fa, int d) {
	dep[x] = d;
	pos[x] = seq.size();
	seq.push_back(x);
	for (auto y : G[x]) if (y != fa) {
		dfs(y, x, d + 1);
		seq.push_back(x);
	}
}
void init(int s) { //根结点为s，调用之前G中应当已经保存了整棵树。
	seq.resize(1);
	dfs(s, -1, 1);
	const int n = seq.size() - 1;
	lg[1] = 0;
	for (int i = 2; i <= n; ++i)
		lg[i] = lg[i >> 1] + 1;
	for (int i = 1; i <= n; ++i)
		a[i][0] = seq[i];
	for (int j = 1; j <= lg[n]; ++j) {
		for (int i = 1; i + (1 << j) - 1 <= n; ++i) {
			int x = a[i][j - 1], y = a[i + (1 << (j - 1))][j - 1];
			a[i][j] = (dep[x] < dep[y] ? x : y);
		}
	}
}
inline int lca(int x, int y) {
	int L = pos[x], R = pos[y];
	if (L > R)
		swap(L, R);
	int k = lg[R - L + 1];
	x = a[L][k];
	y = a[R - (1 << k) + 1][k];
	return dep[x] < dep[y] ? x : y;
}
inline int dist(int x, int y) {
	return dep[x] + dep[y] - 2 * dep[lca(x, y)];
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
	init(s);
	for (int i = 0; i < m; ++i) {
		int x, y;
		scanf("%d %d", &x, &y);
		printf("%d\n", lca(x, y));
	}
	return 0;
}