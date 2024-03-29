#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <random>
#include <limits>
#include <ctime>
using namespace std;
const int maxn = 11000000; //maxn应当大于data数组的大小
namespace Discretization_Int {
	const int base = (1 << 17) - 1;
	int c[base + 10];
	pair<int, int> data[maxn], tmp[maxn];
	void discretization(int* input, int n) {	//只能用于对非负数离散化，若要处理负数，
		for (int i = 0; i < n; ++i)				//可以将所有的值都加上一个特别大的数使其变为非负数然后再离散化
			data[i] = make_pair(input[i], i);
		for (int i = 0; i < 32; i += 16) {
			memset(c, 0, sizeof(c));
			for (int j = 0; j < n; ++j)
				c[(data[j].first >> i) & base]++;
			for (int j = 1; j <= base; ++j)
				c[j] += c[j - 1];
			for (int j = n - 1; j >= 0; --j)
				tmp[--c[(data[j].first >> i) & base]] = data[j];
			for (int j = 0; j < n; ++j)
				data[j] = tmp[j];
		}
		for (int i = 0, j = -1; i < n; ++i) {
			if (i == 0 || data[i].first != data[i - 1].first)
				++j;
			input[data[i].second] = j;
		}
	}
}
namespace Discretization_LLong {
	const int base = (1 << 17) - 1;
	int c[base + 10];
	pair<long long, int> data[maxn], tmp[maxn];
	void discretization(long long* input, int n) {	//只能用于对非负数离散化，若要处理负数，
		for (int i = 0; i < n; ++i)					//可以将所有的值都加上一个特别大的数使其变为非负数然后再离散化
			data[i] = make_pair(input[i], i);
		for (int i = 0; i < 64; i += 16) {
			memset(c, 0, sizeof(c));
			for (int j = 0; j < n; ++j)
				c[(data[j].first >> i) & base]++;
			for (int j = 1; j <= base; ++j)
				c[j] += c[j - 1];
			for (int j = n - 1; j >= 0; --j)
				tmp[--c[(data[j].first >> i) & base]] = data[j];
			for (int j = 0; j < n; ++j)
				data[j] = tmp[j];
		}
		for (int i = 0, j = -1; i < n; ++i) {
			if (i == 0 || data[i].first != data[i - 1].first)
				++j;
			input[data[i].second] = j;
		}
	}
}
long long A[maxn], B[maxn], tmp[maxn];
int main() {
	int n = 10000000;
	default_random_engine e;
	uniform_int_distribution<long long> d(0, LLONG_MAX);
	e.seed(time(0));
	for (int i = 0; i < n; ++i)
		A[i] = B[i] = tmp[i] = d(e);
	printf("start....\n");
	auto start = clock();
	sort(tmp, tmp + n);
	int sz = unique(tmp, tmp + n) - tmp;
	for (int i = 0; i < n; ++i)
		A[i] = lower_bound(tmp, tmp + sz, A[i]) - tmp;
	printf("std::sort: %f\n", static_cast<double>(clock() - start) / CLOCKS_PER_SEC);
	start = clock();
	Discretization_LLong::discretization(B, n);
	printf("std::sort: %f\n", static_cast<double>(clock() - start) / CLOCKS_PER_SEC);
	for (int i = 0; i < n; ++i)
		if (A[i] != B[i])
			abort();
	return 0;
}