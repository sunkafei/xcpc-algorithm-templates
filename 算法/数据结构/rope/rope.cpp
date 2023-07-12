/*
// #include <ext/rope>
// using namespace __gnu_cxx;
rope<int> r;
1. r.push_back(a) 往后插入a
   r.pop_back() 删除最后一个元素
2. r.insert(idx, a) 在下标idx处插入a
   r.insert(idx, rope) 在下标idx处插入rope对象，时间复杂度O(log(n))
3. r.erase(idx, length) 删除从idx开始的length个字符
4. r.replace(idx, a) 将下标idx的位置设为a
5. r.substr(pos, length) 返回从pos开始长度为length的子串，时间复杂度O(log(n))，
   修改substr返回的rope不会改变原rope。
6. 支持基本的字符串运算比如==、+=、<，拼接的时间复杂度为O(log(n))
7. rope的拷贝是O(1)的，修改是O(log(n))的。
8. 注意r[i]返回值是左值，不能修改。修改需调用replace函数。
*/
#include <bits/stdc++.h>
#include <ext/rope>
using namespace __gnu_cxx;
const int maxn = 110000;
rope<int> version[maxn];
int main() {
    const int n = 100000;
    for (int i = 0; i < n; ++i)
        version[0].push_back(i);
    for (int i = 1; i < n; ++i) {
        version[i] = version[i - 1];
        version[i].replace(i, i * 10);
    }
    const int pos = 88888;
    printf("%d %d %d\n", version[pos][pos], version[pos][pos + 1], version[pos + 1][pos + 1]);

    rope<int> r;
    r.push_back(0); r.push_back(1);
    for (int i = 0; i < 32; ++i)
        r += r;
    int total = 0;
    for (int i = 0; i < n; ++i)
        total += r[998244353 + i];
    printf("%d\n", total);
    return 0;
}
