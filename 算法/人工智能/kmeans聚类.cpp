#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <iterator>
#include <random>
#include <limits>
#include <cmath>
#include <array>
#pragma GCC optimize("Ofast")
#pragma GCC target("avx512bw,avx512vl")
#pragma GCC optimization("unroll-loops")
using namespace std;
/*
	vec：待聚类的向量组
	n：向量个数
	dimension：向量维度
	belong：向量属于哪一个簇
	center：算法计算出的每个簇的中心向量
	k：簇的个数
	iterations：迭代次数
*/
template<typename T, int MAXN, int MAXM, int MAXD>
inline void kmeans(const T(&vec)[MAXN][MAXD], int n, int dimension, int(&belong)[MAXN], T(&center)[MAXM][MAXD], int k, int iterations) {
	mt19937 engine;
	vector<int> indices(n), choices;
	iota(indices.begin(), indices.end(), 0);
	sample(indices.begin(), indices.end(), back_inserter(choices), k, engine);
	for (int i = 0; i < k; ++i) {
		copy(vec[choices[i]], vec[choices[i]] + dimension, center[i]);
	}
	for (int t = 0; t < iterations; ++t) { //迭代iterations轮
		for (int i = 0; i < n; ++i) {
			T best = numeric_limits<T>::max();
			for (int j = 0; j < k; ++j) {
				T distance = 0;
				for (int d = 0; d < dimension; ++d) {
					distance += (vec[i][d] - center[j][d]) * (vec[i][d] - center[j][d]);
				}
				if (distance < best) {
					best = distance;
					belong[i] = j;
				}
			}
		}
		if (t + 1 != iterations) {
			int sz[MAXM] = {};
			for (int j = 0; j < k; ++j) {
				for (int d = 0; d < dimension; ++d) {
					center[j][d] = 0;
				}
			}
			for (int i = 0; i < n; ++i) {
				sz[belong[i]] += 1;
				for (int d = 0; d < dimension; ++d) {
					center[belong[i]][d] += vec[i][d];
				}
			}
			for (int j = 0; j < k; ++j) {
				for (int d = 0; d < dimension; ++d) {
					center[j][d] /= sz[j];
				}
			}
		}
	}
}
template<typename T, int MAXN, int MAXM, int MAXD>
inline void kmeans_plusplus(const T(&vec)[MAXN][MAXD], int n, int dimension, int(&belong)[MAXN], T(&center)[MAXM][MAXD], int k, int iterations) {
	mt19937 engine;
	vector<T> distance(n, numeric_limits<T>::max());
	vector<bool> mask(n);
	mask[0] = true;
	copy(vec[0], vec[0] + dimension, center[0]);
	for (int j = 1; j < k; ++j) {
		for (int i = 0; i < n; ++i) {
			T dist = 0;
			for (int d = 0; d < dimension; ++d) {
				dist += (vec[i][d] - center[j - 1][d]) * (vec[i][d] - center[j - 1][d]);
			}
			distance[i] = min(distance[i], dist);
		}
		vector<T> sum(n);
		for (int i = 1; i < n; ++i) {
			sum[i] = sum[i - 1] + (mask[i] ? 0.0 : distance[i]);
		}
		uniform_real_distribution<T> gen(0, sum[n - 1]);
		auto index = lower_bound(sum.begin(), sum.end(), gen(engine)) - sum.begin();
		mask[index] = true;
		copy(vec[index], vec[index] + dimension, center[j]);
	}
	for (int t = 0; t < iterations; ++t) { //迭代iterations轮
		for (int i = 0; i < n; ++i) {
			T best = numeric_limits<T>::max();
			for (int j = 0; j < k; ++j) {
				T distance = 0;
				for (int d = 0; d < dimension; ++d) {
					distance += (vec[i][d] - center[j][d]) * (vec[i][d] - center[j][d]);
				}
				if (distance < best) {
					best = distance;
					belong[i] = j;
				}
			}
		}
		if (t + 1 != iterations) {
			int sz[MAXM] = {};
			for (int j = 0; j < k; ++j) {
				for (int d = 0; d < dimension; ++d) {
					center[j][d] = 0;
				}
			}
			for (int i = 0; i < n; ++i) {
				sz[belong[i]] += 1;
				for (int d = 0; d < dimension; ++d) {
					center[belong[i]][d] += vec[i][d];
				}
			}
			for (int j = 0; j < k; ++j) {
				for (int d = 0; d < dimension; ++d) {
					center[j][d] /= sz[j];
				}
			}
		}
	}
}

constexpr int MAXN = 50000, MAXD = 1024, MAXM = 100;
double vec[MAXN][MAXD], center[MAXM][MAXD];
int belong[MAXN];
int main() {
	mt19937 engine;
	normal_distribution<double> generate(0, 0.5), gen(0, 0.1);
	vector<array<double, MAXD>> cluster(MAXM);
	for (int i = 0; i < MAXM; ++i) {
		for (int j = 0; j < MAXD; ++j) {
			cluster[i][j] = generate(engine);
		}
	}
	for (int i = 0; i < MAXN; ++i) {
		int c = i % MAXM;
		for (int j = 0; j < MAXD; ++j) {
			vec[i][j] = cluster[c][j] + gen(engine);
		}
	}
	kmeans(vec, MAXN, MAXD, belong, center, MAXM, 10); // Error: 103956
	//kmeans_plusplus(vec, MAXN, MAXD, belong, center, MAXM, 10); // Error: 59772.6
	double error = 0;
	for (int i = 0; i < MAXN; ++i) {
		double dist = 0;
		for (int j = 0; j < MAXM; ++j) {
			dist += (vec[i][j] - center[belong[i]][j]) * (vec[i][j] - center[belong[i]][j]);
		}
		error += sqrt(dist);
	}
	int sz[MAXM] = {};
	for (int i = 0; i < MAXN; ++i) {
		sz[belong[i]] += 1;
	}
	for (int i = 0; i < MAXM; ++i) {
		cout << sz[i] << " ";
	}
	cout << endl << "Error: " << error << endl;
	return 0;
}