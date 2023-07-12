#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <numeric>
using namespace std;
using integer = long long;
struct frac {
	integer num, den;
	frac() : num(0), den(1){}
	frac(integer val) {
		num = val;
		den = 1;
	}
	frac(integer a, integer b) {
		integer g = gcd(a, b);
		num = a / g;
		den = b / g;
		if (den < 0) {
			num = -num;
			den = -den;
		}
	}
};
frac operator+ (frac x, frac y) {
	return frac(x.num * y.den + y.num * x.den, x.den * y.den);
}
frac operator- (frac x, frac y) {
	return frac(x.num * y.den - y.num * x.den, x.den * y.den);
}
frac operator* (frac x, frac y) {
	return frac(x.num * y.num, x.den * y.den);
}
frac operator/ (frac x, frac y) {
	return frac(x.num * y.den, x.den * y.num);
}
frac operator+ (frac x, integer val) {
	return frac(x.num + x.den * val, x.den);
}
frac operator* (frac x, integer val) {
	return frac(x.num * val, x.den);
}
frac operator/ (frac x, integer val) {
	return frac(x.num, x.den * val);
}
bool operator== (frac x, frac y) {
	return x.num == y.num && x.den == y.den;
}
bool operator!= (frac x, frac y) {
	return x.num != y.num || x.den != y.den;
}
bool operator< (frac x, frac y) {
	return x.num * y.den < y.num * x.den;
}
frac abs(frac x) {
	if (x.num < 0)
		return frac(-x.num, x.den);
	return x;
}
int main() {
	printf("%d\n", gcd(0, 6));
	return 0;
}