#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 400000;
const int inf = 1 << 30;

struct node { //mn记录最小值，pos记录最小值所在的结点
	node* p, *ch[2], *pos;
	int mn, rev, val;
}nodes[maxn], * cur, * nil;
pair<node*, node*> edge[maxn];
node* newnode(int key) {
	cur->p = cur->ch[0] = cur->ch[1] = nil;
	cur->mn = cur->val = key;
	cur->rev = 0;
	return cur++;
}
void init() {
	cur = nodes;
	nil = newnode(inf);
}
bool isroot(node* x) {
	return x->p == nil || x->p->ch[0] != x && x->p->ch[1] != x;
}
void pushup(node* x) {
	x->mn = x->val;
	x->pos = x;
	if (x->ch[0] != nil && x->ch[0]->mn < x->mn) {
		x->mn = x->ch[0]->mn;
		x->pos = x->ch[0]->pos;
	}
	if (x->ch[1] != nil && x->ch[1]->mn < x->mn) {
		x->mn = x->ch[1]->mn;
		x->pos = x->ch[1]->pos;
	}
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
void splay(node* x) {
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
void link(node* x, node* y) { 
	access(x);
	splay(x);
	x->rev ^= 1;
	x->p = y;
}
node *link(node* x, node* y, int value) { //在结点x与结点y之间连接一条权值为v的边（必须保证x与y属于不同的树）
	node* e = newnode(value);
	link(x, e);
	link(y, e);
	edge[e - nodes] = make_pair(x, y);
	return e; //返回新建的边对应的结点
}
void cut(node* x, node* y) { //断开结点x与y之间的边
	changeroot(x); //将x置为整棵树的根
	access(y);
	splay(y);
	y->ch[0] = y->ch[0]->p = nil;
	pushup(y);
}
void cut(node* x) { //删除结点x所代表的边
	pair<node*, node*> pr = edge[x - nodes];
	cut(pr.first, x);
	cut(pr.second, x);
	edge[x - nodes] = pair<node *, node*>(0, 0);
}
bool exist(node* x) { //检查点x所对应的边是否还存在于树中（没有被删除）
	return edge[x - nodes].first != 0;
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

int main() {

}











