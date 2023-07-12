#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;
const int maxn = 111;
const double eps = 1e-10;
using matrix = double[maxn][maxn];
matrix Q, R;
double u[maxn], w[maxn];
vector<double> eigenvalues(matrix& A, int n) {
	for (int T = 0; T < 1000000; ++T) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				Q[i][j] = 0;
				R[i][j] = A[i][j];
			}
			Q[i][i] = 1;
		}
		for (int i = 0; i < n; ++i) {
			double sum = 0;
			for (int j = i; j < n; ++j)
				sum += fabs(R[i][j]);
			if (sum < eps)
				continue;
			double sigma = R[i][i] <= 0 ? 1 : -1;
			double tot = 0;
			for (int j = i; j < n; ++j) {
				tot += R[j][i] * R[j][i];
				w[j] = -sigma * R[j][i];
			}
			w[i] += sqrt(tot);
			tot = sqrt(tot - R[i][i] * R[i][i] + w[i] * w[i]);
			for (int j = i; j < n; ++j)
				u[j] = w[j] / tot;
			for (int j = 0; j < n; ++j) {
				double product = 0;
				for (int k = i; k < n; ++k)
					product += u[k] * Q[k][j];
				for (int k = i; k < n; ++k)
					Q[k][j] = sigma * (Q[k][j] - 2 * product * u[k]);
			}
			for (int j = i; j < n; ++j) {
				double product = 0;
				for (int k = i; k < n; ++k)
					product += u[k] * R[k][j];
				for (int k = i; k < n; ++k)
					R[k][j] = sigma * (R[k][j] - 2 * product * u[k]);
			}
		}
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				A[i][j] = 0;
				for (int k = 0; k < n; ++k)
					A[i][j] += R[i][k] * Q[j][k];
			}
		}
	}
	vector<double> ret;
	for (int i = 0; i < n; ++i)
		ret.push_back(R[i][i]);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			printf("%.10f ", R[i][j]);
		printf("\n");
	}
	return ret;
}
matrix A = { {-1, 1, 0}, {-4, 3, 0 }, {1, 0, 2} };
int main() {
	eigenvalues(A, 3);
	return 0;
}