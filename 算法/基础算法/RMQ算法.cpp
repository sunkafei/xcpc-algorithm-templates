#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;
const int maxn = 110000, maxlog = 20;
int d[maxn][maxlog], lg[maxn], A[maxn];
void preprocess(int n) { //下标范围[1, n]，调用之前应当完成数组A的赋值
	lg[1] = 0;
	for (int i = 2; i <= n; ++i)
		lg[i] = lg[i >> 1] + 1;
	for (int i = 1; i <= n; ++i)
		d[i][0] = A[i];
	for (int j = 1; j <= lg[n]; ++j)
		for (int i = 1; i + (1 << j) - 1 <= n; ++i)
			d[i][j] = max(d[i][j - 1], d[i + (1 << (j - 1))][j - 1]);
}
inline int rmq(int L, int R) {
	int k = lg[R - L + 1];
	return max(d[L][k], d[R - (1 << k) + 1][k]);
}
int main() {
	return 0;
}