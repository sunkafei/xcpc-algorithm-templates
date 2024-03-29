#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
char T[] = "aaababa", P[] = "aa";
const int maxn = 10000;
int nxt[maxn], extend[maxn];
//注意：字符串T与字符串P必须以不同的特殊字符结尾（该特殊字符不算在字符串内，故不能算入字符串的长度里）
void getnext(char P[])
{
	int po = 1, m = strlen(P);
	nxt[0] = m;
	nxt[1] = mismatch(P + 1, P + m, P).second - P;
	for (int i = 2; i < m; ++i)
	{
		if (nxt[i - po] + i < nxt[po] + po)
			nxt[i] = nxt[i - po];
		else 
		{
			int j = max(nxt[po] + po - i, 0);
			nxt[i] = mismatch(P + j + i, P + m, P + j).second - P;
			po = i;
		}
	}
}
void exkmp(char T[], char P[])
{
	int po = 0, n = strlen(T), m = strlen(P);
	P[m] = 0; T[n] = 1;
	extend[0] = mismatch(T, T + n, P).second - P;
	getnext(P); //注意：若字符串P最后的特殊字符不是0，则字符串的长度会加1，next[0]会与实际不符。
	for (int i = 1; i < n; i++)
	{
		if (nxt[i - po] + i < extend[po] + po)
			extend[i] = nxt[i - po];
		else
		{
			int j = max(extend[po] + po - i, 0);
			extend[i] = mismatch(T + i + j, T + n, P + j).second - P;
			po = i;
		}
	}
}
int main()
{
	exkmp(T, P);
	for (int i = 0; i < 15; ++i)
		printf("%d\n", extend[i]);
	return 0;
}