#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <complex>
#include <cmath>
using namespace std;
const int maxn = 1 << 22; //必须是2的幂
const double pi = acos(-1);
complex<double> w[maxn];
void init() {
    for (int i = 0;i < maxn;++i)
        w[i] = { cos(pi * 2 * i / maxn), sin(pi * 2 * i / maxn) };
}
void fft(vector<complex<double>> &a, bool inverse) {
    for (int i = 0, j = 0; i < a.size(); ++i) {
        if (i < j)
            std::swap(a[i], a[j]);
        for (int k = a.size() >> 1; (j ^= k) < k; k >>= 1);
    }
    for (int step = 2; step <= a.size(); step *= 2) {
        int h = step / 2, d = maxn / step;
        for (int i = 0; i < a.size(); i += step)
            for (int j = 0; j < h; ++j) {
                auto &x = a[i + j];
                auto &y = a[i + j + h];
                auto t = w[d * j] * y;
                y = x - t;
                x = x + t;
            }
    }
    if (inverse) {
        std::reverse(a.begin() + 1, a.end());
        for (auto& x : a) 
            x /= a.size();
    }
}
vector<double> operator* (const vector<double> &a, const vector<double> &b) {
    int size = 2;
    while (size < a.size() + b.size()) size <<= 1;
    vector<complex<double>> x(size), y(size);
    copy(a.begin(), a.end(), x.begin());
    copy(b.begin(), b.end(), y.begin());
    fft(x, false);
    fft(y, false);
    for (int i = 0; i < size; i++) 
        x[i] *= y[i];
    fft(x, true);
    vector<double> res(a.size() + b.size() - 1);
    for (int i = 0; i < res.size(); ++i) 
        res[i] = x[i].real();
    return res;
}
int main() {
    int n, m;
    init();
    scanf("%d %d", &n, &m);
    vector<double> a(n + 1), b(m + 1);
    for (int i = 0; i <= n; ++i)
        scanf("%lf", &a[i]);
    for (int i = 0; i <= m; ++i)
        scanf("%lf", &b[i]);
    auto ans = a * b;
    for (int i = 0; i < ans.size(); ++i)
        printf("%.0f ", ans[i] + 1e-8);
    return 0;
}