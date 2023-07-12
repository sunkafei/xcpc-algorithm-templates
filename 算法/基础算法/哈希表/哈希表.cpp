#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <random>
#include <vector>
#include <climits>
#include <ctime>
using namespace std;
//key_t应当为整数类型，且实际值必须非负
template<typename key_t, typename type> struct hash_table {
	static const int maxn = 1000010;
	static const int table_size = 11110007;
	int first[table_size], nxt[maxn], sz; //init: memset(first, 0, sizeof(first)), sz = 0
	key_t id[maxn];
	type data[maxn];
	type& operator[] (key_t key) {
		const int h = key % table_size;
		for (int i = first[h]; i; i = nxt[i])
			if (id[i] == key)
				return data[i];
		int pos = ++sz;
		nxt[pos] = first[h];
		first[h] = pos;
		id[pos] = key;
		return data[pos] = type();
	}
	bool count(key_t key) {
		for (int i = first[key % table_size]; i; i = nxt[i])
			if (id[i] == key)
				return true;
		return false;
	}
	type get(key_t key) { //如果key对应的值不存在，则返回type()。
		for (int i = first[key % table_size]; i; i = nxt[i])
			if (id[i] == key)
				return data[i];
		return type();
	}
};

unordered_map<long long, long long> A;
hash_table<long long, long long> B;
const int maxn = 1000000;
int main() {
	default_random_engine e;
	uniform_int_distribution<long long> d(0, LLONG_MAX);
	for (int i = 0; i < maxn; ++i) {
		long long key = d(e);
		long long value = d(e);
		A[key] = value;
		B[key] = value;
	}
	for (int i = 0; i < maxn * 10; ++i) {
		long long key = d(e);
		if (A.count(key) != B.count(key))
			abort();
		if (A.count(key)) {
			if (A[key] != B.get(key))
				abort();
		}
	}
	return 0;
}