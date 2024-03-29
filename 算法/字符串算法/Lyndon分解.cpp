#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
//Lyndon分解将一个字符串分解为若干个连续的子串S1, S2, ..., Sn，并且满足
//S1 >= S2 >= ... >= Sn，并且所有这些子串都满足最小后缀是其本身。
//该函数返回Lyndon分解成的所有子串的起始下标。
vector<int> Lyndon(const char* s) {
    int n = strlen(s);
    vector<int> res;
    for (int i = 0; i < n; ) {
        int j = i, k = i + 1;
        while (k < n && s[j] <= s[k]) 
            j = (s[j] == s[k++] ? j + 1 : i);
        while (i <= j) {
            res.push_back(i);
            i += k - j;
        }
    }
    return res;
}
const int maxn = 1110000;
int pos[maxn]; //pos[i]表示前缀s[0, i]的最小后缀的下标
void preprocess(const char* s) {
    int n = strlen(s);
    for (int i = 0; i < n; ) {
        int j = i, k = i + 1;
        pos[i] = i;
        while (k < n && s[j] <= s[k]) {
            if (s[j] < s[k]) {
                pos[k] = j = i;
            }
            else {
                pos[k] = pos[j] + k - j;
                j += 1;
            }
            k += 1;
        }
        while (i <= j) 
            i += k - j;
    }
}
char s[maxn];
int main() { //hdu6761
    const int mod = 1e9 + 7;
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%s", s);
        preprocess(s);
        int n = strlen(s);
        long long ans = 0, val = 1;
        for (int i = 0; i < n; ++i) {
            ans = (ans + (pos[i] + 1) * val) % mod;
            val = val * 1112 % mod;
        }
        printf("%lld\n", ans);
    }
	return 0;
}