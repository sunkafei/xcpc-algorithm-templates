#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define lowbit(x) ((x)&(-x))
namespace Fenwick_Tree
{
	const int maxn = 110000;
	int C[maxn], n;
	void add(int x, int d)
	{
		for (int i = x; i <= n; i += lowbit(i))
			C[i] += d;
	}
	int sum(int x)
	{
		int ret = 0;
		for (int i = x; i > 0; i -= lowbit(i))
			ret += C[i];
		return ret;
	}
}
namespace Fenwick_Tree_2D
{
	const int maxn = 1100;
	int C[maxn][maxn], n, m;
	void add(int x, int y, int d)
	{
		for (int i = x; i <= n; i += lowbit(i))
			for (int j = y; j <= m; j += lowbit(j))
				C[i][j] += d;
	}
	int sum(int x, int y)
	{
		int ret = 0;
		for (int i = x; i > 0; i -= lowbit(i))
			for (int j = y; j > 0; j -= lowbit(j))
				ret += C[i][j];
		return ret;
	}
}
int main()
{
	return 0;
}