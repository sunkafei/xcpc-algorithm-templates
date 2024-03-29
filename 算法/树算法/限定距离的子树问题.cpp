/*
该算法用来求解有根树中结点v的子树中与v距离不超过d的所有结点的权值和（也可以求最值等），
时间复杂度O(d)，若用倍增可以优化到O(logd)。
调用init函数之前应当输入n（结点标号从1开始），以及用add_edge建图。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;
const int maxn = 1010000;
int n, q, w[maxn], head[maxn], nxt[maxn * 2], to[maxn * 2], id[maxn], l[maxn], r[maxn];
int father[maxn], depth[maxn], seq[maxn], cur = 0;
long long sum[maxn];
void add_edge(int u, int v) {
	to[++cur] = v;
	nxt[cur] = head[u];
	head[u] = cur;
}
void init() {
	int sz = 0;
	queue<int> Q;
	father[1] = depth[1] = seq[n + 1] = 0;
	Q.push(1);
	while (!Q.empty()) {
		int x = Q.front(); Q.pop();
		seq[++sz] = x;
		id[x] = sz;
		l[x] = r[x] = 0;
		for (int i = head[x]; i; i = nxt[i]) if (to[i] != father[x]) {
			int y = to[i];
			l[x] = (l[x] ? l[x] : y);
			r[x] = y;
			father[y] = x;
			depth[y] = depth[x] + 1;
			Q.push(y);
		}
	}
	for (int j = 1; j <= n; ++j)
		sum[j] = sum[j - 1] + w[seq[j]];
	for (int j = 1; j <= n; ++j) if (!r[seq[j]])
		r[seq[j]] = r[seq[j - 1]];
	for (int j = n; j >= 1; --j) if (!l[seq[j]])
		l[seq[j]] = l[seq[j + 1]];
}
long long travel(int x, int d) { //求出以x为根的子树中与x距离不超过d的所有结点的权值和（可改为求最值或种类数）
	if (d < 0) return 0;		 //时间复杂度O(d)，可以改成倍增使得时间复杂度变为O(logd)
	long long ret = 0;
	int L = x, R = x;
	for (int i = 0; i <= d; ++i) {
		int left = id[L], right = id[R];
		if (L == 0 || R == 0 || left > right) break;
		long long s = sum[right] - sum[left - 1];
		ret += s;
		L = l[L];
		R = r[R];
	}
	return ret;
}

int main() {
	//freopen("in.txt", "r", stdin);
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i)
		scanf("%d", &w[i]);
	for (int i = 0; i < n - 1; ++i) {
		int u, v;
		scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}
	init();
	scanf("%d", &q);
	while (q--) { //每次询问求出树中与v距离不超过k的所有点的权值和，复杂度O(k^2)
		int v, k;
		scanf("%d %d", &v, &k);
		long long ans = travel(v, k);
		int last = v;
		for (int u = father[v], t = k - 1; u != 0 && t >= 0; u = father[u], --t) {
			ans += travel(u, t) - travel(last, t - 1);
			last = u;
		}
		printf("%lld\n", ans);
	}
	return 0;
}
