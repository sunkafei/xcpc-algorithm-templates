/*
对于状态图中的每一条边(i, j)，设置矩阵A[i][j] = 1。
令B = pow(A, n)，
则从i开始走n步到达j的方案数为B[i][j]。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
const int maxn = 100;
int n;
struct matrix
{
	int data[maxn][maxn];
	matrix() : data(){}
	int *operator[] (int idx)
	{
		return data[idx];
	}
};
matrix operator*(matrix &A, matrix &B)
{
	matrix C;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			for (int k = 0; k < n; ++k)
				C[i][j] += A[i][k] * B[k][j];
	return C;
}
matrix pow(matrix A, int t)
{
	matrix ans;
	for (int i = 0; i < n; ++i)
		ans[i][i] = 1;
	while (t)
	{
		if (t & 1)
			ans = ans * A;
		t >>= 1;
		A = A * A;
	}
	return ans;
}
int main()
{
	return 0;
}