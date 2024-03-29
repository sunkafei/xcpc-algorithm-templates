/*
动态维护由相同数字构成的区间，A是原始数组。将A中相同数字只保留最左侧，其余设为-1得到val数组。
该算法动态维护val数组，时间复杂度O(1)。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 210000;
int A[maxn], val[maxn], n;
void build() {

}
inline void add(int index) { //将A[index]添加到val[index]中构成一个新的区间
	val[index] = A[index];
	//此处添加代码维护数据结构
}
inline void del(int index) { //删除val[index]，也就是删除下标为index的区间
	val[index] = -1;
	//此处添加代码维护数据结构
}
void init() { //调用init函数初始化之前需要读入n和A
	A[0] = A[n + 1] = -1;
	val[1] = A[1];
	for (int i = 2; i <= n; ++i)
		val[i] = (A[i] == A[i - 1] ? -1 : A[i]);
	build(); //用val数组构建数据结构
}
void change(int index, int v) { //将A中下标为index的位置的元素修改为v，函数会维护A和val数组
	if (A[index] == v)
		return;
	if (A[index] == A[index + 1])
		add(index + 1);
	if (val[index] != -1)
		del(index);
	if (val[index + 1] == v)
		del(index + 1);
	A[index] = v;
	if (A[index - 1] != v)
		add(index);
}
int main() {
	freopen("in.txt", "r", stdin);
	n = 5;
	for (int i = 1; i <= n; ++i)
		A[i] = 1;
	init();
	for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	change(3, 2); for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	change(4, 2); for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	change(2, 2); for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	change(2, 1); for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	change(1, 2); for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	change(2, 2); for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	change(3, 1); for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	change(3, 2); for (int i = 1; i <= n; ++i) printf("%d ", val[i]); printf("\n");
	return 0;
}