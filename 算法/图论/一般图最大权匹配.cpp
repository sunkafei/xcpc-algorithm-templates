#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#define dist(e) (lab[e.u] + lab[e.v] - g[e.u][e.v].w * 2)
using namespace std;
const int maxn = 1023, inf = 1e9;
struct edge {
	int u, v, w;
} g[maxn][maxn];
int n, m, num, lab[maxn], match[maxn], slack[maxn], st[maxn], pa[maxn];
int from[maxn][maxn], S[maxn], vis[maxn];
vector<int> flower[maxn];
deque<int> q;
void update(int u, int x) {
	if (!slack[x] || dist(g[u][x]) < dist(g[slack[x]][x]))
		slack[x] = u;
}
void set_slack(int x) {
	slack[x] = 0;
	for (int u = 1; u <= n; ++u)
		if (g[u][x].w > 0 && st[u] != x && S[st[u]] == 0)
			update(u, x);
}
void push(int x) {
	if (x <= n)
		return q.push_back(x);
	for (int i = 0; i < flower[x].size(); i++)
		push(flower[x][i]);
}
void set_st(int x, int b) {
	st[x] = b;
	if (x <= n) return;
	for (int i = 0; i < flower[x].size(); ++i)
		set_st(flower[x][i], b);
}
int get(int b, int xr) {
	int pr = find(flower[b].begin(), flower[b].end(), xr) - flower[b].begin();
	if (pr % 2 == 1) {
		reverse(flower[b].begin() + 1, flower[b].end());
		return (int)flower[b].size() - pr;
	}
	else
		return pr;
}
void set_match(int u, int v) {
	match[u] = g[u][v].v;
	if (u <= n) return;
	edge e = g[u][v];
	int xr = from[u][e.u], pr = get(u, xr);
	for (int i = 0; i < pr; ++i)
		set_match(flower[u][i], flower[u][i ^ 1]);
	set_match(xr, v);
	rotate(flower[u].begin(), flower[u].begin() + pr, flower[u].end());
}
void augment(int u, int v) {
	int xnv = st[match[u]];
	set_match(u, v);
	if (!xnv) return;
	set_match(xnv, st[pa[xnv]]);
	augment(st[pa[xnv]], xnv);
}
int get_lca(int u, int v) {
	static int t = 0;
	for (++t; u || v; swap(u, v)) {
		if (u == 0) continue;
		if (vis[u] == t) return u;
		vis[u] = t;
		u = st[match[u]];
		if (u)u = st[pa[u]];
	}
	return 0;
}
void add(int u, int lca, int v) {
	int b = n + 1;
	while (b <= num && st[b]) ++b;
	if (b > num) ++num;
	lab[b] = 0, S[b] = 0;
	match[b] = match[lca];
	flower[b].clear();
	flower[b].push_back(lca);
	for (int x = u, y; x != lca; x = st[pa[y]])
		flower[b].push_back(x), flower[b].push_back(y = st[match[x]]), push(y);
	reverse(flower[b].begin() + 1, flower[b].end());
	for (int x = v, y; x != lca; x = st[pa[y]])
		flower[b].push_back(x), flower[b].push_back(y = st[match[x]]), push(y);
	set_st(b, b);
	for (int x = 1; x <= num; ++x)
		g[b][x].w = g[x][b].w = 0;
	for (int x = 1; x <= n; ++x)
		from[b][x] = 0;
	for (int i = 0; i < flower[b].size(); ++i) {
		int xs = flower[b][i];
		for (int x = 1; x <= num; ++x)
			if (g[b][x].w == 0 || dist(g[xs][x]) < dist(g[b][x]))
				g[b][x] = g[xs][x], g[x][b] = g[x][xs];
		for (int x = 1; x <= n; ++x)
			if (from[xs][x])
				from[b][x] = xs;
	}
	set_slack(b);
}
void expand(int b) {
	for (int i = 0; i < flower[b].size(); ++i)
		set_st(flower[b][i], flower[b][i]);
	int xr = from[b][g[b][pa[b]].u], pr = get(b, xr);
	for (int i = 0; i < pr; i += 2) {
		int xs = flower[b][i], xns = flower[b][i + 1];
		pa[xs] = g[xns][xs].u;
		S[xs] = 1, S[xns] = 0;
		slack[xs] = 0, set_slack(xns);
		push(xns);
	}
	S[xr] = 1, pa[xr] = pa[b];
	for (int i = pr + 1; i < flower[b].size(); ++i) {
		int xs = flower[b][i];
		S[xs] = -1, set_slack(xs);
	}
	st[b] = 0;
}
bool found(const edge& e) {
	int u = st[e.u], v = st[e.v];
	if (S[v] == -1) {
		pa[v] = e.u, S[v] = 1;
		int nu = st[match[v]];
		slack[v] = slack[nu] = 0;
		S[nu] = 0, push(nu);
	}
	else if (S[v] == 0) {
		int lca = get_lca(u, v);
		if (!lca) return augment(u, v), augment(v, u), true;
		else add(u, lca, v);
	}
	return false;
}
bool matching() {
	fill(S, S + num + 1, -1);
	fill(slack, slack + num + 1, 0);
	q.clear();
	for (int x = 1; x <= num; ++x)
		if (st[x] == x && !match[x])pa[x] = 0, S[x] = 0, push(x);
	if (q.empty()) return false;
	for (;;) {
		while (q.size()) {
			int u = q.front();
			q.pop_front();
			if (S[st[u]] == 1) continue;
			for (int v = 1; v <= n; ++v) {
				if (g[u][v].w > 0 && st[u] != st[v]) {
					if (dist(g[u][v]) == 0) {
						if (found(g[u][v]))
							return true;
					}
					else update(u, st[v]);
				}
			}
		}
		int d = inf;
		for (int b = n + 1; b <= num; ++b)
			if (st[b] == b && S[b] == 1)d = min(d, lab[b] / 2);
		for (int x = 1; x <= num; ++x)
			if (st[x] == x && slack[x])
			{
				if (S[x] == -1)d = min(d, dist(g[slack[x]][x]));
				else if (S[x] == 0)d = min(d, dist(g[slack[x]][x]) / 2);
			}
		for (int u = 1; u <= n; ++u) {
			if (S[st[u]] == 0) {
				if (lab[u] <= d)
					return false;
				lab[u] -= d;
			}
			else if (S[st[u]] == 1) lab[u] += d;
		}
		for (int b = n + 1; b <= num; ++b)
			if (st[b] == b) {
				if (S[st[b]] == 0) lab[b] += d * 2;
				else if (S[st[b]] == 1) lab[b] -= d * 2;
			}
		q.clear();
		for (int x = 1; x <= num; ++x)
			if (st[x] == x && slack[x] && st[slack[x]] != x && dist(g[slack[x]][x]) == 0)
				if (found(g[slack[x]][x]))
					return true;
		for (int b = n + 1; b <= num; ++b)
			if (st[b] == b && S[b] == 1 && lab[b] == 0)
				expand(b);
	}
	return false;
}
pair<long long, int> weight_blossom() {
	fill(match, match + n + 1, 0);
	num = n;
	int matches = 0;
	long long tot_weight = 0;
	for (int u = 0; u <= n; ++u) st[u] = u, flower[u].clear();
	int w_max = 0;
	for (int u = 1; u <= n; ++u)
		for (int v = 1; v <= n; ++v) {
			from[u][v] = (u == v ? u : 0);
			w_max = max(w_max, g[u][v].w);
		}
	for (int u = 1; u <= n; ++u) lab[u] = w_max;
	while (matching()) ++matches;
	for (int u = 1; u <= n; ++u)
		if (match[u] && match[u] < u)
			tot_weight += g[u][match[u]].w;
	return make_pair(tot_weight, matches);
}
int main() { //边权必须是正数
	cin >> n >> m;
	for (int u = 1; u <= n; ++u)
		for (int v = 1; v <= n; ++v)
			g[u][v] = edge{ u,v,0 };
	for (int i = 0, u, v, w; i < m; ++i) {
		cin >> u >> v >> w;
		g[u][v].w = g[v][u].w = w;
	}
	cout << weight_blossom().first << '\n';
	for (int u = 1; u <= n; ++u) cout << match[u] << ' ';
}