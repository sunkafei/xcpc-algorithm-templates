#include <iostream>
#include <chrono>
#include <set>
struct Timer {
	std::chrono::steady_clock::time_point start;
	Timer() : start(std::chrono::steady_clock::now()) {}
	~Timer() {
		auto finish = std::chrono::steady_clock::now();
		auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
		std::cerr << runtime / 1e6 << "s" << std::endl;
	}
};
int main() {
	Timer timer;
	std::set<int> S;
	for (int i = 0; i < 1e6; ++i) {
		S.insert(i);
	}
	return 0;
}