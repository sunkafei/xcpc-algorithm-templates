/*
矩阵树定理与部分扩展：
1.	给出一个无向无权图，设A为邻接矩阵, D为度数矩阵(D[i][i] = 结点i的度数,其他的无值)。
	则基尔霍夫(Kirchhoff)矩阵即为: K = D - A，
	K的任意一个n - 1阶子式即为该图的生成树个数。
2.	把度数矩阵重新定义为D[i][i] = 与结点i相连的所有边的权值和，
	把邻接矩阵重新定义为A[i][j] = 结点i与结点j之间所有边的权值和，
	那么矩阵树定理求的就是: 所有生成树边权乘积的总和。
3.	有向图的情况也是可以做的，若D[i][i] = 结点i的入边的权值和，此时求的就是外向树 (从根向外)，
	若D[i][i] = 结点i的出边的权值和，此时求的就是内向树 (从外向根)，
	既然是有向的，那么就需要指定根，求行列式的时候去掉哪一行就是那一个元素为根。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const long long mod = 1e9 + 7;
const int maxn = 500;
using matrix = long long[maxn][maxn];
//时间复杂度n^3logm，如果超时可换成一般的高斯消元
//注意矩阵A的下标从1开始
long long det(matrix A, int n) {
	long long res = 1;
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			while (A[j][i]) { //辗转相除法
				long long t = A[i][i] / A[j][i];
				for (int k = i; k < n; ++k) {
					A[i][k] = (A[i][k] - t * A[j][k] % mod + mod) % mod;
					swap(A[i][k], A[j][k]);
				}
				res = -res;
			}
		}
		if (!A[i][i]) return 0;
		res = (res * A[i][i] % mod + mod) % mod;
	}
	return res;
}
int main() { //洛谷P6178
	static matrix A;
	int n, m, t;
	scanf("%d %d %d", &n, &m, &t);
	for (int i = 0; i < m; ++i) {
		int x, y, w;
		scanf("%d %d %d", &x, &y, &w);
		--x; --y;
		if (t == 0) {
			A[x][y] = (A[x][y] - w + mod) % mod;
			A[y][x] = (A[y][x] - w + mod) % mod;
			A[x][x] = (A[x][x] + w) % mod;
			A[y][y] = (A[y][y] + w) % mod;
		}
		else {
			if (x == 0) x = n - 1; else if (x == n - 1) x = 0;
			if (y == 0) y = n - 1; else if (y == n - 1) y = 0;
			A[x][y] = (A[x][y] - w + mod) % mod;
			A[y][y] = (A[y][y] + w) % mod;
		}
	}
	printf("%lld\n", det(A, n - 1));
	return 0;
}