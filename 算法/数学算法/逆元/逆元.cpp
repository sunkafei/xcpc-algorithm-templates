#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
const long long maxn = 1000005, mod = 1000000007;

long long pow(long long a, long long n, long long p)
{
	long long ans = 1;
	while (n)
	{
		if (n & 1)
			ans = ans * a % p;
		a = a * a % p;
		n >>= 1;
	}
	return ans;
}
long long inverse1(long long a, long long n)   //费马小定理求逆元
{
	return pow(a, n - 2, n);
}

void extgcd(long long a, long long b, long long& d, long long& x, long long& y) 
{
	if (!b) { d = a; x = 1; y = 0; }
	else { extgcd(b, a % b, d, y, x); y -= x * (a / b); }
}
long long inverse2(long long a, long long n) 
{
	long long d, x, y;
	extgcd(a, n, d, x, y);
	return d == 1 ? (x + n) % n : -1;
}

long long inv[maxn];
void inverse3(long long n, long long p)
{
	inv[1] = 1;
	for (long long i = 2; i <= n; ++i) 
		inv[i] = (p - p / i) * inv[p % i] % p;
}

int main()
{
	int number = 888;
	inverse3(100005, mod);
	printf("%lld %lld %lld\n", inverse1(number, mod), inverse2(number, mod), inv[number]);
	return 0;
}