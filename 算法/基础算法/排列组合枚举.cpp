#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <vector>
using namespace std;
void enumerate(int n, int k) {
	//枚举[0, n)的所有的子集的子集
	for (int s = 0; s < (1 << n); ++s) {
		for (int s0 = s; s0; s0 = (s0 - 1) & s) { //枚举集合s的所有子集

		}
	}
	//枚举[0, n)中所有大小为k的子集
	for (int x, y, s = (1 << k) - 1; s < (1 << n); x = s & -s, y = s + x, s = (((s & ~y) / x) >> 1) | y) {
		//cout << bitset<5>(s) << endl;
	}
	//枚举[0, n)中所有大小为k的排列
	for (int x, y, s = (1 << k) - 1; s < (1 << n); x = s & -s, y = s + x, s = (((s & ~y) / x) >> 1) | y) {
		vector<int> P;
		for (int i = 0; i < n; ++i) if (s & (1 << i))
			P.push_back(i);
		do {
			//for (int i = 0; i < k; ++i) printf("%d ", P[i]); printf("\n");
		} while (next_permutation(P.begin(), P.end()));
	}
}
int main() {
	enumerate(4, 3);
	return 0;
}