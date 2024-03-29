#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
using namespace std;
const int maxn = 210000;
const int maxdim = 1001;
const double eps = 1e-8;
using matrix = double[maxdim][maxdim];
using vec = array<double, maxdim>;
using pair_t = pair<double, vec>;
struct PCA {
	matrix A, V;
	int column[maxdim], n;
	void update(int r, int c, double v) {
		A[r][c] = v;
		if (column[r] == c || fabs(A[r][c]) > fabs(A[r][column[r]])) {
			for (int i = 0; i < n; ++i) if (i != r)
				if (fabs(A[r][i]) > fabs(A[r][column[r]]))
					column[r] = i;
		}
	}
	void Jacobi() {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)
				V[i][j] = 0;
			V[i][i] = 1;
			column[i] = (i == 0 ? 1 : 0);
		}
		for (int i = 0; i < n; ++i) 
			for (int j = 0; j < n; ++j) 
				if (j != i && fabs(A[i][j]) > fabs(A[i][column[i]]))
					column[i] = j;
		for (int T = 0; ; ++T) { //迭代次数限制
			int x, y;
			double val = 0;
			for (int i = 0; i < n; ++i)
				if (fabs(A[i][column[i]]) > val)
					val = fabs(A[i][column[i]]), x = i, y = column[i];
			if (val < eps) //精度限制
				break;
			double phi = atan2(-2 * A[x][y], A[y][y] - A[x][x]) / 2;
			double sinp = sin(phi), cosp = cos(phi);
			for (int i = 0; i < n; ++i) if (i != x && i != y) {
				double a = A[x][i] * cosp + A[y][i] * sinp;
				double b = A[x][i] * -sinp + A[y][i] * cosp;
				update(x, i, a);
				update(y, i, b);
			}
			for (int i = 0; i < n; ++i) if (i != x && i != y) {
				double a = A[i][x] * cosp + A[i][y] * sinp;
				double b = A[i][x] * -sinp + A[i][y] * cosp;
				update(i, x, a);
				update(i, y, b);
			}
			for (int i = 0; i < n; ++i) {
				double a = V[i][x] * cosp + V[i][y] * sinp;
				double b = V[i][x] * -sinp + V[i][y] * cosp;
				V[i][x] = a, V[i][y] = b;
			}
			double a = A[x][x] * cosp * cosp + A[y][y] * sinp * sinp + 2 * A[x][y] * cosp * sinp;
			double b = A[x][x] * sinp * sinp + A[y][y] * cosp * cosp - 2 * A[x][y] * cosp * sinp;
			double tmp = (A[y][y] - A[x][x]) * sin(2 * phi) / 2 + A[x][y] * cos(2 * phi);
			update(x, y, tmp);
			update(y, x, tmp);
			A[x][x] = a, A[y][y] = b;
		}
	}
	//a 为输入向量组
	//n 为向量的维数
	//center 指针用来保存输入向量组的中心点
	//返回特征值和特征向量的pair，按照特征值从大到小排序
	//特征值是各个点在对应特征向量方向的坐标平方和，除以(a.size() - 1)为方差。
	auto solve(vector<vec> a, int n, vec* center = nullptr) {
		this->n = n;
		vec s = {};
		for (int i = 0; i < a.size(); ++i)
			for (int j = 0; j < n; ++j)
				s[j] += a[i][j];
		for (int j = 0; j < n; ++j)
			s[j] /= a.size();
		for (int i = 0; i < a.size(); ++i)
			for (int j = 0; j < n; ++j)
				a[i][j] -= s[j];
		if (center) *center = s;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				A[i][j] = 0;
				for (int k = 0; k < a.size(); ++k)
					A[i][j] += a[k][i] * a[k][j];
			}
		}
		Jacobi();
		vector<pair_t> result;
		for (int i = 0; i < n; ++i)
			result.emplace_back(A[i][i], vec());
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				result[i].second[j] = V[j][i];
		sort(result.begin(), result.end(), greater<pair_t>());
		return result;
	}
}pca;
int main() { //1329070654.526
	freopen("in.txt", "r", stdin);
	vector<vec> a;
	int n, m;
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; ++i) {
		vec v = {};
		for (int j = 0; j < m; ++j)
			scanf("%lf", &v[j]);
		a.push_back(v);
	}
	auto now = clock();
	auto result = pca.solve(a, m);
	for (int i = 0; i < m; ++i)
		printf("%.3f\n", result[i].first);
	printf("time: %f\n", double(clock() - now) / CLOCKS_PER_SEC);
	return 0;
}