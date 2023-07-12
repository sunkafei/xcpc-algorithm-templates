#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;
const int maxn = 110000, sigma_size = 20;
struct Automaton {
	int n, m, cur, G[maxn][sigma_size];
	int inq[maxn], cls[maxn];
	vector<int> from[maxn][sigma_size];
	unordered_set<int> equiv[maxn]; //equiv[i]表示编号为i的等价类的集合
	void init(int n, int m, vector<bool> final) { //状态数、输入字符集大小、每个结点是否为终态
		//如果要多次使用该算法，需要在此处先将状态清空。
		this->n = n;
		this->m = m;
		this->cur = 2;
		for (int i = 0; i < n; ++i) {
			int st = final[i] ^ 1;
			equiv[st].insert(i);
			cls[i] = st;
		}
	} 
	void add_edge(int x, int c, int y) { //添加一条从x指向y的边，字符为c
		G[x][c] = y;
	}
	int minimize() { //调用该方法之前应该先将不可达状态从自动机中删除
		for (int i = 0; i < n; ++i)
			for (int c = 0; c < m; ++c)
				from[G[i][c]][c].push_back(i);
		queue<int> Q;
		Q.push(0); inq[0] = true;
		while (!Q.empty()) {
			int x = Q.front(); Q.pop(); //x是一个等价类
			inq[x] = false;
			for (int c = 0; c < m; ++c) {
				unordered_map<int, vector<int>> par; //class -> set of states
				for (auto i : equiv[x]) for (auto u : from[i][c])
					//if (cls[u] != x)
					par[cls[u]].push_back(u);
				for (auto &[id, member] : par) if (member.size() != equiv[id].size()) {
					int now = cur++;
					for (auto y : member) {
						equiv[id].erase(y);
						equiv[now].insert(y);
						cls[y] = now;
					}
					if (inq[id] || equiv[now].size() < equiv[id].size())
						Q.push(now), inq[now] = true;
					else
						Q.push(id), inq[id] = true;
				}
			}
		}
		return cur; //返回合并之后的总状态数
	}
} am;
int main() {
	int b, m;
	scanf("%d %d", &b, &m);
	vector<bool> final(m, 0); final[0] = 1;
	am.init(m, b, move(final));
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < b; ++j)
			am.add_edge(i, j, (i * b + j) % m);
	int sz = am.minimize();
	printf("%d 0\n", sz);
	printf("G");
	for (int i = 1; i < sz; ++i)
		printf(" B");
	printf("\n");
	for (int i = 0; i < sz; ++i) {
		for (int j = 0; j < b; ++j) {
			int first = *am.equiv[i].begin();
			printf("%d ", am.cls[(first * b + j) % m]);
		}
		printf("\n");
	}
	return 0;
}