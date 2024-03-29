#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
struct node {
	node *ch[2]; // ×óÓÒ×ÓÊ÷
	int height, value;
	node() : height(0), value(0), ch() {}
	void maintain() {
		height = max(ch[0]->height, ch[1]->height) + 1;
	}
}*nil = new node;
void rotate(node* &o, int d) {
	node* k = o->ch[d ^ 1];
	o->ch[d ^ 1] = k->ch[d];
	k->ch[d] = o;
	o->maintain(); k->maintain();
	o = k;
}
node *Insert(int X, node *T)
{
	if (T == nil) {
		T = new node;
		T->value = X;
		T->height = 0;
		T->ch[0] = T->ch[1] = nil;
	}
	else if (X < T->value) {
		T->ch[0] = Insert(X, T->ch[0]);
		if (T->ch[0]->height - T->ch[1]->height == 2) {
			if (X >= T->ch[0]->value)
				rotate(T->ch[0], 0);
			rotate(T, 1);
		}
	}
	else if (X > T->value) {
		T->ch[1] = Insert(X, T->ch[1]);
		if (T->ch[1]->height - T->ch[0]->height == 2){
			if (X <= T->ch[1]->value)
				rotate(T->ch[1], 1);
			rotate(T, 0);
		}
	}
	T->height = max(T->ch[0]->height, T->ch[1]->height) + 1;
	return T;
}

int main() {
	//	freopen("in.txt", "r", stdin);
	int n;
	scanf("%d", &n);
	node *root = nil;
	for (int i = 0; i < n; ++i) {
		int val;
		scanf("%d", &val);
		root = Insert(val, root);
	}
	printf("%d\n", root->value);
	return 0;
}