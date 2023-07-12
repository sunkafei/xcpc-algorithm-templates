/*
莫比乌斯函数性质：
sigma mu(d) = [n == 1]
 d|n
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 1000000;
int vis[maxn], prime[maxn], mu[maxn], cnt;
void init()
{
	memset(vis, 0, sizeof(vis));
	mu[1] = 1;
	cnt = 0;
	for (int i = 2; i < maxn; ++i)
	{
		if (!vis[i])
		{
			prime[cnt++] = i;
			mu[i] = -1;
		}
		for (int j = 0; j < cnt && i * prime[j] < maxn; ++j)
		{
			int t = i * prime[j]; 
			vis[t] = 1;
			if (i % prime[j] == 0) 
			{
				mu[t] = 0;
				break;
			}
			else
			{
				mu[t] = -mu[i];
			}
		}
	}
}
int main()
{
	init();
	printf("%d\n", mu[3*7*11*13]);
	return 0;
}
