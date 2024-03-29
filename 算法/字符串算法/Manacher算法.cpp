#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int maxn = 1.1e7 * 2 + 100; //maxn应当大于 原字符串长度的两倍加二
char s[maxn];
int len[maxn];
//回文串在原字符串中的长度为len[i] - 1
void manacher(char* str) {
	int n = strlen(str), m = 0;
	for (int i = 0; i < n; ++i) {
		s[++m] = '#';
		s[++m] = str[i];
	}
	s[++m] = '#'; s[0] = '$'; //s是加入新字符后的字符串
	len[1] = 1;
	int r = 1, po = 1; //r是当前极长回文子串的最右的端点 po为r对应的回文子串的中心
	for (int i = 1; i <= m; ++i) {
		if (i < r)
			len[i] = min(len[2 * po - i], r - i + 1); //2*po-i为i在当前这个极长回文子串中在左边相对应的位置
		else
			len[i] = 1;
		while (s[i + len[i]] == s[i - len[i]])
			++len[i];
		if (i + len[i] > r) {
			r = i + len[i] - 1;
			po = i;
		}
	}
	//原字符串的最长回文子串为 max{ len[i] - 1 }
	//以i为中心的奇回文串的长度为len[2 * i + 2] - 1
	//以i为左中心的偶回文串的长度为len[2 * i + 3] - 1
}
char a[maxn / 2];
int main() {
	scanf("%s", a);
	const int n = strlen(a);
	manacher(a);
	int ans = *max_element(len, len + n * 2 + 4) - 1;
	printf("%d\n", ans);
	return 0;
}