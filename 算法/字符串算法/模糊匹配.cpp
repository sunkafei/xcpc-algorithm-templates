/*
init(s) 用s作为主串来初始化
query(t) 查询t在s中出现的次数（只要不同字符的个数不超过一个就算匹配）
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
const int maxn = 410000;
int n, sz, root[maxn];
pair<int, int> range[maxn];
struct SuffixArray {
    int sa[maxn], rank[maxn];
    int t[maxn], t2[maxn], c[maxn];
    char s[maxn];
    void build_sa(const char *str, int m = 256) {
        strcpy(s, str);
        memset(t, 0, sizeof(int) * (2 * n + 10));
        memset(t2, 0, sizeof(int) * (2 * n + 10));
        int* x = t, * y = t2;
        for (int i = 0; i < m; ++i) c[i] = 0;
        for (int i = 0; i < n; ++i) c[x[i] = s[i]]++;
        for (int i = 1; i < m; ++i) c[i] += c[i - 1];
        for (int i = n - 1; i >= 0; --i) sa[--c[x[i]]] = i;
        for (int k = 1; k <= n; k <<= 1) {
            int p = 0;
            for (int i = n - 1; i >= n - k; --i) y[p++] = i;
            for (int i = 0; i < n; ++i) if (sa[i] >= k) y[p++] = sa[i] - k;
            for (int i = 0; i < m; ++i) c[i] = 0;
            for (int i = 0; i < n; ++i) c[x[y[i]]]++;
            for (int i = 1; i < m; ++i) c[i] += c[i - 1];
            for (int i = n - 1; i >= 0; --i) sa[--c[x[y[i]]]] = y[i];
            swap(x, y);
            p = 1; x[sa[0]] = 0;
            for (int i = 1; i < n; ++i)
                x[sa[i]] = y[sa[i - 1]] == y[sa[i]] && y[sa[i - 1] + k] == y[sa[i] + k] ? p - 1 : p++;
            if (p >= n) break;
            m = p;
        }
    }
    void calc() { //将空串放在sa数组的最前面，并计算rank数组
        for (int i = n; i >= 1; --i)
            sa[i] = sa[i - 1];
        sa[0] = n;
        for (int i = 0; i <= n; ++i)
            rank[sa[i]] = i;
    }
    pair<int, int> advance(pair<int, int> range, char c) {
        //设区间[x, y]表示sa中的一个区间，返回将[x, y]中的每个后缀都往前走一个字符c得到的区间
        auto [x, y] = range;
        int L = 1, R = n;
        while (L < R) {
            int M = L + (R - L) / 2;
            if (s[sa[M]] < c || s[sa[M]] == c && rank[sa[M] + 1] < x)
                L = M + 1;
            else
                R = M;
        }
        int tmp = L;
        R = n;
        while (L < R) {
            int M = L + (R - L + 1) / 2;
            if (s[sa[M]] > c || s[sa[M]] == c && rank[sa[M] + 1] > y)
                R = M - 1;
            else
                L = M;
        }
        if (s[sa[L]] != c || rank[sa[L] + 1] < x || rank[sa[L] + 1] > y)
            return { 0, -1 }; //空区间
        return { tmp, L };
    }
}pre, suf;
struct node {
    int l, r, v;
} T[maxn * 25];
void ins(int& i, int l, int r, int p) {
    int m = (l + r) >> 1;
    T[++sz] = T[i]; i = sz;
    T[i].v++;
    if (l == r) return;
    if (p <= m) ins(T[i].l, l, m, p);
    else ins(T[i].r, m + 1, r, p);
}
int ask(int x, int y, int v) {
    int l = 0, r = n, k = 0;
    x = root[x - 1], y = root[y];
    int p = v - 1;
    if (p < 0) return 0;
    while (l < r) {
        int m = (l + r) >> 1, t = T[T[y].l].v - T[T[x].l].v;
        if (p <= m)
            x = T[x].l, y = T[y].l, r = m;
        else
            x = T[x].r, y = T[y].r, l = m + 1, k += t;
    }
    k += T[y].v - T[x].v;
    return k;
}
void init(string s) {
    ::sz = 0; ::n = s.size();
    pre.build_sa(s.c_str());
    pre.calc();
    reverse(s.begin(), s.end());
    suf.build_sa(s.c_str());
    suf.calc();
    int tree = 0;
    for (int i = 0; i <= n; ++i) {
        int j = n - suf.sa[i] - 1;
        if (j + 2 <= n)
            ins(tree, 0, n, pre.rank[j + 2]);
        root[i + 1] = tree;
    }
}
long long query(string t) {
    long long ans = 0;
    range[t.size()] = { 0, n };
    for (int i = t.size() - 1; i >= 0; --i) {
        if (range[i + 1].first > range[i + 1].second)
            range[i] = range[i + 1];
        else
            range[i] = pre.advance(range[i + 1], t[i]);
    }
    pair<int, int> now(0, n);
    for (int i = 0; i < t.size() && now.first <= now.second; ++i) {
        if (range[i + 1].first <= range[i + 1].second) {
            ans += ask(now.first + 1, now.second + 1, range[i + 1].second + 1);
            ans -= ask(now.first + 1, now.second + 1, range[i + 1].first);
        }
        now = suf.advance(now, t[i]);
    }
    if (now.first <= now.second)
        ans -= (now.second - now.first + 1) * (t.size() - 1);
    return ans;
}
int main() {
    //freopen("in.txt", "r", stdin);
    int T, Q;
    cin >> T;
    while (T--) {
        string s, t;
        cin >> s;
        init(s);
        cin >> Q;
        while (Q--) {
            cin >> t;
            printf("%lld\n", query(t));
        }
    }
    return 0;
}