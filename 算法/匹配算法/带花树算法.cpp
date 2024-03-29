#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;
const int maxn = 1005;
struct Blossom {
    int mate[maxn], nxt[maxn], pa[maxn], st[maxn], vis[maxn], t, n;
    int ban[maxn];
    vector<int> G[maxn];
    queue<int> Q;
    void init(int n) { //编号从0开始
        this->n = n;
        this->t = 0;
        memset(mate, -1, sizeof(mate));
        memset(ban, 0, sizeof(ban));
        for (int i = 0; i < n; ++i)
            G[i].clear();
    }
    inline void add_edge(int x, int y) { //添加双向边(x, y)
        G[x].push_back(y);
        G[y].push_back(x);
    }
    inline int find(int x) {
        return pa[x] == x ? x : pa[x] = find(pa[x]);
    }
    inline void merge(int a, int b) {
        pa[find(a)] = find(b);
    }
    int lca(int x, int y) {
        for (t++;;swap(x, y)) if (~x) {
            if (vis[x = find(x)] == t)
                return x;
            vis[x] = t;
            x = ~mate[x] ? nxt[mate[x]] : -1;
        }
    }
    void group(int a, int p) {
        for (int b, c; a != p; merge(a, b), merge(b, c), a = c) {
            b = mate[a], c = nxt[b];
            if (find(c) != p)
                nxt[c] = b;
            if (st[b] == 2)
                st[b] = 1, Q.push(b);
            if (st[c] == 2)
                st[c] = 1, Q.push(c);
        }
    }
    void augment(int s) {
        for (int i = 0; i < n; ++i)
            nxt[i] = vis[i] = -1, pa[i] = i, st[i] = 0;
        Q = queue<int>();
        Q.push(s);
        st[s] = 1;
        while (mate[s] == -1 && !Q.empty()) {
            int x = Q.front();Q.pop();
            for (auto y : G[x]) {
                if (!ban[y] && y != mate[x] && find(x) != find(y) && st[y] != 2) {
                    if (st[y] == 1) {
                        int p = lca(x, y);
                        if (find(x) != p)
                            nxt[x] = y;
                        if (find(y) != p)
                            nxt[y] = x;
                        group(x, p);
                        group(y, p);
                    }
                    else if (mate[y] == -1) {
                        nxt[y] = x;
                        for (int j = y, k, i; ~j; j = i)
                            k = nxt[j], i = mate[k], mate[j] = k, mate[k] = j;
                        break;
                    }
                    else
                        nxt[y] = x, Q.push(mate[y]), st[mate[y]] = 1, st[y] = 2;
                }
            }
        }
    }
    void solve() { //求最大匹配
        for (int i = 0; i < n; ++i) if (mate[i] == -1) //从所有的点开始增广
            augment(i);
        int ans = 0;
        for (int i = 0; i < n; ++i) if (mate[i] != -1)
            ans++;
        printf("%d\n", ans / 2); //匹配数
        for (int i = 0; i < n; ++i) //打印每个点的匹配点，-1表示未匹配
            printf("%d ", mate[i]);
        printf("\n");
    }
    vector<int> unnecessary() { //求出所有的非必要匹配点（存在一个最大匹配不包含这个点）
        for (int i = 0; i < n; ++i) if (mate[i] == -1)//先求出最大匹配
            augment(i);
        vector<int> ret;
        for (int x = 0; x < n; ++x) {
            if (mate[x] == -1)
                ret.push_back(x);
            else {
                int y = mate[x];
                mate[y] = -1;
                mate[x] = -1;
                ban[x] = 1;
                augment(y);
                ban[x] = 0;
                if (mate[y] != -1)
                    ret.push_back(x);
                else
                    augment(y);
            }
        }
        return ret;
    }
}mp;
int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    mp.init(n);
    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y); --x; --y;
        mp.add_edge(x, y);
    }
    mp.solve();
    return 0;
}