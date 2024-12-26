#include <vector>
#include <algorithm>
using namespace std;
struct state {
	bool inFinal();
	int score();
	void expand(int, vector<state>&);
};
int search(state& s, int player, int alpha, int beta) {
	if (s.inFinal()) {
		return s.score();
	}
	vector<state> children;
	s.expand(player, children);
	for (auto child : children) {
		int value = search(child, player ^ 1, alpha, beta);
		if (!player) {
			alpha = max(alpha, value);
		}
		else {
			beta = min(beta, value);
		}
		if (beta <= alpha) {
			break;
		}
	}
	return !player ? alpha : beta;
}