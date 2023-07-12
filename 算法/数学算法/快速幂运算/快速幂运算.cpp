#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
const int maxn = 100;
const int mod = 1e9 + 7;
int n;
struct matrix
{
	int data[maxn][maxn];
	matrix() : data() {}
	int *operator[] (int idx)
	{
		return data[idx];
	}
	const int *operator[] (int idx) const 
	{
		return data[idx];
	}
};
matrix operator*(const matrix &A, const matrix &B)
{
	matrix C;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			for (int k = 0; k < n; ++k)
				C[i][j] += A[i][k] * B[k][j];
	return C;
}
matrix operator+ (const matrix &A, const matrix &B)
{
	matrix C;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			C[i][j] = A[i][j] + B[i][j];
	return C;
}
matrix pow(matrix A, int m)
{
	matrix ans;
	for (int i = 0; i < n; ++i)
		ans[i][i] = 1;
	while (m)
	{
		if (m & 1)
			ans = ans * A;
		A = A * A;
		m >>= 1;
	}
	return ans;
}
long long pow_mod(long long a, int m)
{
	long long ans = 1;
	while (m)
	{
		if (m & 1)
			ans = ans * a % mod;
		a = a * a % mod;
		m >>= 1;
	}
	return ans;
}
matrix pow_sum(matrix A, int m) //pow_sum(A, m) = sigma pow(A, i)
{                               //               1<=i<=m
	matrix ans, B = A;
	while (m)
	{
		if (m & 1)
			ans = ans * A + B;
		B = B * A + B;
		A = A * A;
		m >>= 1;
	}
	return ans;
}
long long pow_sum(long long a, int m) //pow_sum(A, m) = sigma pow(A, i)
{                               //                      1<=i<=m
	long long ans = 0, b = a;
	while (m)
	{
		if (m & 1)
			ans = (ans * a + b) % mod;
		b = (b * a + b) % mod;
		a = (a * a) % mod;
		m >>= 1;
	}
	return ans;
}
//通过预处理，可以在O(1)的时间内求出x的n次幂
struct Power_Int { //指数在int范围内
	static const int base = (1 << 16) - 1;
	long long f[2][base + 1];
	void init(int x) {
		f[0][0] = f[1][0] = 1;
		for (int i = 1; i <= base; ++i)
			f[0][i] = f[0][i - 1] * x % mod;
		f[1][1] = f[0][base] * x % mod;
		for (int i = 2; i <= base; ++i)
			f[1][i] = f[1][i - 1] * f[1][1] % mod;
	}
	long long pow(int n) {
		return f[0][n & base] * f[1][(n >> 16) & base] % mod;
	}
};
struct Power_LLong { //指数在long long范围内
	static const int base = (1 << 16) - 1;
	long long f[4][base + 1];
	void init(int x) {
		for (int i = 0; i < 4; ++i)
			f[i][0] = 1;
		for (int i = 1; i <= base; ++i)
			f[0][i] = f[0][i - 1] * x % mod;
		for (int j = 1; j <= 3; ++j) {
			f[j][1] = f[j - 1][base] * f[j - 1][1] % mod;
			for (int i = 2; i <= base; ++i)
				f[j][i] = f[j][i - 1] * f[j][1] % mod;
		}
	}
	long long pow(long long n) {
		return f[0][n & base] * f[1][(n >> 16) & base] % mod
			* f[2][(n >> 32) & base] % mod * f[3][(n >> 48) & base] % mod;
	}
}power;

int main()
{
	long long ans = pow_sum(2, 10);
	printf("%lld\n", ans);
	n = 2;
	matrix A; A[0][0] = 4; A[0][1] = 2; A[1][1] = 7; A[1][0] = 1;
	matrix B = pow_sum(A, 3);
	A = A * A * A + A * A + A;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%d ", B[i][j]);
		}
		printf("\n");
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}
	system("pause");
	return 0;
}