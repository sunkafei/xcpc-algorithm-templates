#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <queue>
#include <ctime>
using namespace std;
const int maxn = 1100;
const int maxedges = 51000;
const int inf = 1 << 30;
struct edge {
	int to, flow;
	edge *next, *pair;
	edge() {}
	edge(int to, int flow, edge *next) : to(to), flow(flow), next(next) {}
	void *operator new(unsigned, void *p) { return p; }
};
struct ISAP {
	int gap[maxn], h[maxn], n, s, t;
	edge *cur[maxn], *first[maxn], edges[maxedges * 2], *ptr;
	void init(int n) {
		this->n = n;
		ptr = edges;
		memset(first, 0, sizeof(first));
		memset(gap, 0, sizeof(gap));
		memset(h, 0, sizeof(h));
		gap[0] = n;
	}
	void add_edge(int from, int to, int cap) {
		first[from] = new(ptr++)edge(to, cap, first[from]);
		first[to] = new(ptr++)edge(from, 0, first[to]);
		first[from]->pair = first[to];
		first[to]->pair = first[from];
	}
	int augment(int x, int limit) {
		if (x == t)
			return limit;
		int rest = limit;
		for (edge*& e = cur[x]; e; e = e->next) if (e->flow && h[e->to] + 1 == h[x]) {
			int d = augment(e->to, min(rest, e->flow));
			e->flow -= d, e->pair->flow += d, rest -= d;
			if (h[s] == n || !rest)
				return limit - rest;
		}
		int minh = n;
		for (edge *e = cur[x] = first[x]; e; e = e->next) if (e->flow)
			minh = min(minh, h[e->to] + 1);
		if (--gap[h[x]] == 0)
			h[s] = n;
		else
			++gap[h[x] = minh];
		return limit - rest;
	}
	int solve(int s, int t, int limit = inf) {
		this->s = s; this->t = t;
		memcpy(cur, first, sizeof(first)); // memcpy!
		int flow = 0;
		while (h[s] < n && flow < limit)
			flow += augment(s, limit - flow);
		return flow;
	}
}isap;
int main()
{
	freopen("D:\\in.txt", "r", stdin);
	int n, m;
	scanf("%d %d", &n, &m);
	isap.init(n + 5);
	while (m--)
	{
		int s, t, u;
		scanf("%d %d %d", &s, &t, &u);
		isap.add_edge(s, t, u);
	}
	auto start = clock();
	printf("%d\n", isap.solve(1, n));
	double tot = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
	printf("Isap: %f\n", tot);
	return 0;
}