//phi(n) 表示小于n且与n互素的整数个数
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
const int maxn = 1000000;
int vis[maxn], prime[maxn], phi[maxn], cnt;
void init() {
	memset(vis, 0, sizeof(vis));
	phi[1] = 1;
	cnt = 0;
	for (int i = 2; i < maxn; i++) {
		if (!vis[i]) {
			prime[cnt++] = i;
			phi[i] = i - 1;
		}
		for (int j = 0; j < cnt && i * prime[j] < maxn; j++) {
			int t = i * prime[j];
			vis[t] = 1;
			if (i % prime[j] == 0) {
				phi[t] = phi[i] * prime[j];
				break;
			}
			else {
				phi[t] = phi[i] * phi[prime[j]];
			}
		}
	}
}
int euler(int n) { //时间复杂度O(sqrt(n))
	int ans = n;
	for (int i = 2; i * i <= n; ++i) if (n % i == 0) {
		ans = ans / i * (i - 1);
		while (n % i == 0)
			n /= i;
	}
	if (n > 1) 
		ans = ans / n * (n - 1);
	return ans;
}
int main() {
	init();
	for (int i = 0; i <= 10000; ++i) if (phi[i] != euler(i))
		printf("%d\n", i);
	return 0;
}