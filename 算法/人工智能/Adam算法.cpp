#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <functional>
#include <array>
using namespace std;
const int dim = 2;
using vec = array<double, dim>;
/*
* grad: 计算梯度的函数
* pos: 搜索的起始点
* lr: 学习率（移动的步长）
* decay: 学习率的衰减指数
* exit: 当学习率小于exit时算法结束
* 注意该算法求出的是 函数的极**小**值点
*/
vec Adam(function<vec(vec)> grad, vec pos, double lr, double decay, double exit,
	const double beta1 = 0.9, const double beta2 = 0.9) {
	double pw1 = 1, pw2 = 1;
	vec m = {}, v = {};
	for (int t = 1; lr >= exit; ++t) {
		vec g = grad(pos);
		pw1 *= beta1;
		pw2 *= beta2;
		for (int i = 0; i < dim; ++i) {
			m[i] = m[i] * beta1 + g[i] * (1 - beta1);
			v[i] = v[i] * beta2 + g[i] * g[i] * (1 - beta2);
			double update = m[i] / (1 - pw1);
			double factor = lr / (sqrt(v[i] / (1 - pw2)) + 1e-8);
			pos[i] -= update * factor;
		}
		lr *= decay;
	}
	return pos;
}

int main() {
	auto grad = [](vec A) {
		double x = A[0], y = A[1];
		return vec{ 2 * y + -4 / (x * x), 2 * x + -4 / (y * y) };
	};
	auto ans = Adam(grad, vec{ 1, 1 }, 0.01, 0.999, 1e-8);
	printf("%.10f %.10f\n", ans[0], ans[1]); //1.2599210498948732
	return 0;
}