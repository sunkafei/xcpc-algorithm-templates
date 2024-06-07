#include <ranges>
#include <cassert>
#include <cstdio>
template<typename T, int maxsize>
class darray : public std::ranges::view_interface<darray<T, maxsize>> {
private:
    T data[maxsize];
    int sz;
public:
    darray() : sz(0) {}
    darray(const darray& rhs) noexcept : sz(rhs.sz) {
        copy(rhs.begin(), rhs.begin() + sz, data);
    }
    darray& operator= (const darray& rhs) noexcept {
        sz = rhs.sz;
        copy(rhs.begin(), rhs.begin() + sz, data);
        return *this;
    }
    void push_back(T val) noexcept {
        assert(sz < maxsize);
        data[sz++] = std::move(val);
    }
    void expand() noexcept {
        assert(sz < maxsize);
        sz += 1;
    }
    void erase(T val) noexcept {
        for (int i = 0; i < sz; ++i) {
            if (data[i] == val) {
                data[i] = std::move(data[sz - 1]);
                sz -= 1;
                return;
            }
        }
    }
    void replace(T old_val, T new_val) noexcept {
        for (int i = 0; i < sz; ++i) {
            if (data[i] == old_val) {
                data[i] = new_val;
                return;
            }
        }
    }
    void set(T val) noexcept {
        data[0] = val;
        sz = 1;
    }
    bool contains(T val) const noexcept {
        for (int i = 0; i < sz; ++i) {
            if (data[i] == val) {
                return true;
            }
        }
        return false;
    }
    void pop_back() {
        assert(sz > 0);
        sz -= 1;
    }
    void clear() noexcept {
        sz = 0;
    }
    auto* begin() noexcept {
        return data;
    }
    auto* end() noexcept {
        return data + sz;
    }
    const auto* begin() const noexcept {
        return data;
    }
    const auto* end() const noexcept {
        return data + sz;
    }
};
int main() {
    darray<int, 100> arr;
    for (int i = 0; i < 50; ++i) {
        arr.push_back(i);
    }
    for (int i = 1; i < 50; ++i) {
        arr[i] += arr[i - 1];
    }
    std::printf("%d %d %d\n", (int)arr.size(), arr.front(), arr[30]);
	return 0;
}