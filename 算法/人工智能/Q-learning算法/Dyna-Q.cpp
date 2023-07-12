#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <random>
#include <tuple>
#include <vector>
#include <map>
using namespace std;
const int nrow = 10, ncol = 10, max_state = nrow * ncol, max_action = 4;
const int dr[4] = { -1, 1, 0, 0 };
const int dc[4] = { 0, 0, -1, 1 };
struct CliffWalkingEnv {
	char s[11][11];
	int x, y;
	vector<pair<int, int>> path;
	CliffWalkingEnv() {
		reset();
	}
	int reset() {
		x = 0, y = 0;
		path.clear();
		path.emplace_back(x, y);
		memset(s, 0, sizeof(s));
		for (int i = 0; i < nrow; ++i) {
			for (int j = 0; j < ncol; ++j) {
				s[i][j] = '.';
			}
		}
		for (int i = 0; i < 5; ++i) {
			for (int j = 1; j < ncol - 1; ++j) {
				s[i][j] = '#';
			}
		}
		return x * ncol + y;
	}
	void print() {
		char t[11][11];
		memcpy(t, s, sizeof(s));
		for (auto [x, y] : path) {
			if (t[x][y] >= '1' && t[x][y] < '9')
				t[x][y] += 1;
			else
				t[x][y] = '1';
		}
		for (int i = 0; i < nrow; ++i)
			printf("%s\n", t[i]);
		printf("\n");
	}
	tuple<int, int, int> step(int number) {
		int nx = x + dr[number], ny = y + dc[number];
		if (nx >= 0 && nx < nrow && ny >= 0 && ny < ncol)
			x = nx, y = ny;
		int reward = -1, done = false;
		if (s[x][y] == '#') {
			reward = -100;
			done = true;
		}
		if (x == 0 && y == ncol - 1)
			done = true;
		path.emplace_back(x, y);
		return make_tuple(x * ncol + y, reward, done);
	}
} env;

const double alpha = 1e-1; /*学习率*/
const double gamma = 0.9; /*折扣因子*/
const double epsilon = 1e-2; /*epsilon-greedy*/
const int N = 10; //Q-Planning的次数
const int max_episode = 200; //训练多少轮
uniform_real_distribution<double> p;
uniform_int_distribution<int> d;
default_random_engine e;
double Q[max_state][max_action];
map<pair<int, int>, int> id;
vector<tuple<int, int, int, int>> model;
int epsilon_greedy(int state) { /*基于epsilon-greedy选择动作*/
	if (p(e) < epsilon)
		return d(e) % max_action;
	return max_element(Q[state], Q[state] + max_action) - Q[state];
}
void learn(int s0, int a0, int r, int s1) {
	auto td_error = r + gamma * *max_element(Q[s1], Q[s1] + max_action) - Q[s0][a0];
	Q[s0][a0] += alpha * td_error;
}
void update(int s0, int a0, int r, int s1) { 
	learn(s0, a0, r, s1);
	pair<int, int> pr(s0, a0);
	if (!id.count(pr)) {
		id[pr] = model.size();
		model.emplace_back(s0, a0, r, s1);
	}
	else {
		model[id[pr]] = make_tuple(s0, a0, r, s1);
	}
	for (int i = 0; i < N; ++i) {
		int idx = d(e) % model.size();
		auto [s0, a0, r, s1] = model[idx];
		learn(s0, a0, r, s1);
	}
}
void DynaQ() {
	for (int i = 0; i < max_episode; ++i) {
		int state = env.reset();
		for (;;) {
			int action = epsilon_greedy(state);
			auto [next_state, reward, done] = env.step(action);
			update(state, action, reward, next_state);
			state = next_state;
			if (done)
				break;
		}
		env.print();
	}
}
int main() {
	DynaQ();
	return 0;
}