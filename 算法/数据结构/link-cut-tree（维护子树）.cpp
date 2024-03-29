/*
用link-cut-tree维护子树信息：对于维护的信息，每个点要开两个属性，其中一个是在原树中的子树信息，
另一个是在LCT中虚子树的信息。除了要在pushup中维护这些信息外，还要在access和link函数中维护。
若要访问一个结点x的子树信息，应当先将其access到根，此时x->size表示x的子树中除去x外所有结点的信息（大小），
与x自身的信息结合即可得到x的子树信息（x->size + 1即为子树大小）。
若要访问整棵树的信息，可以考虑结合changeroot函数，并使用结点的sum信息。
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 400000;


struct node {
	node* p, * ch[2];
	int rev, sum, size; //sum维护当前点的虚子树与实子树大小之和，size是在LCT中虚子树的大小
}nodes[maxn], * cur, * nil;
node* newnode() {
	cur->p = cur->ch[0] = cur->ch[1] = nil;
	cur->sum = 1;
	cur->size = 0;
	cur->rev = 0;
	return cur++;
}
void init() {
	cur = nodes;
	nil = newnode();
	nil->sum = 0;
}
bool isroot(node* x) {
	return x->p == nil || x->p->ch[0] != x && x->p->ch[1] != x;
}
void pushup(node* x) {
	x->sum = x->ch[0]->sum + x->ch[1]->sum + x->size + 1;
}
void pushdown(node* x) {
	if (x->rev) {
		x->rev = 0;
		if (x->ch[0] != nil) x->ch[0]->rev ^= 1;
		if (x->ch[1] != nil) x->ch[1]->rev ^= 1;
		swap(x->ch[0], x->ch[1]);
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
		x->size += x->ch[1]->sum - y->sum; //动态维护虚子树信息
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
	splay(x); //将x伸展到splay的根结点，这样才能保证link的正确性
}
void link(node* x, node* y) { //将结点x与y连接起来（必须保证x与y属于不同的树且不为nil结点）
	changeroot(x);
	changeroot(y);
	x->p = y;
	y->size += x->sum; //动态维护虚子树信息
	pushup(y);
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



node* nd[maxn];
int main() {
	//freopen("in.txt", "r", stdin);
	int n, q;
	scanf("%d %d", &n, &q);
	init();
	for (int i = 1; i <= n; ++i)
		nd[i] = newnode();
	while (q--) {
		char tp;
		int x, y;
		scanf(" %c %d %d", &tp, &x, &y);
		if (tp == 'A') {
			link(nd[x], nd[y]);
		}
		else {
			changeroot(nd[x]);
			access(nd[y]);
			long long L = nd[y]->size + 1; //将虚子树的大小加1，即为子树大小。
			changeroot(nd[y]);
			access(nd[x]);
			long long R = nd[x]->size + 1;
			printf("%lld\n", L * R);
		}
	}
	return 0;
}









