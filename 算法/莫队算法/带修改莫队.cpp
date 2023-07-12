#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;
const int maxn = 210000; // maxn > n + pow(n, 2.0 / 3.0)
struct query {			//tp == 0对应查询操作：查询区间为[l, r]
	int tp, l, r, id, t;//tp == 1对应修改操作：将下标为l的位置修改为r
};
inline void del(int index) { //删除下标为index的元素

}
inline void add(int index) { //添加下标为index的元素

}
inline void change(query& q, bool flag) { //将输入数组下标为q.l的值修改为q.r，flag表示这次修改是否位于当前区间内
										  //flag为true则考虑这次修改对答案的影响，否则不考虑
}
inline void undo(query& q, bool flag) { //撤销修改操作，flag表示这次撤销是否位于当前处理的区间内

}
inline int get_ans() { //用小于O(sqrt(n))的时间复杂度求出当前区间的解
	return 0;
}

int belong[maxn], ans[maxn];
bool operator< (query a, query b) {
	if (belong[a.l] != belong[b.l])
		return belong[a.l] < belong[b.l];
	else if (belong[a.r] != belong[b.r])
		return belong[a.r] < belong[b.r];
	else
		return a.t < b.t;
}
void work(int n, vector<query> q) { //n指定数组大小（下标从1开始）
	int size = pow(n, 2.0 / 3.0);
	int number = ceil((double)n / size);
	for (int i = 1; i <= number; ++i)
		for (int j = (i - 1) * size + 1; j <= i * size; ++j)
			belong[j] = i;
	vector<query> a, b; //a记录查询，b记录修改
	for (auto item : q) {
		if (item.tp == 0) {
			item.id = a.size();
			item.t = b.size();
			a.push_back(item);
		}
		else {
			b.push_back(item);
		}
	}
	sort(a.begin(), a.end());
	int l = 1, r = 0, t = 0;
	for (auto item : a) {
		while (l < item.l) del(l++);
		while (l > item.l) add(--l);
		while (r < item.r) add(++r);
		while (r > item.r) del(r--);
		while (t < item.t) {
			change(b[t], item.l <= b[t].l && item.r >= b[t].l);
			++t;
		}
		while (t > item.t) {
			--t;
			undo(b[t], item.l <= b[t].l && item.r >= b[t].l);
		}
		ans[item.id] = get_ans();
	}
	/*
	for (int i = 0; i < a.size(); ++i)
		printf("%d\n", ans[i]);
	*/
}

int main() {
	return 0;
}