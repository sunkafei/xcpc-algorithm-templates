#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <bitset>
using namespace std;
const int maxn = 110;
bitset<maxn> a[maxn];
void Gauss(int n) {
	int now = 0; //now记录当前正在处理的行号
	for (int i = 0; i < n; ++i) { //i记录当前正在处理的列号
		int j = now;
		while (j < n && !a[j][i]) ++j;
		if (j == n) continue;
		if (j != now) swap(a[now], a[j]);
		for (int k = 0; k < n; ++k)
			if (k != now && a[k][i]) 
				a[k] ^= a[now];
		++now;
	}
}
int main() {

	return 0;
}