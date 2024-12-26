#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
char T[] = "abcdefabc", P[] = "abc";
const int maxn = 10000;
int f[maxn];	//f[i]表示字符串s[0, i-1]的后缀与前缀的最长公共部分（后缀与前缀均不包含字符串本身）
//若f[i] = k则，字符串s[0, k-1]与字符串s[i-k, i-1]相同
void getfail(char* P, int* f) {
	int m = strlen(P);
	f[0] = 0; f[1] = 0;
	for (int i = 1; i < m; ++i) {
		int j = f[i];
		while (j && P[j] != P[i])
			j = f[j];
		f[i + 1] = P[j] == P[i] ? j + 1 : 0;
	}
}
void find(char* T, char* P, int* f) {
	int n = strlen(T), m = strlen(P);
	//getfail(P, f);
	int j = 0;
	for (int i = 0; i < n; ++i) {
		while (j && P[j] != T[i])
			j = f[j];
		if (P[j] == T[i])
			++j;
		if (j == m)
			printf("%d\n", i - m + 1); //在串T中找到了P，下标为i - m + 1
	}
}
int main() {
	getfail(P, f);
	find(T, P, f);
	return 0;
}