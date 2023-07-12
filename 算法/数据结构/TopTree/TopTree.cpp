/*
K=0 表示子树修改，后面x,y，表示以x为根的子树的点权值改成y
K=1 表示换根，后面x，表示把这棵树的根变成x
K=2 表示链修改，后面x,y,z，表示把这棵树中x-y的路径上点权值改成z
K=3 表示子树询问min，后面x，表示以x为根的子树中点的权值min
K=4 表示子树询问max，后面x，表示以x为根的子树中点的权值max
K=5 表示子树加，后面x,y，表示x为根的子树中点的权值+y
K=6 表示链加，后面x,y,z，表示把这棵树中x-y的路径上点权值改成+z
K=7 表示链询问min，后面x,y，表示把这棵树中x-y的路径上点的min
K=8 表示链询问max，后面x,y，表示把这棵树中x-y的路径上点的max
K=9 表示换父亲，后面x,y，表示把x的父亲换成y，如果y在x子树里不操作。
K=10 表示链询问sum，后面x,y,z，表示表示把这棵树中x-y的路径上点的sum
K=11 表示子树询问sum，后面x，表示以x为根的子树的点权sum
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <climits>
using namespace std;
//const int E_ALL = 0, E_CH = 1, E_RE = 2;
//const int S_LCT = 0, S_AAA = 2;
const int maxn = 110000 << 2, inf = INT_MAX;
struct mark { //splay树上的标记类
	int k, b;
	mark(int k = 1, int b = 0) :k(k), b(b) {} //k == 0表示设置值，k == 1表示增加值，b表示修改或增加的值
	int operator ()(int x, int sz = 1) { //将当前标记类作用于值x（sz指定值的个数）
		return k * x + sz * b;
	}
	mark& operator +=(mark a) { //将当前标记与a结合
		b = b * a.k + a.b; //计算新的标记值
		k *= a.k; //若两个标记中有一个为set则结果为set
		return *this;
	}
	bool operator !=(mark a) {
		return k != a.k || b != a.b;
	}
}none;
struct info { //splay树上的信息类
	int minv, maxv, sumv, sz;
	info(int minv = inf, int maxv = -inf, int sumv = 0, int sz = 0) :minv(minv), maxv(maxv), sumv(sumv), sz(sz) {}
	info operator +(info rhs) { //将信息进行合并
		return info(min(minv, rhs.minv), max(maxv, rhs.maxv), sumv + rhs.sumv, sz + rhs.sz);
	}
	info& operator +=(mark b) { //将标记作用于信息
		if (sz) {
			minv = b(minv);
			maxv = b(maxv);
			sumv = b(sumv, sz);
		}
		return *this;
	}
};
struct node {
	node* son[4], * fa;
	info se, re, ch, all;
	mark mch, mre;
	bool rev, imag;
	node() { //construction for null node
		fill(son, son + 4, fa = 0);
		se = re = ch = all = info();
		mch = mre = none;
		rev = imag = 0;
	}
	int kind() {
		for (int i = 0; i < 4; ++i)
			if (fa->son[i] == this)
				return i;
	}
	bool isroot(int type) {
		return type ? (!fa->imag || !imag) : (fa->son[0] != this && fa->son[1] != this);
	}
	void link(int d, node* s) {
		son[d] = s;
		s->fa = this;
	}
	void set(node* s, int d) {
		down();
		son[d] = s;
		s->fa = this;
		up();
	}
	void reverse() {
		swap(son[0], son[1]);
		rev ^= 1;
	}
	void edit(mark f, int type) {
		//if (this == null) return;
		if (type == 0 || type == 1) {
			se += f;
			mch += f;
			ch += f;
		}
		if (type == 0 || type == 2) {
			mre += f;
			re += f;
		}
		all = ch + re;
	}
	void up() {
		ch = son[0]->ch + son[1]->ch + se;
		re = son[0]->re + son[1]->re + son[2]->all + son[3]->all;
		all = ch + re;
	}
	void down() {
		if (rev) {
			son[0]->reverse();
			son[1]->reverse();
			rev = 0;
		}
		if (mre != none) {
			son[0]->edit(mre, 2);
			son[1]->edit(mre, 2);
			son[2]->edit(mre, 0);
			son[3]->edit(mre, 0);
			mre = none;
		}
		if (mch != none) {
			son[0]->edit(mch, 1);
			son[1]->edit(mch, 1);
			mch = none;
		}

	}
}pool[maxn], *me = pool, *bin[maxn], **ptr = bin, *null = new(me++) node, *nd[maxn], *root;
node* newnode(int val = inf) { //val表示结点的初始权值
	node* cur = *ptr ? *ptr-- : me++;
	fill(cur->son, cur->son + 4, cur->fa = null);
	cur->re = info();
	cur->mch = cur->mre = none;
	cur->rev = 0;
	if (val == inf) {
		cur->se = cur->all = cur->ch = info();
		cur->imag = 1;
	}
	else {
		cur->se = cur->all = cur->ch = info(val, val, val, 1);
		cur->imag = 0;
	}
	return cur;
}
void delnode(node*& pos) {
	*++ptr = pos;
	pos = 0;
}
void trans(node* pos) {
	node* fa = pos->fa, * grf = fa->fa;
	fa->down(), pos->down();
	int d = pos->kind();
	if (grf != null)
		grf->son[fa->kind()] = pos;
	pos->fa = grf;
	fa->link(d, pos->son[d ^ 1]);
	pos->link(d ^ 1, fa);
	fa->up();
}
void splay(node* pos, int type) {
	while (!pos->isroot(type)) {
		node* fa = pos->fa;
		if (!fa->isroot(type))
			trans(fa->kind() == pos->kind() ? fa : pos);
		trans(pos);
	}
	pos->up();
}
void addvirt(node* pos, node* fa) {
	if (pos == null) return;
	fa->down();
	for (int i = 2; i < 4; ++i) {
		if (fa->son[i] == null) {
			fa->set(pos, i);
			return;
		}
	}
	while (fa->son[2]->imag)
		fa = fa->son[2];
	splay(fa, 2);
	node* vi = newnode();
	vi->link(2, fa->son[2]);
	vi->link(3, pos);
	fa->set(vi, 2);
	splay(vi, 2);
}
void delvirt(node* pos) {
	if (pos == null) return;
	static node** top = (node * *)malloc(maxn * sizeof(node*));
	for (node* p = pos->fa; p->imag; p = p->fa)* ++top = p;
	if (*top) (*top)->fa->down();
	for (; *top; top--) (*top)->down();
	node* fa = pos->fa;
	int d = pos->kind();
	if (fa->imag) {
		fa->fa->set(fa->son[d ^ 1], fa->kind());
		splay(fa->fa, 2);
		delnode(fa);
	}
	else {
		fa->set(null, d);
		splay(fa, 2);
	}
	pos->fa = null;
}
void access(node* pos) {
	node* pred = null;
	while (pos != null) {
		splay(pos, 0);
		addvirt(pos->son[1], pos);
		delvirt(pred);
		pos->set(pred, 1);
		pred = pos;
		for (pos = pos->fa; pos->imag; pos = pos->fa);
	}
}
void touch(node* pos) {
	access(pos);
	splay(pos, 0);
}
void beroot(node* pos) { //将结点pos设置为根结点
	touch(pos);
	pos->reverse();
}
void link(node* x, node* y) {
	beroot(x);
	touch(y);
	addvirt(x, y);
}
void cut(node* x, node* y) {
	beroot(x);
	touch(y);
	y->set(null, 0);
	x->fa = null;
}
node* findroot(node* pos) {
	touch(pos);
	while (pos->son[0] != null) {
		pos->down();
		pos = pos->son[0];
	}
	splay(pos, 0);
	return pos;
}
node* findfa(node* pos) {
	touch(pos);
	pos = pos->son[0];
	if (pos == null) return 0;
	while (pos->son[1] != null) {
		pos->down();
		pos = pos->son[1];
	}
	splay(pos, 0);
	return pos;
}
void changefather(node *pos, node *father) {
	if (pos == root) return;
	node* fa = findfa(pos);
	cut(fa, pos);
	if (findroot(pos) != findroot(father))
		link(pos, father);
	else
		link(pos, fa);
}
namespace chain {
	info query(node *x, node *y) { //对x到y的链进行查询
		beroot(x);
		touch(y);
		return y->ch;
	}
	void edit(node *x, node *y, mark f) //对x到y的链进行修改
	{
		beroot(x);
		touch(y);
		y->edit(f, 1);
	}
}
namespace sub {
	info query(node *rt) { //在以root为根的情况下，对rt子树进行查询
		touch(rt);
		return rt->se + rt->son[2]->all + rt->son[3]->all;
	}
	void edit(node *rt, mark f) { //在以root为根的情况下，对rt子树进行修改
		touch(rt);
		rt->se += f;
		rt->son[2]->edit(f, 0);
		rt->son[3]->edit(f, 0);
	}
}

int main() {
#define get(x) scanf("%d",&x)
#define put(x) printf("%d",x)
	static int w[maxn], edge[maxn][2];
	int n, m, tmp; get(n), get(m);
	for (int i = 1; i <= n - 1; ++i)
		get(edge[i][0]), get(edge[i][1]);
	for (int i = 1; i <= n; ++i) {
		get(w[i]);
		nd[i] = newnode(w[i]);
	}
	get(tmp); root = nd[tmp];
	for (int i = 1; i <= n - 1; ++i)  link(nd[edge[i][0]], nd[edge[i][1]]);
	for (int i = 1; i <= m; ++i) {
		int opt, x, y, z, Ans; get(opt);
		info res;
		beroot(root);
		switch (opt)
		{
		case 0:
		case 5: //子树修改
			get(x), get(y);
			sub::edit(nd[x], mark(opt == 0 ? 0 : 1, y)); //0 set 5 add
			break;
		case 1:
			get(tmp); root = nd[tmp]; //换根
			break;
		case 2:
		case 6: //链修改
			get(x), get(y), get(z);
			chain::edit(nd[x], nd[y], mark(opt == 2 ? 0 : 1, z));
			break;
		case 3://min
		case 4://max
		case 11://sum
			get(x);
			res = sub::query(nd[x]);
			switch (opt)
			{
			case 3:Ans = res.minv; break;
			case 4:Ans = res.maxv; break;
			case 11:Ans = res.sumv; break;
			}
			put(Ans), putchar('\n');
			break;
		case 7://min
		case 8://max
		case 10://sum
			get(x), get(y);
			res = chain::query(nd[x], nd[y]);
			switch (opt)
			{
			case 7:Ans = res.minv; break;
			case 8:Ans = res.maxv; break;
			case 10:Ans = res.sumv; break;
			}
			put(Ans), putchar('\n');
			break;
		case 9: //换父亲
			get(x), get(y);
			changefather(nd[x], nd[y]);
			break;
		}
	}
	return 0;
}