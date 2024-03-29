#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 400000;
const int inf = 1 << 30;

struct node {
	node* p, * ch[2];
	int mx, rev, val, add;
}nodes[maxn], * cur, * nil;
node* newnode(int key) {
	cur->p = cur->ch[0] = cur->ch[1] = nil;
	cur->mx = cur->val = key;
	cur->add = cur->rev = 0;
	return cur++;
}
void init() {
	cur = nodes;
	nil = newnode(-inf);
}
bool isroot(node* x) {
	return x->p == nil || x->p->ch[0] != x && x->p->ch[1] != x;
}
void increase(node* x, int v) {
	x->val += v;
	x->add += v;
	x->mx += v;
}
void pushup(node* x) {
	x->mx = x->val;
	if (x->ch[0] != nil)
		x->mx = max(x->mx, x->ch[0]->mx);
	if (x->ch[1] != nil)
		x->mx = max(x->mx, x->ch[1]->mx);
}
void pushdown(node* x) {
	if (x->rev) {
		x->rev = 0;
		if (x->ch[0] != nil) x->ch[0]->rev ^= 1;
		if (x->ch[1] != nil) x->ch[1]->rev ^= 1;
		swap(x->ch[0], x->ch[1]);
	}
	if (x->add) {
		if (x->ch[0] != nil)
			increase(x->ch[0], x->add);
		if (x->ch[1] != nil)
			increase(x->ch[1], x->add);
		x->add = 0;
	}
}
void rotate(node* x, int d) {
	if (isroot(x)) return;
	node* y = x->p;
	y->ch[!d] = x->ch[d];
	x->ch[d]->p = y;
	x->p = y->p;
	if (!isroot(y))
		y->p->ch[y == y->p->ch[1]] = x;
	x->ch[d] = y;
	y->p = x;
	pushup(y);
}
void splay(node* x) { //若要修改一个点的权值则要先将其伸展到根
	static node* sta[maxn];
	int top = 1;
	sta[0] = x;
	for (node* y = x; !isroot(y); y = y->p)
		sta[top++] = y->p;
	while (top) pushdown(sta[--top]);
	while (!isroot(x)) {
		node* y = x->p;
		if (isroot(y))
			rotate(x, x == y->ch[0]);
		else {
			int d = y == y->p->ch[0];
			if (x == y->ch[d])
				rotate(x, !d);
			else
				rotate(y, d);
			rotate(x, d);
		}
	}
	pushup(x);
}
node* access(node* x) { //打通结点x到根结点的路径
	node* y = nil;
	while (x != nil) {
		splay(x);
		y->p = x;
		x->ch[1] = y;
		pushup(x);
		y = x;
		x = x->p;
	}
	return y;
}
void changeroot(node* x) { //将x置为整棵树的根结点
	access(x)->rev ^= 1;
}
void link(node* x, node* y) { //将结点x与y连接起来（必须保证x与y属于不同的树）
	access(x);
	splay(x);
	x->rev ^= 1;
	x->p = y;
}
void cut(node* x) { //断开结点x与它的父结点之间的边
	access(x);
	splay(x);
	x->ch[0] = x->ch[0]->p = nil;
	pushup(x);
}
void cut(node* x, node* y) { //断开结点x与y之间的边
	changeroot(x); //将x置为整棵树的根
	cut(y); //删除y与其父结点之间的边
}
node* getroot(node* x) { //返回结点x所在的树当前的根结点
	access(x);
	splay(x);
	while (pushdown(x), x->ch[0] != nil)
		x = x->ch[0];
	splay(x);
	return x;
}
bool sametree(node* x, node* y) { //判断结点x与y是否属于同一棵树
	changeroot(x);
	return getroot(y) == x;
}

int n, m, x, y, w;
int eu[maxn], ev[maxn];
int main()
{
	//freopen("in.txt", "r", stdin);
	while (scanf("%d", &n) != -1)
	{
		init();
		for (int i = 1; i < n; i++)
			scanf("%d%d", &eu[i], &ev[i]);
		for (int i = 1; i <= n; i++)
		{
			int a;
			scanf("%d", &a);
			newnode(a);
		}
		for (int i = 1; i < n; i++)
			link(nodes + eu[i], nodes + ev[i]);
		scanf("%d", &m);
		for (int i = 1; i <= m; i++)
		{
			scanf("%d", &x);
			if (x == 1)
			{
				scanf("%d%d", &x, &y);
				if (sametree(nodes + x, nodes + y))
				{
					printf("-1\n");
					continue;
				}
				link(nodes + x, nodes + y);
			}
			else if (x == 2)
			{
				scanf("%d%d", &x, &y);
				if (x == y || !sametree(nodes + x, nodes + y))
				{
					printf("-1\n");
					continue;
				}
				changeroot(nodes + x);
				cut(nodes + y);
			}
			else if (x == 3)
			{
				scanf("%d%d%d", &w, &x, &y);
				if (!sametree(nodes + x, nodes + y))
				{
					printf("-1\n");
					continue;
				}
				changeroot(nodes + x);
				access(nodes + y);
				splay(nodes + y);
				node* q = nodes + y;
				increase(q, w);
			}
			else
			{
				scanf("%d%d", &x, &y);
				if (!sametree(nodes + x, nodes + y))
				{
					printf("-1\n");
					continue;
				}
				changeroot(nodes + x);
				access(nodes + y);
				splay(nodes + y);
				printf("%d\n", (nodes + y)->mx);
			}
		}
		printf("\n");
	}
	return 0;
}











