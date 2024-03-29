#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <ctime>
using namespace std;
namespace Closure { //求有向图的传递闭包
	const int n = 4000; //对于n = 4000，可以在大约1s的时间内求解
	bitset<n> A[n];
	void solve(int M[][n]) { //M是一个01矩阵，M[i][j]为1当且仅当有一条从i到j的有向边
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				A[i][j] = M[i][j];
		for (int k = 0; k < n; ++k)
			for (int i = 0; i < n; ++i)
				if (A[i][k])
					A[i] |= A[k];
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				M[i][j] = A[i][j]; //将结果复制回输入矩阵中
	}
}
const int maxn = 4000;
int A[maxn][maxn], B[maxn][maxn];
int main() {
	for (int i = 0; i < maxn; ++i)
		for (int j = 0; j < maxn; ++j)
			A[i][j] = B[i][j] = rand() % 2;
	clock_t start = clock();
	for (int k = 0; k < maxn; ++k)
		for (int i = 0; i < maxn; ++i)
			for (int j = 0; j < maxn; ++j)
				B[i][j] = (B[i][j] || (B[i][k] && B[k][j]));
	clock_t end = clock();
	printf("%.5f\n", (double)(end - start) / CLOCKS_PER_SEC);
	start = clock();
	Closure::solve(A);
	end = clock();
	printf("%.5f\n", (double)(end - start) / CLOCKS_PER_SEC);
	for (int i = 0; i < maxn; ++i)
		for (int j = 0; j < maxn; ++j)
			if (A[i][j] != B[i][j])
				abort();
	return 0;
}