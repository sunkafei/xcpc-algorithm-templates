/*
seq下标从1开始，表示欧拉序列。欧拉序列记录的是点的标号，一段欧拉序列中若一个点出现了两次则不予处理。
树上的一条路径一定对应一段连续的欧拉序列（可能还要并上一个额外的点）。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 210000;
const int maxlog = 25;
int head[maxn], nxt[maxn], to[maxn], seq[maxn], depth[maxn], anc[maxn][maxlog], first[maxn], last[maxn], cur, sz;
struct item { //下标区间[L, R]的欧拉序列，若lca != 0则还要将lca考虑进来
	int L, R, lca;
	item(){}
	item(int L, int R, int lca) : L(L), R(R), lca(lca){}
};
void init() {
	cur = sz = 0;
	memset(head, 0, sizeof(head));
	memset(depth, 0, sizeof(depth));
	memset(anc, 0, sizeof(anc));
}
void add_edge(int x, int y) { //对于树中的每条边要调用两次add_edge
	to[++cur] = y;
	nxt[cur] = head[x];
	head[x] = cur;
}
void dfs(int x, int fa) { //dfs(1, 0)
	seq[++sz] = x;
	first[x] = sz;
	for (int i = head[x]; i; i = nxt[i]) if (to[i] != fa) {
		int y = to[i];
		depth[y] = depth[x] + 1;
		anc[y][0] = x;
		for (int i = 1; (1 << i) <= depth[y]; ++i) 
			anc[y][i] = anc[anc[y][i - 1]][i - 1];
		dfs(y, x);
	}
	seq[++sz] = x;
	last[x] = sz;
}
int getlca(int x, int y) {
	if (depth[x] < depth[y])
		swap(x, y);
	for (int i = maxlog - 1; i >= 0; --i)
		if (depth[x] - (1 << i) >= depth[y])
			x = anc[x][i];
	if (x == y)
		return x;
	for (int i = maxlog - 1; i >= 0; --i)
		if (anc[x][i] != anc[y][i]) 
			x = anc[x][i], y = anc[y][i];
	return anc[x][0];
}
item path(int x, int y) { //返回树上x到y的路径对应的欧拉序列
	if (first[x] > first[y])
		swap(x, y);
	int lca = getlca(x, y);
	if (lca == x)
		return item(first[x], first[y], 0);
	else
		return item(last[x], first[y], lca);
}
int main() {

	return 0;
}