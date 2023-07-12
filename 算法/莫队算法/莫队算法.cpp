#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
const int maxn = 110000; // maxn > n + sqrt(n)
inline void del(int index) { //删除下标为index的元素

}
inline void add(int index) { //添加下标为index的元素

}
inline int get_ans() { //用小于O(sqrt(n))的时间复杂度求出当前区间的解
	return 0;
}
namespace MoDui {
	struct query { //查询区间为[l, r]，id表示是第几次查询
		int l, r, id;
	};
	int belong[maxn], ans[maxn];
	bool operator<(query a, query b) {
		if (belong[a.l] != belong[b.l])
			return belong[a.l] < belong[b.l];
		else if (belong[a.l] & 1)
			return a.r < b.r;
		else
			return a.r > b.r;
	}
	void work(int n, vector<query> q) { //n指定数组大小（下标从1开始）
		int size = sqrt(n);
		int number = ceil((double)n / size);
		for (int i = 1; i <= number; ++i)
			for (int j = (i - 1) * size + 1; j <= i * size; ++j)
				belong[j] = i;
		sort(q.begin(), q.end());
		int l = 1, r = 0;
		for (auto item : q) {
			while (l < item.l) del(l++);
			while (l > item.l) add(--l);
			while (r < item.r) add(++r);
			while (r > item.r) del(r--);
			ans[item.id] = get_ans();
		}
	}
}
int main() {
	return 0;
}