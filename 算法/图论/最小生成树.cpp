#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 210000;
int n, m, from[maxn], to[maxn], weight[maxn];
int p[maxn], r[maxn];
int find(int x) {
	return p[x] == x ? x : p[x] = find(p[x]);
}
int kruskal() {
	for (int i = 1; i <= n; ++i)
		p[i] = i;
	for (int i = 1; i <= m; ++i)
		r[i] = i;
	sort(r + 1, r + m + 1, [](int a, int b) {
		return weight[a] < weight[b];
	});
	int ans = 0, edges = 0;
	for (int i = 1; i <= m; ++i) {
		int e = r[i];
		int x = find(from[e]), y = find(to[e]);
		if (x != y) {
			p[x] = y;
			ans += weight[e];
			edges += 1;
		}
	}
	if (edges != n - 1)
		return -1; //无解
	else
		return ans;
}
int main() { //洛谷 P3366
	//freopen("in.txt", "r", stdin);
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= m; ++i)
		scanf("%d %d %d", &from[i], &to[i], &weight[i]);
	int ans = kruskal();
	if (ans == -1)
		puts("orz");
	else
		printf("%d\n", ans);
	return 0;
}