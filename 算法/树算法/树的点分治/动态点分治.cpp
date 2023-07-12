#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#define dfs(x, fa, d)
using namespace std;
const int maxn = 110000;
vector<pair<int, int>> G[maxn]; //to, weight
int vis[maxn], siz[maxn], f[maxn], pa[maxn], pos, sum; //pa[x]表示在点分树上结点x的父亲
int son[maxn], dep[maxn], father[maxn], top[maxn];
void DFS1(int x, int fa, int d) {
    dep[x] = d;
    siz[x] = 1;
    son[x] = 0;
    father[x] = fa;
    for (auto [y, w] : G[x]) if (y != fa) {
        DFS1(y, x, d + 1);
        siz[x] += siz[y];
        if (siz[son[x]] < siz[y])
            son[x] = y;
    }
}
void DFS2(int x, int tp) {
    top[x] = tp;
    if (son[x])
        DFS2(son[x], tp);
    for (auto [y, w] : G[x])
        if (y != father[x] && y != son[x])
            DFS2(y, y);
}
int lca(int x, int y) {
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]])
            swap(x, y);
        x = father[top[x]];
    }
    return dep[x] < dep[y] ? x : y;
}
int dist(int x, int y) {
    return dep[x] + dep[y] - 2 * dep[lca(x, y)];
}
void init(int n) {
    for (int i = 0; i <= n; ++i) {
        vis[i] = false;
        G[i].clear();
    }
}
void getroot(int x, int fa) {
    f[x] = 0; siz[x] = 1;
    for (auto [y, w] : G[x]) if (y != fa && !vis[y]) {
        getroot(y, x);
        f[x] = max(f[x], siz[y]);
        siz[x] += siz[y];
    }
    f[x] = max(f[x], sum - siz[x]);
    if (f[x] < f[pos])
        pos = x;
}
void prepare(int x) { //pa[x]为结点x在点分树中的父结点，没有的话为-1。
    for (auto [y, w] : G[x]) if (!vis[y]) {
        //注意在dfs中枚举子结点y的时候要判两个条件：y != fa && !vis[y]
        dfs(y, x, w);
    }
}
void solve(int x, int cnt, int pre = -1) { //cnt为子树x中的结点总数
    pos = maxn - 1; f[pos] = sum = cnt;
    getroot(x, -1);
    int root = pos; //因为pos是全局变量，递归的时候值会改变，所以此处存为局部变量。
    vis[root] = true;
    pa[root] = pre;
    for (auto [y, w] : G[root]) if (!vis[y]) {
        int total = siz[y] < siz[root] ? siz[y] : cnt - siz[root];
        solve(y, total, root);
    }
    vis[root] = false;
    prepare(root);
}
int main() {
    int n;
    scanf("%d", &n);
    init(n);
    for (int i = 1; i < n; ++i) {
        int x, y, w;
        scanf("%d %d %d", &x, &y, &w);
        G[x].emplace_back(y, w);
        G[y].emplace_back(x, w);
    }

    DFS1(1, 0, 1);
    DFS2(1, 1);
    solve(1, n);

    return 0;
}