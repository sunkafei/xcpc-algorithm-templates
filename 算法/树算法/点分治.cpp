#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#define dfs1(x, fa, d)
#define dfs2(x, fa, d)
using namespace std;
const int maxn = 110000;
vector<pair<int, int>> G[maxn]; //to, weight
int vis[maxn], siz[maxn], f[maxn], pos, sum;
void init(int n) {
	for (int i = 0; i <= n; ++i) {
		vis[i] = false;
		G[i].clear();
	}
}
void getroot(int x, int fa) {
    f[x] = 0; siz[x] = 1;
    for (auto[y, w] : G[x]) if (y != fa && !vis[y]) {
        getroot(y, x); 
        f[x] = max(f[x], siz[y]);
        siz[x] += siz[y];
    }
    f[x] = max(f[x], sum - siz[x]);
    if (f[x] < f[pos])
        pos = x;
}
void calc(int x) { //统计经过结点x的所有答案
    for (auto [y, w] : G[x]) if (!vis[y]) {
        //注意在dfs中枚举子结点y的时候要判两个条件：y != fa && !vis[y]
        dfs1(y, x, w); //计算当前子树与之前子树以及 结点x 连接构成的路径。
        dfs2(y, x, w); //维护处理过的子树的信息
    }
    //维护信息的时候有两种方案：
    //1. 在每次calc调用的时候新建一个数据结构维护。
    //2. 维护一个全局的数据结构，此时在calc末尾应当加一个循环来撤销之前的所有操作。
}
void solve(int x, int cnt) { //cnt为子树x中的结点总数
    pos = maxn - 1; f[pos] = sum = cnt;
    getroot(x, -1);
    int root = pos; //因为pos是全局变量，递归的时候值会改变，所以此处存为局部变量。
    vis[root] = 1;
    calc(root);
    for (auto [y, w] : G[root]) if (!vis[y])
        solve(y, siz[y] < siz[root] ? siz[y] : cnt - siz[root]);
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
    solve(1, n);
	return 0;
}