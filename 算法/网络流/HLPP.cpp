#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
#include <ctime>
using namespace std;
const int maxn = 2e5 + 5, maxedges = 4e6 + 5, inf = 0x3f3f3f3f;
int n, m, s, t, tot;
int v[maxedges * 2], w[maxedges * 2], first[maxn], nxt[maxedges * 2];
int h[maxn], e[maxn], gap[maxn * 2], inq[maxn];//节点高度是可以到达2n-1的
struct cmp{
	inline bool operator()(int a, int b) const {
		return h[a] < h[b];//因为在优先队列中的节点高度不会改变，所以可以直接比较
	}
};
queue<int> Q;
priority_queue<int, vector<int>, cmp> heap;
inline void add_edge(int from, int to, int flow) {
	tot += 2;
	v[tot + 1] = from; v[tot] = to; w[tot] = flow; w[tot + 1] = 0;
	nxt[tot] = first[from]; first[from] = tot;
	nxt[tot + 1] = first[to]; first[to] = tot + 1;
	return;
}
inline bool bfs() {
	memset(h + 1, 0x3f, sizeof(int) * n);
	h[t] = 0; 
	Q.push(t);
	while (!Q.empty())
	{
		int now = Q.front(); Q.pop();
		for (int go = first[now]; go; go = nxt[go])
			if (w[go ^ 1] && h[v[go]] > h[now] + 1)
				h[v[go]] = h[now] + 1, Q.push(v[go]);
	}
	return h[s] != inf;
}
inline void push(int now) {
	for (int go = first[now]; go; go = nxt[go]) {
		if (w[go] && h[v[go]] + 1 == h[now]) {
			int d = min(e[now], w[go]);
			w[go] -= d; w[go ^ 1] += d; e[now] -= d; e[v[go]] += d;
			if (v[go] != s && v[go] != t && !inq[v[go]])
				heap.push(v[go]), inq[v[go]] = 1;
			if (!e[now])//已经推送完毕可以直接退出
				break;
		}
	}
}
inline void relabel(int now) {
	h[now] = inf;
	for (int go = first[now]; go; go = nxt[go])
		if (w[go] && h[v[go]] + 1 < h[now])
			h[now] = h[v[go]] + 1;
	return;
}
inline int hlpp() {
	int now, d;
	if (!bfs()) //s和t不连通
		return 0;
	h[s] = n;
	memset(gap, 0, sizeof(int) * (n * 2));
	for (int i = 1; i <= n; i++)
		if (h[i] < inf)
			++gap[h[i]];
	for (int go = first[s]; go; go = nxt[go]) {
		if (d = w[go]) {
			w[go] -= d; w[go ^ 1] += d; e[s] -= d; e[v[go]] += d;
			if (v[go] != s && v[go] != t && !inq[v[go]])
				heap.push(v[go]), inq[v[go]] = 1;
		}
	}
	while (!heap.empty()) {
		inq[now = heap.top()] = 0; heap.pop(); push(now);
		if (e[now]) {
			if (!--gap[h[now]]) //gap优化，因为当前节点是最高的所以修改的节点一定不在优先队列中，不必担心修改对优先队列会造成影响
				for (int i = 1; i <= n; i++)
					if (i != s && i != t && h[i] > h[now] && h[i] < n + 1)
						h[i] = n + 1;
			relabel(now); ++gap[h[now]];
			heap.push(now); inq[now] = 1;
		}
	}
	return e[t];
}
int main()
{
	freopen("D:\\in.txt", "r", stdin);
	scanf("%d %d", &n, &m); s = 1; t = n;
	while (m--)
	{
		int s, t, u;
		scanf("%d %d %d", &s, &t, &u);
		add_edge(s, t, u);
	}
	auto start = clock();
	printf("%d\n", hlpp());
	double tot = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
	printf("HLPP: %f\n", tot);
	return 0;
}