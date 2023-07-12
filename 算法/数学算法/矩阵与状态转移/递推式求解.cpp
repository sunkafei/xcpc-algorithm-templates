#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int mod = 1e9 + 7;
int a[2100], c[2100], tmp[2100], tmpa[2100], res[2100], k;
long long n;
void mul(int* p, int* q, int* c)
{
	memset(tmp, 0, sizeof(tmp));
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			tmp[i + j] = (1ll * p[i] * q[j] + tmp[i + j]) % mod;
	for (int i = 2 * k - 2; i >= k; i--)
		for (int j = 0; j < k; j++)
			tmp[i - j - 1] = (1ll * tmp[i] * c[j] + tmp[i - j - 1]) % mod;
	for (int i = 0; i < k; i++)
		p[i] = tmp[i];
}
int solve(int* a, int* c, long long n) //a为初始值数组，c为系数矩阵
{
	if (n < k) return a[n];
	memset(tmpa, 0, sizeof(tmpa));
	memset(res, 0, sizeof(res));
	tmpa[1] = res[0] = 1;
	while (n)
	{
		if (n & 1)
			mul(res, tmpa, c);
		mul(tmpa, tmpa, c);
		n >>= 1;
	}
	int ans = 0;
	for (int i = 0; i < k; i++)
		ans = (ans + 1ll * a[i] * res[i]) % mod;
	return ans;
}




int pow_mod(int a, long long N) {
	int ans = 1;
	a %= mod;
	while (N) {
		if (N & 1)
			ans = 1ll * ans * a % mod;
		a = 1ll * a * a % mod;
		N >>= 1;
	}
	return ans;
}
int main() {
	int T;
	scanf("%d", &T);
	while (T--) {
		scanf("%d %lld", &k, &n);
		memset(a, 0, sizeof(a));
		if (k == 1) {
			printf("1\n");
			continue;
		}
		if (n == -1) {
			printf("%d\n", 2 * pow_mod(k + 1, mod - 2) % mod);
			continue;
		}
		int rev = pow_mod(k, mod - 2);
		for (int i = 0; i < k; ++i)
			c[i] = rev;
		a[0] = 1;
		for (int i = 1; i < k; ++i) {
			for (int j = i - k; j < i; ++j) if (j >= 0) {
				a[i] += 1ll * rev * a[j] % mod;
				if (a[i] >= mod)a[i] -= mod;
			}
		}
		int ans = solve(a, c, n);
		printf("%d\n", ans);
	}
	return 0;
}