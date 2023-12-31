#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 210000;
const unsigned long long x = 123;
unsigned long long H[maxn], xp[maxn];
char s[maxn];
int n;
void init()
{
	n = strlen(s);
	H[n] = 0;
	for (int i = n - 1; i >= 0; --i)
		H[i] = H[i + 1] * x + s[i] - 'a' + 1;
	xp[0] = 1;
	for (int i = 1; i <= n; ++i)
		xp[i] = xp[i - 1] * x;
}
unsigned long long Hash(int i, int L)
{
	return H[i] - H[i + L] * xp[L];
}
int main()
{
	strcpy(s, "abcdabcfg");
	init();
	cout << Hash(0, 3) << endl;
	cout << Hash(4, 3) << endl;
	cout << Hash(1, 3) << endl;
	return 0;
}