/*
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>
// using namespace __gnu_pbds;
tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> t;
1. 第二个模板参数为值域的类型，如果只要实现set的功能，则用null_type即可
2. 基本操作与std::map/std::set兼容
3. t.erase(val) 从平衡树中删除值为val的结点
4. t.order_of_key(val) 返回平衡树中比val小的数有多少个
5. t.find_by_order(k) 返回排名为k的位置的迭代器（排名从0开始）
6. 不支持多重值，如果需要多重值，可以再开一个unordered_map来记录值出现的次数。
   将val<<32后加上出现的次数后插入tree，注意此时应该为long long类型。
*/
#include <cstdio>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;
const int maxn = 110000;
tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> t;
int main() {
    for (int i = 1; i <= 5; ++i)
        t.insert(i);
    printf("=>%d\n", *t.find_by_order(2));
    return 0;
}
