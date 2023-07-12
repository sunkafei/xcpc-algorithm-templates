/*
1.	rank[i]表示下标i的排名（排名从0开始）。
2.	sa[i]表示第i小的后缀的下标（i从0开始）。
3.	height[i]表示sa[i - 1]与sa[i]的最长公共前缀。
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
const int maxn = 210000; //maxn应当开到最大字符串长度的两倍，否则(1)处下标访问可能越界。
const int maxlog = 20;
struct Suffix_Array {
	char s[maxn];
	int sa[maxn], rank[maxn], height[maxn];
	int t[maxn], t2[maxn], c[maxn], n;
	void init(const char* str) {
		strcpy(s, str);
		n = strlen(s);
		memset(t, 0, sizeof(int) * (2 * n + 10));  //为了保证(1)处访问越界时得到的数组值恒为0，应当将t和t2数组清空
		memset(t2, 0, sizeof(int) * (2 * n + 10));
	}
	void build_sa(int m = 256) {
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
				x[sa[i]] = y[sa[i - 1]] == y[sa[i]] && y[sa[i - 1] + k] == y[sa[i] + k] ? p - 1 : p++; //(1)
			if (p >= n) break;
			m = p;
		}
	}
	void getheight() {
		int k = 0;
		for (int i = 0; i < n; ++i) rank[sa[i]] = i;
		for (int i = 0; i < n; ++i) if (rank[i] > 0) {
			if (k) k--;
			int j = sa[rank[i] - 1];
			while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
			height[rank[i]] = k;
		}
	}
	int d[maxn][maxlog], log[maxn];
	void RMQ_init() {
		log[0] = -1;
		for (int i = 1; i <= n; ++i)
			log[i] = log[i / 2] + 1;
		for (int i = 0; i < n; ++i)
			d[i][0] = height[i];
		for (int j = 1; j <= log[n]; ++j)
			for (int i = 0; i + (1 << j) - 1 < n; ++i)
				d[i][j] = min(d[i][j - 1], d[i + (1 << (j - 1))][j - 1]);
	}
	int lcp(int i, int j) { //返回下标i开始的后缀与下标j开始的后缀的最长公共前缀。
		if (i == j)
			return n - i;
		if (rank[i] > rank[j])
			swap(i, j);
		int x = rank[i] + 1, y = rank[j];
		int k = log[y - x + 1];
		return min(d[x][k], d[y - (1 << k) + 1][k]);
	}
	pair<int, int> locate(int l, int r) {
		//返回一个最长的区间[L, R]使得sa中下标从L到R的所有后缀都以s[l, r]为前缀。
		int pos = rank[l], length = r - l + 1;
		int L = 0, R = pos;
		while (L < R) {
			int M = L + (R - L) / 2;
			if (lcp(l, sa[M]) >= length) R = M;
			else L = M + 1;
		}
		int tmp = L;
		L = pos, R = n - 1;
		while (L < R) {
			int M = L + (R - L + 1) / 2;
			if (lcp(l, sa[M]) >= length) L = M;
			else R = M - 1;
		}
		return make_pair(tmp, L);
	}
	pair<int, int> locate(vector<pair<int, int>> ranges) {
		//将ranges中的所有下标区间对应的子串拼接到一起，得到字符串T，
		//返回一个最长的区间[L, R]使得sa中下标从L到R的所有后缀都以T为前缀，
		//无解返回{ 0, -1 }。
		int l = 0, r = n - 1, pos = 0;
		for (auto [x, y] : ranges) {
			int L = l, R = r, len = y - x + 1;
			while (L < R) {
				int M = L + (R - L) / 2;
				int pre = sa[M] + pos < n ? lcp(sa[M] + pos, x) : 0;
				int ch1 = s[sa[M] + pos + pre], ch2 = s[x + pre];
				if (pre >= len || ch1 > ch2) R = M;
				else L = M + 1;
			}
			int left = L;
			L = l, R = r;
			while (L < R) {
				int M = L + (R - L + 1) / 2;
				int pre = sa[M] + pos < n ? lcp(sa[M] + pos, x) : 0;
				int ch1 = s[sa[M] + pos + pre], ch2 = s[x + pre];
				if (pre >= len || ch1 < ch2) L = M;
				else R = M - 1;
			}
			int right = L;
			if (sa[left] + pos > n || lcp(sa[left] + pos, x) < len)
				return make_pair(0, -1);
			else
				l = left, r = right;
			pos += len;
		}
		return make_pair(l, r);
	}
	pair<int, int> go(int x, int y, int h, char c) {
		//设sa数组中区间[x, y]的最长公共前缀至少为h，则返回区间中的后缀i满足s[i + h] == c
		//若要识别s中所有以c开头的后缀，则调用go(0, n - 1, 0, c)
		int L = x, R = y;
		while (L < R) {
			int M = L + (R - L) / 2;
			if (sa[M] + h < n && s[sa[M] + h] >= c) R = M;
			else L = M + 1;
		}
		int tmp = L;
		R = y;
		while (L < R) {
			int M = L + (R - L + 1) / 2;
			if (s[sa[M] + h] == c) L = M;
			else R = M - 1;
		}
		if (sa[L] + h >= n || s[sa[L] + h] != c)
			return { 0, -1 }; //空区间
		return { tmp, L };
	}
	pair<int, int> go(int x, int y, int h, const string &str) {
		//设sa数组中区间[x, y]的最长公共前缀至少为h，则返回区间中的后缀i满足s[i + h + j] == str[j]
		//若要识别s中所有以str开头的后缀，则调用go(0, n - 1, 0, str)
		for (auto c : str) {
			int L = x, R = y;
			while (L < R) {
				int M = L + (R - L) / 2;
				if (sa[M] + h < n && s[sa[M] + h] >= c) R = M;
				else L = M + 1;
			}
			x = L, R = y;
			while (L < R) {
				int M = L + (R - L + 1) / 2;
				if (s[sa[M] + h] == c) L = M;
				else R = M - 1;
			}
			if (sa[L] + h >= n || s[sa[L] + h] != c)
				return { 0, -1 }; //空区间
			y = L;
			h += 1;
		}
		return { x, y };
	}
	pair<int, int> go_rev(int x, int y, char c) {
		//设区间[x, y]表示sa中的一个区间，将[x, y]中的每个后缀都往前走一个字符c，
		//得到一个新的连续区间并返回，注意这个新区间不一定是[x, y]的子区间
		//注意：因为sa[0, n-1]不包含空串，所以go(0, n - 1, 0, c) != go_rev(0, n - 1, c)
		int L = 0, R = n - 1;
		while (L < R) {
			int M = L + (R - L) / 2;
			if (s[sa[M]] < c || s[sa[M]] == c && (sa[M] == n - 1 || rank[sa[M] + 1] < x))
				L = M + 1;
			else
				R = M;
		}
		int tmp = L;
		R = n - 1;
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
}arr;
int main() {
	static char s[maxn];
	scanf("%s", s);
	int n = strlen(s), Q;
	scanf("%d", &Q);
	arr.init(s);
	arr.build_sa();
	arr.getheight();
	arr.RMQ_init();
	while (Q--) {
		int x, y, a, b;
		scanf("%d %d %d %d", &x, &y, &a, &b); --x; --y; --a; --b;
		auto [L, R] = arr.locate(vector<pair<int, int>>{make_pair(x, y), make_pair(a, b)});
		printf("%d\n", R - L + 1);
	}
}
