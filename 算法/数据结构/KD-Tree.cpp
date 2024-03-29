#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <array>
#define sqr(x) ((long long)(x)*(x))
using namespace std;
const int maxn = 210000;
const int inf = 1 << 30;
const int k = 2;
using point = array<int, k>;
namespace kdt {
	int cur, sz[maxn], ch[maxn][2]; //如果没有插入操作，只需要询问最近点，则可以不维护sz属性
	int mark[maxn], real[maxn]; //没有删除操作时可以不记录这两个属性
	point mn[maxn], mx[maxn], val[maxn];
	int ans;
	void init() {
		cur = 0;
		for (int i = 0; i < k; ++i)
			mn[0][i] = inf, mx[0][i] = -inf;
	}
	inline int newnode(point pt) {
		int x = ++cur;
		sz[x] = 1;
		ch[x][0] = ch[x][1] = 0;
		mn[x] = mx[x] = val[x] = pt;
		mark[x] = real[x] = 1; //如果是静态KD-Tree则不需要记录mark标记和real标记
		return x;
	}
	inline void pushup(int x) {
		sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + 1;
		real[x] = real[ch[x][0]] + real[ch[x][1]] + mark[x];
		for (int i = 0; i < k; ++i) {
			mn[x][i] = min(val[x][i], min(mn[ch[x][0]][i], mn[ch[x][1]][i]));
			mx[x][i] = max(val[x][i], max(mx[ch[x][0]][i], mx[ch[x][1]][i]));
		}
		//要么特判不合并0号结点的值，要么将0号结点的值设为极限值
	}
	inline void pushdown(int x) {
		//如果pushdown会将标记传给0号结点的话，则要在pushup中要判断不能合并0号结点的值
	}
	//A序列是已经用newnode分配好的结点序列，参数区间[L, R]是闭区间
	template<int d = 0> int build(int L, int R, int* A) {
		if (L > R) return 0;
		int mid = (L + R) >> 1;
		nth_element(A + L, A + mid, A + R + 1, [](int x, int y) {
			return val[x][d] < val[y][d];
			});
		int x = A[mid];
		ch[x][0] = build<(d + 1) % k>(L, mid - 1, A);
		ch[x][1] = build<(d + 1) % k>(mid + 1, R, A);
		pushup(x);
		return x;
	}
	inline bool in(int x, const point& lower, const point& upper) { //x in [lower, upper]
		for (int i = 0; i < k; ++i)
			if (mn[x][i] < lower[i] || mx[x][i] > upper[i])
				return false;
		return true;
	}
	inline bool out(int x, const point& lower, const point& upper) { //x out of [lower, upper]
		for (int i = 0; i < k; ++i)
			if (mn[x][i] > upper[i] || mx[x][i] < lower[i])
				return true;
		return false;
	}
	inline bool contain(int x, const point& lower, const point& upper) { //val[x] in [lower, upper]
		for (int i = 0; i < k; ++i)
			if (val[x][i] < lower[i] || val[x][i] > upper[i])
				return false;
		return true;
	}
	//-----------------区间查询和修改-------------------------------
	int query(int x, const point& lower, const point& upper) {
		if (x == 0 || out(x, lower, upper))
			return 0;
		if (in(x, lower, upper))
			return sz[x];
		pushdown(x);
		return contain(x, lower, upper) + query(ch[x][0], lower, upper) + query(ch[x][1], lower, upper);
	}
	void update(int x, const point& lower, const point& upper) {
		if (x == 0 || out(x, lower, upper))
			return;
		if (in(x, lower, upper)) {
			; //对区间打上标记
			return;
		}
		if (contain(x, lower, upper))
			; //直接修改当前结点的值
		pushdown(x);
		update(ch[x][0], lower, upper);
		update(ch[x][1], lower, upper);
		pushup(x);
	}
	//----------------------圆（球）形查询--------------------------
	inline bool contain(int x, const point& center, long long r) { //val[x]是否在以center为圆心半径为r的圆里
		long long dist = 0;
		for (int i = 0; i < k; ++i)
			dist += sqr(val[x][i] - center[i]);
		return dist <= r * r;
	}
	int query(int x, const point& center, long long r) {
		if (x == 0 || gmin(x, center) > r * r) //当前区域与查询圆没有交
			return 0;
		if (gmax(x, center) <= r * r) //当前区域在圆内
			return sz[x];
		pushdown(x);
		return contain(x, center, r) + query(ch[x][0], center, r) + query(ch[x][1], center, r);
	}
	//-----------------查询与a曼哈顿距离最近和最远的点--------------
	inline int manhattan(const point& a, const point& b) {
		int dist = 0;
		for (int i = 0; i < k; ++i)
			dist += abs(a[i] - b[i]);
		return dist;
	}
	inline int fmin(int x, const point& a) {
		int ret = 0;
		for (int i = 0; i < k; ++i) //如果坐标范围可以到2e9，下面的加法会溢出
			ret += max(mn[x][i] - a[i], 0) + max(a[i] - mx[x][i], 0);
		return ret;
	}
	inline int fmax(int x, const point& a) {
		int ret = 0;
		for (int i = 0; i < k; ++i) //如果坐标范围可以到2e9，下面的加法会溢出
			ret += max(abs(mn[x][i] - a[i]), abs(mx[x][i] - a[i]));
		return ret;
	}
	void querymin(int x, const point& a) { //查询之前应先将ans设为无穷大
		ans = min(ans, manhattan(val[x], a));
		int f[2];
		f[0] = ch[x][0] ? fmin(ch[x][0], a) : inf;
		f[1] = ch[x][1] ? fmin(ch[x][1], a) : inf;
		int d = f[0] >= f[1];
		if (f[d] < ans)
			querymin(ch[x][d], a);
		if (f[!d] < ans)
			querymin(ch[x][!d], a);
	}
	void querymax(int x, const point& a) { //查询之前应先将ans设为无穷小
		ans = max(ans, manhattan(val[x], a));
		int f[2];
		f[0] = ch[x][0] ? fmax(ch[x][0], a) : -inf;
		f[1] = ch[x][1] ? fmax(ch[x][1], a) : -inf;
		int d = f[0] <= f[1];
		if (f[d] > ans)
			querymax(ch[x][d], a);
		if (f[!d] > ans)
			querymax(ch[x][!d], a);
	}
	//-----------------查询与a欧几里得距离最近和最远的点------------
	//如果要查询欧几里得距离，应当将point的类型改为array<long long, k>，同时调大inf的值
	inline long long euclid(const point& a, const point& b) {
		long long dist = 0;
		//如果坐标范围可以到2e9且k>=3，则dist就会超出long long的表示范围
		//可以考虑用double或long double存储
		for (int i = 0; i < k; ++i)
			dist += sqr((long long)a[i] - b[i]);
		return dist;
	}
	inline long long gmin(int x, const point& a) { //点a到x表示区域的最近欧氏距离
		long long ret = 0;
		for (int i = 0; i < k; ++i) //可能超出long long的表示范围
			ret += sqr(max(mn[x][i] - a[i], 0)) + sqr(max(a[i] - mx[x][i], 0));
		return ret;
	}
	inline long long gmax(int x, const point& a) { //点a到x表示区域的最远欧氏距离
		long long ret = 0;
		for (int i = 0; i < k; ++i) //可能超出long long的表示范围
			ret += max(sqr(mn[x][i] - a[i]), sqr(mx[x][i] - a[i]));
		return ret;
	}
	//注意ans保存的是距离的平方
	void findmin(int x, const point& a) { //查询之前应先将ans设为无穷大
		//此处乘1LL，只用于与前面的模板同步，没有实际意义
		ans = min(1LL * ans, euclid(val[x], a)); //ans应当用long long或double存储
		long long f[2];
		f[0] = ch[x][0] ? gmin(ch[x][0], a) : inf;
		f[1] = ch[x][1] ? gmin(ch[x][1], a) : inf;
		int d = f[0] >= f[1];
		if (f[d] < ans)
			findmin(ch[x][d], a);
		if (f[!d] < ans)
			findmin(ch[x][!d], a);
	}
	void findmax(int x, const point& a) { //查询之前应先将ans设为无穷小
		ans = max(1LL * ans, euclid(val[x], a)); //ans应当用long long或double存储
		long long f[2];
		f[0] = ch[x][0] ? gmax(ch[x][0], a) : -inf;
		f[1] = ch[x][1] ? gmax(ch[x][1], a) : -inf;
		int d = f[0] <= f[1];
		if (f[d] > ans)
			findmax(ch[x][d], a);
		if (f[!d] > ans)
			findmax(ch[x][!d], a);
	}
	//-----------------动态KD-Tree：插入删除操作---------------------
	//考虑到算法整体的时间复杂度瓶颈不在插入删除上，此处直接重用了前面的build函数来重构，
	//单次时间复杂度O(nlogn)，可以通过直接取中间的位置优化为单次O(n)
	const double alpha = 0.6;
	int* pos, length, arr[maxn];
	decltype(build<0>)* func;
	void dfs(int x) {
		if (!x) return;
		pushdown(x);
		dfs(ch[x][0]);
		if (mark[x]) //被标记为删除的点不参与重构，若没有删除则直接执行下面的操作，不需要if判断
			arr[++length] = x; //如果结点还有权值等属性的话也要一起参与重构
		dfs(ch[x][1]);
	}
	template<int d = 0> void add(int& x, point a) {
		if (!x) {
			x = newnode(a);
			return;
		}
		pushdown(x);
		if (a[d] < val[x][d])
			add<(d + 1) % k>(ch[x][0], a);
		else
			add<(d + 1) % k>(ch[x][1], a);
		pushup(x);
		if (sz[ch[x][0]] > sz[x] * alpha || sz[ch[x][1]] > sz[x] * alpha)
			pos = &x, func = build<d>;
	}
	void insert(int& x, point a) { //应当保证没有重复的点
		pos = nullptr;
		add(x, a);
		if (pos) {
			length = 0;
			dfs(*pos);
			*pos = func(1, length, arr);
		}
	}
	template<int d = 0> void del(int& x, point a) {
		if (!x)
			return;
		pushdown(x);
		if (a == val[x])
			mark[x] = 0; //如果还有其他标记的话也要一起清空
		else if (a[d] < val[x][d])
			del<(d + 1) % k>(ch[x][0], a);
		else
			del<(d + 1) % k>(ch[x][1], a);
		pushup(x);
	}
	void remove(int& x, point a) { //注意在查询操作的时候不要考虑已经被删掉的点
		del(x, a);
		if (real[x] >= sz[x] / 2) { //超过一半的结点被删除则重建树
			length = 0;
			dfs(x);
			x = func(1, length, arr);
		}
	}
};
point a[maxn];
int node[maxn];
int main() {
	freopen("in.txt", "r", stdin);
	kdt::init();
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i)
		scanf("%d %d", &a[i][0], &a[i][1]);
	for (int i = 0; i < n; ++i)
		node[i] = kdt::newnode(a[i]);
	int root = kdt::build(0, n, node);
	return 0;
}