#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
char T[] = "abcdefabc", P[] = "abc";
const int maxn = 10000;
int f[maxn];	//f[i]��ʾ�ַ���s[0, i-1]�ĺ�׺��ǰ׺����������֣���׺��ǰ׺���������ַ�������
//��f[i] = k���ַ���s[0, k-1]���ַ���s[i-k, i-1]��ͬ
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
			printf("%d\n", i - m + 1); //�ڴ�T���ҵ���P���±�Ϊi - m + 1
	}
}
int main() {
	getfail(P, f);
	find(T, P, f);
	return 0;
}