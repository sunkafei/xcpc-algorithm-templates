/*
1. 无向图有欧拉路径的充要条件是：图联通且至多有两个奇点。
   若不存在奇点，则有欧拉回路。
2. 有向图有欧拉路径的充要条件是：至多有两个点的入度不等于出度，
   且必须是一个点的出度比入度大1（起点），一个点的入度比出度大1（终点），
   且忽略边的方向后图必须连通。
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
using namespace std;
const int maxn = 110000;
int t, n, m;
namespace direct_graph {
	//若有多组数据应将G和result清空，调用完euler之后G中的边会被删除
	vector<int> G[maxn];
	vector<pair<int, int>> result;
	void dfs(int x) {
		while (G[x].size()) {
			auto y = G[x].back(); 
			G[x].pop_back();
			dfs(y);
			result.emplace_back(x, y);
		}
	}
	bool euler(int s) {
		dfs(s);
		reverse(result.begin(), result.end());
		if (result.size() != m || (result.size() && result.back().second != s))
			return false; //若只要求有欧拉道路则：if (result.size() != m) return false;
		for (int i = 1; i < result.size(); ++i) if (result[i - 1].second != result[i].first)
			return false;
		return true;
	}
	void solve() {
		multimap<pair<int, int>, int> id;
		for (int i = 1; i <= m; ++i) {
			int x, y;
			scanf("%d %d", &x, &y);
			G[x].push_back(y);
			id.emplace(make_pair(x, y), i);
		}
		int s = 1; while (G[s].empty() && s <= n) s++;
		if (!euler(s)) {
			puts("NO");
			return;
		}
		else {
			puts("YES");
			for (auto pr : result) {
				int x = pr.first, y = pr.second;
				auto iter = id.find({ x, y });
				if (iter == id.end()) {
					iter = id.find({ y, x });
					printf("%d ", -iter->second);
				}
				else
					printf("%d ", iter->second);
				id.erase(iter);
			}
		}
	}
}
namespace undirect_graph {
	//若有多组数据应将G和result清空，调用完euler之后G中的边会被删除
	multiset<int> G[maxn];
	vector<pair<int, int>> result;
	void dfs(int x) {
		while (G[x].size()) {
			auto iter = G[x].begin();
			auto y = *iter;
			G[x].erase(iter);
			G[y].erase(G[y].find(x));
			dfs(y);
			result.emplace_back(x, y);
		}
	}
	bool euler(int s) {
		dfs(s);
		reverse(result.begin(), result.end());
		if (result.size() != m || (result.size() && result.back().second != s))
			return false; //若只要求有欧拉道路则：if (result.size() != m) return false;
		for (int i = 1; i < result.size(); ++i) if (result[i - 1].second != result[i].first)
			return false;
		return true;
	}
	void solve() {
		multimap<pair<int, int>, int> id;
		for (int i = 1; i <= m; ++i) {
			int x, y;
			scanf("%d %d", &x, &y);
			G[x].insert(y);
			G[y].insert(x);
			id.emplace(make_pair(x, y), i);
		}
		int s = 1; while (G[s].empty() && s <= n) s++;
		if (!euler(s)) {
			puts("NO");
			return;
		}
		else {
			puts("YES");
			for (auto pr : result) {
				int x = pr.first, y = pr.second;
				auto iter = id.find({ x, y });
				if (iter == id.end()) {
					iter = id.find({ y, x });
					printf("%d ", -iter->second);
				}
				else
					printf("%d ", iter->second);
				id.erase(iter);
			}
		}
	}
}
int main() { //uoj 117
	//freopen("in.txt", "r", stdin);
	scanf("%d %d %d", &t, &n, &m);
	if (t == 2) { //有向图
		direct_graph::solve();
	}
	else {
		undirect_graph::solve();
	}
	return 0;
}