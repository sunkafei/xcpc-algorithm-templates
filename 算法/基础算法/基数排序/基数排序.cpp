#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <random>
#include <limits>
#include <ctime>
using namespace std;
const int maxn = 1100000; //maxn应当大于data数组的大小
namespace RadixSort_Int {
	const int base = (1 << 17) - 1;
	int c[base + 10], tmp[maxn];
	void sort(int* data, int n) {			//只能用于对非负数离散化，若要处理负数，
		for (int i = 0; i < 32; i += 16) {	//可以将所有的值都加上一个特别大的数使其变为非负数然后再离散化
			memset(c, 0, sizeof(c));
			for (int j = 0; j < n; ++j)
				c[(data[j] >> i) & base]++;
			for (int j = 1; j <= base; ++j)
				c[j] += c[j - 1];
			for (int j = n - 1; j >= 0; --j)
				tmp[--c[(data[j] >> i) & base]] = data[j];
			for (int j = 0; j < n; ++j)
				data[j] = tmp[j];
		}
	}
}
namespace RadixSort_LLong {
	const int base = (1 << 17) - 1;
	int c[base + 10];
	long long tmp[maxn];
	void sort(long long* data, int n) {		//只能用于对非负数离散化，若要处理负数，
		for (int i = 0; i < 64; i += 16) {	//可以将所有的值都加上一个特别大的数使其变为非负数然后再离散化
			memset(c, 0, sizeof(c));
			for (int j = 0; j < n; ++j)
				c[(data[j] >> i) & base]++;
			for (int j = 1; j <= base; ++j)
				c[j] += c[j - 1];
			for (int j = n - 1; j >= 0; --j)
				tmp[--c[(data[j] >> i) & base]] = data[j];
			for (int j = 0; j < n; ++j)
				data[j] = tmp[j];
		}
	}
}
int A[maxn], B[maxn];
int main() {
	int n = 1000000;
	default_random_engine e;
	uniform_int_distribution<int> d(0, INT_MAX);
	for (int i = 0; i < n; ++i)
		A[i] = B[i] = d(e);
	printf("start....\n");
	auto start = clock();
	sort(A, A + n);
	printf("std::sort: %f\n", static_cast<double>(clock() - start) / CLOCKS_PER_SEC);
	start = clock();
	RadixSort_Int::sort(B, n);
	printf("std::sort: %f\n", static_cast<double>(clock() - start) / CLOCKS_PER_SEC);
	for (int i = 0; i < n; ++i)
		if (A[i] != B[i])
			abort();
	return 0;
}