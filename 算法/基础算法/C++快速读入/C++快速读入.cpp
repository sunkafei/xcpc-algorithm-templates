#include <cstdio>
using namespace std;
inline int read_positive() { //读取一个正整数
	int x = 0;
	char c = getchar();
	while (c < '0' || c > '9') 
		c = getchar();
	while (c >= '0' && c <= '9') {
		x = x * 10 + c - '0';
		c = getchar();
	}
	return x;
}
inline int read_negative() { //可以读取负数
	int x = 0, y = 1;
	char c = getchar();
	while (c < '0' || c > '9') {
		if (c == '-') 
			y = -1;
		c = getchar(); 
	}
	while (c >= '0' && c <= '9') {
		x = x * 10 + c - '0';
		c = getchar();
	}
	return x * y;
}
int main() {
    
}