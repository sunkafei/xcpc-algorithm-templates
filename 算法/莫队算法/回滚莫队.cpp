/*
left[i]表示第i块的左端点编号（块编号从1开始）
right[i]表示第i块的右端点编号
belong[i]表示下标i所在的块编号（下标从1开始）
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;
const int maxn = 110000;
void init() { //用O(1)的时间复杂度创建一个空的区间

}
void add_right(int index) { //将输入数组中下标index的元素添加到当前区间的右边

}
void add_left(int index) {	//将输入数组中下标index的元素添加到当前区间的左边
							//该函数可能需要保存原状态，以便于之后状态的恢复
}
void breakpoint() { //记录下当前的状态以便于之后恢复
					//调用该函数之后会调用add_left修改状态，最后用resume恢复到调用breakpoint之前的状态
}
void resume() { //撤销所有的add_left操作，恢复到调用breakpoint之前的状态（一次性撤销所有的add_left）

}
int get_ans() { //用小于O(sqrt(n))的时间复杂度求出当前区间的解
	return 0;
}
namespace MoDui {
	struct query { //查询区间为[l, r]，id表示是第几次查询
		int l, r, id;
	};
	int left[maxn], right[maxn], belong[maxn], ans[maxn]; // ans可能要用long long存
	bool operator< (query a, query b) {
		return belong[a.l] != belong[b.l] ? belong[a.l] < belong[b.l] : a.r < b.r;
	}
	void work(int n, vector<query> q) { //n指定数组大小（下标从1开始）
		int size = sqrt(n);	
		int number = ceil((double)n / size);
		for (int i = 1; i <= number; ++i) {
			left[i] = size * (i - 1) + 1;
			right[i] = size * i;
			for (int j = left[i]; j <= right[i]; ++j)
				belong[j] = i;
		}
		right[number] = n;
		sort(q.begin(), q.end());
		for (auto item : q) if (belong[item.l] == belong[item.r]) {
			init();
			for (int i = item.l; i <= item.r; ++i)
				add_right(i);
			ans[item.id] = get_ans();
		}
		for (int i = 0, k = 1; k <= number; ++k) {
			init();
			for (int r = right[k]; i < q.size() && belong[q[i].l] == k; ++i) {
				int ql = q[i].l, qr = q[i].r;
				int l = right[k] + 1;
				if (belong[ql] != belong[qr]) {
					while (r < qr) {
						++r; 
						add_right(r);
					}
					breakpoint();
					while (l > ql) {
						--l; 
						add_left(l);
					}
					ans[q[i].id] = get_ans();
					resume();
				}
			}
		}
	}
}
int main() {
	return 0;
}