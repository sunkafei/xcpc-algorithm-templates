#include <iostream>
#include <cstdio>
#include <array>
class fixed { // 高精度定点数
private:
	static constexpr int num_blocks = 40;
	static constexpr int offset = (num_blocks / 2) * 64;
	std::array<uint64_t, num_blocks> data = {};
public:
	fixed(double value) {
		uint64_t bits = *(uint64_t*)&value;
		uint64_t fraction = bits & 0xfffffffffffff;
		uint64_t exponent = (bits & 0x7ff0000000000000) >> 52;
		uint64_t sign = bits >> 63;
		if (exponent != 0) {
			exponent -= 1023;
			fraction |= (1ull << 52);
		}
		else {
			exponent = -1022;
		}
		exponent += offset - 52;
		data[exponent / 64] |= (fraction << (exponent % 64));
		fraction <<= 11;
		data[(exponent + 52) / 64] |= (fraction >> (63 - (exponent + 52) % 64));
		if (sign) {
			for (int i = 0; i < num_blocks; ++i) {
				data[i] = ~data[i];
			}
			for (int i = 0; i < num_blocks; ++i) {
				data[i] += 1;
				if (data[i]) {
					break;
				}
			}
		}
	}
	explicit operator double() const {
		decltype(this->data) data = this->data;
		uint64_t sign = 0;
		if (data[num_blocks - 1] & (1ull << 63)) {
			sign = 1;
			for (int i = 0; i < num_blocks; ++i) {
				data[i] = ~data[i];
			}
			for (int i = 0; i < num_blocks; ++i) {
				data[i] += 1;
				if (data[i]) {
					break;
				}
			}
		}
		int64_t top = -1;
		for (int i = num_blocks - 1; i >= 0; --i) {
			if (data[i]) {
				for (int j = 63; j >= 0; --j) {
					if (data[i] & (1ull << j)) {
						top = i * 64 + j;
						goto finish;
					}
				}
			}
		}
		if (top == -1) {
			return 0;
		}
	finish:;
		int64_t exponent = top - offset + 1023;
		uint64_t fraction = 0;
		if (exponent >= 2047) {
			exponent = 2047;
		}
		else {
			for (int i = 1; i <= 52; ++i) {
				auto block = (top - i) / 64;
				auto num = (top - i) % 64;
				if (data[block] & (1ull << num)) {
					fraction |= (1ull << (52 - i));
				}
			}
			uint64_t carry = 0;
			if (data[(top - 53) / 64] & (1ull << ((top - 53) % 64))) {
				for (int i = 54; i <= 54 + 64; ++i) {
					auto block = (top - i) / 64;
					auto num = (top - i) % 64;
					if (data[block] & (1ull << num)) {
						carry = 1;
						break;
					}
				}
				if (!carry) {
					auto block = (top - 53) / 64;
					for (int i = 0; i < block; ++i) if (data[i]) {
						carry = 1;
						break;
					}
				}
				if (!carry) {
					if (fraction & 1) {
						carry = 1;
					}
				}
			}
			fraction += carry;
			if (fraction > 0xfffffffffffff) {
				fraction = 0;
				exponent += 1;
			}
			if (exponent <= 0) {
				fraction |= (1ull << 52);
				fraction >>= -exponent + 1;
				exponent = 0;
			}
		}
		fraction &= 0xfffffffffffff;
		uint64_t ret = (sign << 63) | (exponent << 52) | fraction;
		return *(double*)&ret;
	}
	void operator+= (const fixed& rhs) {
		uint64_t overflow = 0;
		for (int i = 0; i < num_blocks; ++i) {
			uint64_t value = rhs.data[i] + overflow;
			if (value < overflow) {
				overflow = 1;
			}
			else {
				overflow = 0;
			}
			data[i] += value;
			if (data[i] < value) {
				overflow = 1;
			}
		}
	}
	fixed operator+ (const fixed& rhs) const {
		fixed ret = *this;
		ret += rhs;
		return ret;
	}
	void debug() const {
		for (int i = 0; i < num_blocks; ++i) {
			for (int j = 0; j < 64; ++j) {
				if (data[i] & (1ull << j)) {
					printf("bit: %d\n", i * 64 + j - offset);
				}
			}
		}
	}
};
int main() {
	return 0;
}