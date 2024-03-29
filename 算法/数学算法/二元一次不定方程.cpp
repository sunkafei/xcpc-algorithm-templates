#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
void gcd(long long a, long long b, long long& d, long long& x, long long& y) {
	if (!b) {
		d = a;
		x = 1;
		y = 0;
	}
	else {
		gcd(b, a % b, d, y, x);
		y -= x * (a / b);
	}
}
int main() {
	//freopen("in.txt", "r", stdin);
	int T;
	scanf("%d", &T);
	while (T--) {
		long long a, b, c, g, x, y;
		scanf("%lld %lld %lld", &a, &b, &c);
		gcd(a, b, g, x, y);
		if (c % g != 0) { //ax + by = c 无解
			puts("-1");
			continue;
		}
		x = x * c / g, y = y * c / g; //算出一组特解
		long long dx = b / g, dy = a / g;
		//通解的形式为 (x + s * dx, y - s * dy)
		//若要求x和y都大于0，则s的取值范围为[L, R]
		long long L = x > 0 ? (1 - x) / dx : -x / dx + 1;
		long long R = y > 0 ? (y - 1) / dy : y / dy - 1;
		if (L <= R)
			printf("%lld %lld %lld %lld %lld\n", R - L + 1, x + L * dx, y - R * dy,
				x + R * dx, y - L * dy);
		else //无正整数数解时，分别输出x和y的最小正整数值
			printf("%lld %lld\n", x + L * dx, y - R * dy);
	}
	return 0;
}