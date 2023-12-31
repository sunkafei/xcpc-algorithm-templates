//注意：异或运算符的优先级小于比较运算符！！！
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 1000;
int sg[maxn], vis[maxn];
void getSG()
{
	memset(sg, 0, sizeof(sg));
	for (int i = 1; i < maxn; ++i)
	{
		memset(vis, 0, sizeof(vis));
		for (int j = 1; j <= i; ++j)
			vis[sg[i - j]] = true;
		for (int j = 0; j < maxn; ++j)
		{
			if (!vis[j])
			{
				sg[i] = j;
				break;
			}
		}
	}
}
int main()
{
	getSG();
	printf("%d\n", sg[5]);
	return 0;
}