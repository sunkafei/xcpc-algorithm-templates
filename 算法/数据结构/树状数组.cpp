#include <cstdio>
#define lowbit(x) (x&-x)
namespace dimension1 {
	constexpr int maxn = 550000;
	int n; // ��״����ĳ���
	long long C[maxn];
	inline auto sum(int x) { //����ǰ׺��
		long long ret = 0;
		for (int i = x; i > 0; i -= lowbit(i)) {
			ret += C[i];
		}
		return ret;
	}
	inline void add(int x, int d) { //�����޸�
		for (int i = x; i <= n; i += lowbit(i)) {
			C[i] += d;
		}
	}
}
namespace dimension2 {
	const int maxn = 1100;
	int n, m; // ��״����ĳ���
	long long C[maxn][maxn];
	inline auto sum(int x, int y) { //�����άǰ׺��
		long long ret = 0;
		for (int i = x; i > 0; i -= lowbit(i)) {
			for (int j = y; j > 0; j -= lowbit(j)) {
				ret += C[i][j];
			}
		}
		return ret;
	}
	inline void add(int x, int y, int d) { //�����޸�
		for (int i = x; i <= n; i += lowbit(i)) {
			for (int j = y; j <= m; j += lowbit(j)) {
				C[i][j] += d;
			}
		}
	}
}
int main() { // ���P3374
	using namespace dimension1;
	int m;
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; ++i) {
		int v;
		scanf("%d", &v);
		add(i, v);
	}
	while (m--) {
		int tp, x, y;
		scanf("%d %d %d", &tp, &x, &y);
		if (tp == 1) {
			add(x, y);
		}
		else {
			printf("%lld\n", sum(y) - sum(x - 1));
		}
	}
	return 0;
}