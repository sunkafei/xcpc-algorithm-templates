/*
P2056 [ZJOI2007]捉迷藏
动态维护树中白色结点的最长距离。
操作C翻转一个结点的颜色，操作G进行一次查询（树中白色结点的直径）。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <tuple>
using namespace std;
const int maxn = 101000;
template<typename T> class heap {
private:
    priority_queue<T> Q, R;
public:
    void maintain() {
        while (!R.empty() && Q.top() == R.top())
            Q.pop(), R.pop();
    }
    void push(const T& val) {
        Q.push(val);
    }
    void erase(const T& val) { //只能删除还在优先队列中的值，不能删除不存在的值。
        R.push(val);
        maintain();
    }
    void pop() {
        Q.pop();
        maintain();
    }
    T top() {
        return Q.top();
    }
    T top2() { //返回第二大的值，调用之前必须保证有size() >= 2。
        auto val = Q.top(); pop();
        auto sec = Q.top(); push(val);
        return sec;
    }
    auto size() const {
        return Q.size() - R.size();
    }
    bool empty() const {
        return size() == 0;
    }
    template<typename ...F> void emplace(F&&... args) {
        Q.emplace(std::forward<F>(args)...);
    }
};
vector<int> G[maxn]; //to, weight
int vis[maxn], siz[maxn], f[maxn], pa[maxn], pos, sum; //pa[x]表示在点分树上结点x的父亲
int state[maxn], res[maxn], rt, n;
int son[maxn], dep[maxn], father[maxn], top[maxn];
heap<int> head[maxn], Q[maxn], answer;
void DFS1(int x, int fa, int d) {
    dep[x] = d;
    siz[x] = 1;
    son[x] = 0;
    father[x] = fa;
    for (auto y : G[x]) if (y != fa) {
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
    for (auto y : G[x])
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
void getroot(int x, int fa) {
    f[x] = 0; siz[x] = 1;
    for (auto y : G[x]) if (y != fa && !vis[y]) {
        getroot(y, x);
        f[x] = max(f[x], siz[y]);
        siz[x] += siz[y];
    }
    f[x] = max(f[x], sum - siz[x]);
    if (f[x] < f[pos])
        pos = x;
}
void dfs(int x, int fa) {
    Q[rt].push(dist(x, pa[rt]));
    for (auto y : G[x]) if (y != fa && !vis[y]) {
        dfs(y, x);
    }
}
void prepare(int x) {
    head[x].push(0);
    res[x] = (head[x].size() == 1 ? 0 : head[x].top() + head[x].top2());
    answer.emplace(res[x]);
    if (pa[x] < 0) return;
    Q[x].push(dist(x, pa[x]));
    for (auto y : G[x]) if (!vis[y]) {
        ::rt = x; 
        dfs(y, x);
    }
    head[pa[x]].push(Q[x].top());
}
void update(int x) {
    state[x] ^= 1;
    for (int y = -1, p = x; p >= 1; y = p, p = pa[y]) {
        if (y == -1) {
            if (state[x] == 0)
                head[x].push(0);
            else
                head[x].erase(0);
        }
        else {
            auto& q = Q[y];
            if (q.size())
                head[p].erase(q.top());
            if (state[x] == 0)
                q.emplace(dist(x, p));
            else
                q.erase(dist(x, p));
            if (q.size())
                head[p].push(q.top());
        }
        int last = res[p];
        if (head[p].size() >= 2)
            res[p] = head[p].top() + head[p].top2();
        else if (head[p].size() >= 1)
            res[p] = (state[p] == 0 ? head[p].top() : 0);
        else
            res[p] = -1;
        if (last != res[p]) {
            answer.erase(last);
            answer.emplace(res[p]);
        }
    }
}
void solve(int x, int cnt, int pre = -1) { //cnt为子树x中的结点总数
    pos = maxn - 1; f[pos] = sum = cnt;
    getroot(x, -1);
    int root = pos; //因为pos是全局变量，递归的时候值会改变，所以此处存为局部变量。
    vis[root] = true;
    pa[root] = pre;
    for (auto y : G[root]) if (!vis[y]) {
        int total = siz[y] < siz[root] ? siz[y] : cnt - siz[root];
        solve(y, total, root);
    }
    vis[root] = false;
    prepare(root);
}
int main() {
    //freopen("in.txt", "r", stdin);
    scanf("%d", &n);
    for (int i = 1; i < n; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        G[x].emplace_back(y);
        G[y].emplace_back(x);
    }
    DFS1(1, 0, 1);
    DFS2(1, 1);
    solve(1, n);
    int Q, x; scanf("%d", &Q);
    while (Q--) {
        char tp; scanf(" %c ", &tp);
        if (tp == 'G') {
            printf("%d\n", answer.top());
        }
        else {
            scanf("%d", &x);
            update(x);
        }
    }
    return 0;
}