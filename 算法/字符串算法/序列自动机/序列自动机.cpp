/*
序列自动机：能够识别给定序列的所有子序列（非连续子序列）
last[c]表示字符c在序列中最后一次出现的位置对应的自动机结点
par[i]表示下标i处的字符的上一次的出现位置对应的自动机结点
ch[i][x]表示以下标i为起点，字符x的第一次出现位置对应的自动机结点
对于用build建立的自动机，自动机结点与原数组中的位置一一对应（自动机中下标i的位置对应原数组中下标i的位置）。
对于用extend函数建立的自动机，自动机结点与原数组中的位置有常数值的偏移。
基本原理：
对于给定序列A、B判断B是否为A的子序列，只要从左往右找到字符B[0]在A中的第一次出现位置A[i]，然后再从i + 1开始找到B[1]的第一次出现位置，
以此类推。若按照如上算法在找到B之前已经到达了A的结尾，则B不是A的子序列，否则B是A的子序列。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
const int maxn = 110000;
const int sigma_size = 26;
int cur, root, par[maxn], last[sigma_size], ch[maxn][sigma_size];
void init()
{
	cur = root = 1;
	memset(ch, 0, sizeof(ch));
	for (int i = 0; i < sigma_size; ++i)
		last[i] = root;
}
void extend(int x) {
	par[++cur] = last[x];
	for (int c = 0; c < sigma_size; ++c)
		for (int i = last[c]; i && !ch[i][x]; i = par[i])
			ch[i][x] = cur;
	last[x] = cur;
}
void build(int A[], int n)	//若数组已知，则可以直接构造序列自动机，此时root = 0.
{							//注意：数组A的下标从1开始
	for (int i = n; i > 0; --i)
	{
		memcpy(ch[i - 1], ch[i], sizeof(ch[i]));
		ch[i - 1][A[i]] = i;
	}
}
int main()
{
	int A[] = { 0, 1, 1, 2, 3, 5, 1, 2 }, B[] = { 1, 1, 2, 5, 1 };
	//init();
	//for (int i = 0; i < sizeof(A) / sizeof(*A); ++i)
	//	extend(A[i]);
	//int st = root;
	build(A, sizeof(A) / sizeof(*A) - 1);
	int st = 0;
	for (int i = 0; i < sizeof(B) / sizeof(*B); ++i)
		st = ch[st][B[i]];
	printf("%d\n", st);
	return 0;
}