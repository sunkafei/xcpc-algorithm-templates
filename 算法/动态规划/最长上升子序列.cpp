#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 110000;
const int inf = 1 << 30;
//d[i]表示以下标i结尾的最长上升子序列长度
int a[maxn], g[maxn], d[maxn], n;
int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i)
		scanf("%d", &a[i]);
	for (int i = 1; i <= n; ++i)
		g[i] = inf;
	for (int i = 1; i <= n; ++i) {
		int k = lower_bound(g + 1, g + n + 1, a[i]) - g;
		g[k] = a[i];
		d[i] = k;
	}
	int ans = *max_element(d + 1, d + n + 1);
	printf("%d\n", ans);
	return 0;
}