#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <set>
#include <ctime>
#include <random>
#define rank Rank
using namespace std;
const int maxn = 510000;
namespace treap_old {
    //在每次更新标记的时候对被更新节点的属性进行维护。 
    //在pushdown的时候不修改根节点的值。
    //在pushup的时候直接访问子树的属性，而无需考虑flip等特殊标记。
    struct node {
        node* lch, * rch;
        int r;
        int v;
        int s;
        void up() {
            s = lch->s + rch->s + 1;
        }
        void down() {
            /*
            if (rev)
            {
                rev = 0;
                lch->rev ^= 1;
                rch->rev ^= 1;
                swap(lch, rch);
            }
            */
        }
    }nodes[maxn], * cur, * nil;
    node* newnode(int x) {
        node* o = cur++;
        o->lch = o->rch = nil;
        o->r = rand();
        o->v = x;
        o->s = 1;
        return o;
    }
    void init() {
        cur = nodes;
        nil = newnode(0);
        nil->s = 0;
    }
    node* copy(node* o) {
        *cur = *o;
        return cur++;
    }
    //如果有down操作则可持久化操作放在down中，down中复制左右子树后进行标记下传
    void merge(node*& o, node* a, node* b) {
        if (a == nil) o = b;
        else if (b == nil) o = a;
        else if (a->r > b->r) {
            o = a; //o = copy(a);
            o->down();
            merge(o->rch, a->rch, b);
            //o->rch->fa = o;
            o->up();
        }
        else {
            o = b; //o = copy(b);
            o->down();
            merge(o->lch, a, b->lch);
            //o->lch->fa = o;
            o->up();
        }
    }
    void split(node* o, node*& a, node*& b, int k) {
        if (k == 0)
            a = nil, b = o;
        else if (o->s <= k)
            a = o, b = nil;
        else if (o->lch->s >= k) { //如果有序列的翻转操作则应当将o->down()放在该if语句之前，以确保正确访问左子树的大小 
            o->down();
            b = o; //b = copy(o);
            split(o->lch, a, b->lch, k);
            //a->fa = nil;
            //b->lch->fa = b;
            b->up();
        }
        else {
            o->down();
            a = o; //a = copy(o);
            split(o->rch, a->rch, b, k - o->lch->s - 1);
            //a->rch->fa = a;
            //b->fa = nil;
            a->up();
        }
    }
    bool find(node* o, int x) {
        while (o != nil) {
            if (x == o->v)
                return true;
            else if (x < o->v)
                o = o->lch;
            else
                o = o->rch;
        }
        return false;
    }
    int rank(node* o, int x) {
        if (o == nil)
            return 1;
        if (x <= o->v)
            return rank(o->lch, x);
        else return rank(o->rch, x) + o->lch->s + 1;
    }
    int kth(node* o, int k) {
        if (o == nil || k <= 0 || k > o->s) return 0;
        if (k == o->lch->s + 1) return o->v;
        else if (k <= o->lch->s) return kth(o->lch, k);
        else return kth(o->rch, k - o->lch->s - 1);
    }
    //insert(root, rank(root, x), x);
    void insert(node*& o, int k, int x) {
        node* a, * b;
        split(o, a, b, k - 1);
        merge(a, a, newnode(x));
        merge(o, a, b);
    }
    //del(root, rank(root, x));
    void del(node*& o, int k) {
        node* a, * b, * c;
        split(o, a, b, k - 1);
        split(b, b, c, 1);
        merge(o, a, c);
        //freenode(b);
    }
    //node *root = nil; //创建一棵空树 
}
namespace treap {
    struct node {
        node* lch, * rch;
        int v;
        int s;
        void up() {
            s = lch->s + rch->s + 1;
        }
    }nodes[maxn], * cur, * nil;
    node* newnode(int v) {
        node* x = cur++;
        x->lch = x->rch = nil;
        x->v = v;
        x->s = 1;
        return x;
    }
    void init() {
        cur = nodes;
        nil = newnode(0);
        nil->s = 0;
    }
    node* copy(node* x) {
        *cur = *x;
        return cur++;
    }
    node* merge(node* a, node* b) {
        if (a == nil) return b;
        if (b == nil) return a;
        if (rand() % (a->s + b->s) < a->s) { // 必须保证rand产生的随机数足够大
            node* x = copy(a);
            x->rch = merge(a->rch, b);
            x->up();
            return x;
        }
        else {
            node* x = copy(b);
            x->lch = merge(a, b->lch);
            x->up();
            return x;
        }
    }
    node* merge(node* a, node* b, node* c) {
        return merge(a, merge(b, c));
    }
    pair<node*, node*> split(node* x, int k) {
        if (k == 0)
            return { nil, x };
        if (x->s <= k)
            return { x, nil };
        if (x->lch->s >= k) {
            node* b = copy(x);
            auto [a, m] = split(x->lch, k);
            b->lch = m;
            b->up();
            return { a, b };
        }
        else {
            node* a = copy(x);
            auto [m, b] = split(x->rch, k - x->lch->s - 1);
            a->rch = m;
            a->up();
            return { a, b };
        }
    }
    tuple<node*, node*, node*> split(node* x, int L, int R) {
        auto [a, m] = split(x, L - 1);
        auto [b, c] = split(m, R - L + 1);
        return { a, b, c };
    }
    bool find(node* x, int v) {
        while (x != nil) {
            if (v == x->v)
                return true;
            else if (v < x->v)
                x = x->lch;
            else
                x = x->rch;
        }
        return false;
    }
    int rank(node* x, int v) { //v在x中是第几大的值
        if (x == nil)
            return 1;
        if (v <= x->v)
            return rank(x->lch, v);
        else
            return rank(x->rch, v) + x->lch->s + 1;
    }
    int kth(node* x, int k) {
        if (x == nil || k <= 0 || k > x->s) return 0;
        if (k == x->lch->s + 1) return x->v;
        else if (k <= x->lch->s) return kth(x->lch, k);
        else return kth(x->rch, k - x->lch->s - 1);
    }
    //insert(root, rank(root, x), x);
    [[nodiscard]] node* insert(node* x, int k, int v) {
        auto [a, b] = split(x, k - 1);
        return merge(a, newnode(v), b);
    }
    //del(root, rank(root, x));
    [[nodiscard]] node* del(node* x, int k) {
        auto [a, m] = split(x, k - 1);
        auto [v, b] = split(m, 1);
        return merge(a, b);
    }
    //node *root = nil; //创建一棵空树 
}
namespace shared_treap { //用智能指针优化可持久化treap的内存
    struct node;
    struct pointer {
        node* ptr;
        pointer() : ptr(nullptr) {}
        pointer(node* ptr);
        pointer(const pointer& ptr);
        pointer& operator= (const pointer& rhs);
        operator node* () const;
        node* operator-> ();
        node& operator* ();
        ~pointer();
        void del();
    };
    struct node {
        pointer lch, rch;
        int v, s, ref;
        void up();
    }nodes[maxn];
    node* const nil = nodes;
    vector<node*> pool;
    inline pointer::pointer(const pointer& ptr) : pointer(ptr.ptr) {}
    inline pointer::pointer(node* ptr) : ptr(ptr) {
        if (ptr)
            ptr->ref += 1;
    }
    inline pointer& pointer::operator= (const pointer& rhs) {
        if (ptr != rhs.ptr) {
            del();
            ptr = rhs.ptr;
            if (ptr)
                ptr->ref += 1;
        }
        return *this;
    }
    inline node* pointer::operator-> () {
        return ptr;
    }
    inline pointer::operator node* () const {
        return ptr;
    }
    inline void pointer::del() {
        if (ptr) {
            ptr->ref -= 1;
            if (ptr->ref == 0) {
                ptr->lch = ptr->rch = nullptr;
                pool.push_back(ptr);
            }
        }
    }
    inline pointer::~pointer() {
        del();
    }
    inline void node::up() {
        s = lch->s + rch->s + 1;
    }
    void init() {
        pool.clear();
        for (int i = 1; i < maxn; ++i)
            pool.push_back(nodes + i);
    }
    pointer newnode(int v) {
        node* x = pool.back(); //类型是node* !!!!!
        pool.pop_back();
        x->lch = x->rch = nil;
        x->v = v;
        x->s = 1;
        x->ref = 0;
        return x;
    }
    inline node* copy(node* x) {
        if (x == nil) return x;
        node* y = pool.back(); pool.pop_back();
        *y = *x;
        y->ref = 0;
        return y;
    }
    pointer merge(node* a, node* b) {
        if (a == nil) return b;
        if (b == nil) return a;
        if (rand() % (a->s + b->s) < a->s) { // 必须保证rand产生的随机数足够大
            auto x = copy(a);
            x->rch = merge(a->rch, b);
            x->up();
            return x;
        }
        else {
            auto x = copy(b);
            x->lch = merge(a, b->lch);
            x->up();
            return x;
        }
    }
    pointer merge(node* a, node* b, node* c) {
        return merge(a, merge(b, c));
    }
    pair<pointer, pointer> split(node* x, int k) {
        if (k == 0)
            return { nil, x };
        if (x->s <= k)
            return { x, nil };
        if (x->lch->s >= k) {
            pointer b = copy(x);
            auto [a, m] = split(x->lch, k);
            b->lch = m;
            b->up();
            return { a, b };
        }
        else {
            pointer a = copy(x);
            auto [m, b] = split(x->rch, k - x->lch->s - 1);
            a->rch = m;
            a->up();
            return { a, b };
        }
    }
    tuple<pointer, pointer, pointer> split(node* x, int L, int R) {
        auto [a, m] = split(x, L - 1);
        auto [b, c] = split(m, R - L + 1);
        return { a, b, c };
    }
    bool find(node* x, int v) {
        while (x) {
            if (v == x->v)
                return true;
            else if (v < x->v)
                x = x->lch;
            else
                x = x->rch;
        }
        return false;
    }
    int rank(node* x, int v) { //v在x中是第几大的值
        if (x == nil)
            return 1;
        if (v <= x->v)
            return rank(x->lch, v);
        else
            return rank(x->rch, v) + x->lch->s + 1;
    }
    int kth(node* x, int k) {
        if (x == nil || k <= 0 || k > x->s) return 0;
        if (k == x->lch->s + 1) return x->v;
        else if (k <= x->lch->s) return kth(x->lch, k);
        else return kth(x->rch, k - x->lch->s - 1);
    }
    //insert(root, rank(root, x), x);
    [[nodiscard]] pointer insert(node* x, int k, int v) {
        auto [a, b] = split(x, k - 1);
        return merge(a, newnode(v), b);
    }
    //del(root, rank(root, x));
    [[nodiscard]] pointer del(node* x, int k) {
        auto [a, m] = split(x, k - 1);
        auto [v, b] = split(m, 1);
        return merge(a, b);
    }
    //pointer root = nil; //创建一棵空树 
}
namespace shared_treap_pushdown {
    struct node;
    struct pointer {
        node* ptr;
        pointer() : ptr(nullptr) {}
        pointer(node* ptr);
        pointer(const pointer& ptr);
        pointer& operator= (const pointer& rhs);
        operator node* () const;
        node* operator-> ();
        node& operator* ();
        ~pointer();
        void del();
    };
    struct node {
        pointer lch, rch;
        int s, ref;
        int val, add;
        long long sum;
        void up();
        void down();
        void mark(int tag) { //在mark之前要先copy当前结点！！！
            add += tag;
            val += tag;
            sum += 1LL * tag * s;
        }
    }nodes[maxn];
    node* const nil = nodes;
    vector<node*> pool;
    inline pointer::pointer(const pointer& ptr) : pointer(ptr.ptr) {}
    inline pointer::pointer(node* ptr) : ptr(ptr) {
        if (ptr)
            ptr->ref += 1;
    }
    inline pointer& pointer::operator= (const pointer& rhs) {
        if (ptr != rhs.ptr) {
            del();
            ptr = rhs.ptr;
            if (ptr)
                ptr->ref += 1;
        }
        return *this;
    }
    inline node* pointer::operator-> () {
        return ptr;
    }
    inline pointer::operator node* () const {
        return ptr;
    }
    inline void pointer::del() {
        if (ptr) {
            ptr->ref -= 1;
            if (ptr->ref == 0) {
                ptr->lch = ptr->rch = nullptr;
                pool.push_back(ptr);
            }
        }
    }
    inline pointer::~pointer() {
        del();
    }
    void init() {
        pool.clear();
        for (int i = 1; i < maxn; ++i)
            pool.push_back(nodes + i);
    }
    pointer newnode(int v) {
        node* x = pool.back(); //类型是node* !!!!!
        pool.pop_back();
        x->lch = x->rch = nil;
        x->val = x->sum = v;
        x->add = 0;
        x->s = 1;
        x->ref = 0;
        return x;
    }
    inline node* copy(node* x) {
        if (x == nil) return x;
        node* y = pool.back(); pool.pop_back();
        *y = *x;
        y->ref = 0;
        return y;
    }
    pointer merge(node* a, node* b) {
        if (a == nil) return b;
        if (b == nil) return a;
        if (rand() % (a->s + b->s) < a->s) { // 必须保证rand产生的随机数足够大
            auto x = copy(a);
            x->down();
            x->rch = merge(x->rch, b);
            x->up();
            return x;
        }
        else {
            auto x = copy(b);
            x->down();
            x->lch = merge(a, x->lch);
            x->up();
            return x;
        }
    }
    inline pointer merge(node* a, node* b, node* c) {
        return merge(a, merge(b, c));
    }
    pair<pointer, pointer> split(node* x, int k) {
        if (k == 0)
            return { nil, x };
        if (x->s <= k)
            return { x, nil };
        x = copy(x);
        x->down();
        if (x->lch->s >= k) {
            auto [a, m] = split(x->lch, k);
            x->lch = m;
            x->up();
            return { a, x };
        }
        else {
            auto [m, b] = split(x->rch, k - x->lch->s - 1);
            x->rch = m;
            x->up();
            return { x, b };
        }
    }
    inline tuple<pointer, pointer, pointer> split(node* x, int L, int R) {
        auto [a, m] = split(x, L - 1);
        auto [b, c] = split(m, R - L + 1);
        return { a, b, c };
    }
    inline void node::up() {
        s = lch->s + rch->s + 1;
        sum = lch->sum + rch->sum + val;
    }
    inline void node::down() {
        if (add) {
            if (lch.ptr != nil) {
                lch = copy(lch.ptr);
                lch->mark(add);
            }
            if (rch.ptr != nil) {
                rch = copy(rch.ptr);
                rch->mark(add);
            }
            add = 0;
        }
    }
}
int main() {
    using namespace shared_treap_pushdown;
    init();
    pointer root = nil;

    return 0;
}
