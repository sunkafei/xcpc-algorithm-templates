#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <functional>
#include <queue>
using namespace std;
template<typename T> class heap {
private:
	priority_queue<T> Q, R;
public:
	void maintain() {
		while (!R.empty() && Q.top() == R.top())
			Q.pop(), R.pop();
	}
	void push(const T& val) {
		Q.push(val);
	}
	void erase(const T& val) { //只能删除还在优先队列中的值，不能删除不存在的值。
		R.push(val);
		maintain();
	}
	void pop() {
		Q.pop();
		maintain();
	}
	T top() {
		return Q.top();
	}
	T top2() { //返回第二大的值，调用之前必须保证有size() >= 2。
		auto val = Q.top(); pop();
		auto sec = Q.top(); push(val);
		return sec;
	}
	auto size() const {
		return Q.size() - R.size();
	}
	bool empty() const {
		return size() == 0;
	}
	template<typename ...F> void emplace(F&&... args) {
		Q.emplace(std::forward<F>(args)...);
	}
};
int main() {
	
	return 0;
}