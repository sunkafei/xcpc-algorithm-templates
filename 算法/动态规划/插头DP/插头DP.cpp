#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int INF = 100000000;
int nrows, ncols;
int G[10][10];
// 插头编号：0表示无插头，1表示和数字2连通，2表示和数字3连通
struct State {
    int up[9]; // up[i](0<=i<m)表示第i列处轮廓线上方的插头编号
    int left; // 当前格（即下一个要放置的方格）左侧的插头
    // 三进制编码
    int encode() const {
        int key = left;
        for (int i = 0; i < ncols; i++)
            key = key * 3 + up[i];
        return key;
    }
    // 在(row,col)处放一个新方格。UDLR分别为该方格上下左右四个边界上的插头编号
    // 产生的新状态存放在T里，成功返回true，失败返回false
    bool next(int row, int col, int U, int D, int L, int R, State& T) const {
        if (row == nrows - 1 && D != 0) return false; // 最下行下方不能有插头
        if (col == ncols - 1 && R != 0) return false; // 最右列右边不能有插头
        int must_left = (col > 0 && left != 0); // 是否必须要有左插头
        int must_up = (row > 0 && up[col] != 0); // 是否必须要有上插头
        if ((must_left && L != left) || (!must_left && L != 0)) return false; // 左插头不匹配
        if ((must_up && U != up[col]) || (!must_up && U != 0)) return false; // 上插头不匹配
        if (must_left && must_up && left != up[col]) return false; // 若左插头和上插头都存在，二者必须匹配
        // 产生新状态。实际上只有当前列的下插头和left插头有变化
        for (int i = 0; i < ncols; i++) T.up[i] = up[i];
        T.up[col] = D;
        T.left = R;
        return true;
    }
};
int memo[9][9][59049]; // 3^10
// 当前要放置格子(row, col)，状态为S。返回最小总长度
int rec(int row, int col, const State& S) {
    if (col == ncols) { col = 0; row++; }
    if (row == nrows) return 0;
    int key = S.encode();
    int& res = memo[row][col][key];
    if (res >= 0) return res;
    res = INF;
    State T;
    if (G[row][col] <= 1) { // 空格（0）或者障碍格（1）
        if (S.next(row, col, 0, 0, 0, 0, T)) res = min(res, rec(row, col + 1, T)); // 整个格子里都不连线
        if (G[row][col] == 0) // 如果是空格，可以连线。由于线不能分叉，所以这条线一定连接格子的某两个边界（6种情况）
            for (int t = 1; t <= 2; t++) { // 枚举线的种类。t=1表示2线，t=2表示3线
                if (S.next(row, col, t, t, 0, 0, T)) res = min(res, rec(row, col + 1, T) + 2); // 上<->下
                if (S.next(row, col, t, 0, t, 0, T)) res = min(res, rec(row, col + 1, T) + 2); // 上<->左
                if (S.next(row, col, t, 0, 0, t, T)) res = min(res, rec(row, col + 1, T) + 2); // 上<->右
                if (S.next(row, col, 0, t, t, 0, T)) res = min(res, rec(row, col + 1, T) + 2); // 下<->左
                if (S.next(row, col, 0, t, 0, t, T)) res = min(res, rec(row, col + 1, T) + 2); // 下<->右
                if (S.next(row, col, 0, 0, t, t, T)) res = min(res, rec(row, col + 1, T) + 2); // 左<->右
            }
    }
    else {
        int t = G[row][col] - 1; // 数字为2和3，但插头类型是1和2，所以要减1
        // 由于线不能分叉，所以这条线一定连接格子中间的数字和某一个边界（4种情况）
        if (S.next(row, col, t, 0, 0, 0, T)) res = min(res, rec(row, col + 1, T) + 1); // 从上边界出来
        if (S.next(row, col, 0, t, 0, 0, T)) res = min(res, rec(row, col + 1, T) + 1); // 从下边界出来
        if (S.next(row, col, 0, 0, t, 0, T)) res = min(res, rec(row, col + 1, T) + 1); // 从左边界出来
        if (S.next(row, col, 0, 0, 0, t, T)) res = min(res, rec(row, col + 1, T) + 1); // 从右边界出来
    }
    return res;
}
int main() {
    while (scanf("%d%d", &nrows, &ncols) == 2 && nrows && ncols) {
        for (int i = 0; i < nrows; i++)
            for (int j = 0; j < ncols; j++)
                scanf("%d", &G[i][j]);
        State S;
        memset(&S, 0, sizeof(S));
        memset(memo, -1, sizeof(memo));
        int ans = rec(0, 0, S);
        if (ans == INF) ans = 0;
        printf("%d\n", ans / 2);
    }
    return 0;
}
/*
    void normalize() {
        int rep[maxn] = {}, num = 0;
        for (int i = 0; i < m; ++i) if (cp[i] > 0) {
            if (rep[cp[i]] <= 0)
                rep[cp[i]] = ++num;
            cp[i] = rep[cp[i]];
        }
        if (left > 0) {
            if (rep[left] <= 0) 
                rep[left] = ++num;
            left = rep[left];
        }
    }
    // 把所有编号为b的连通分量改成a
    void merge(int a, int b) {
        if (a == b) return;
        for (int i = 0; i < ncols; i++)
            if (comp[i] == b) comp[i] = a;
    }
*/