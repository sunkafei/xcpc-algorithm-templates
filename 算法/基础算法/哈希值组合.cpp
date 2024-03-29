#include <iostream>
#include <unordered_set>
#include <string>
template<typename F, typename ...T> inline size_t hash_combine(const F& first, const T&... params) {
    if constexpr (sizeof...(params) == 0) {
        return std::hash<F>()(first);
    }
    else {
        size_t seed = hash_combine(params...);
        return std::hash<F>()(first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}
template<typename A, typename B>
struct std::hash<std::pair<A, B>> {
    inline size_t operator() (const std::pair<A, B>& pr) const {
        return hash_combine(pr.first, pr.second);
    }
};
template<typename ...T>
struct std::hash<std::tuple<T...>> {
    inline size_t operator() (const std::tuple<T...>& tp) const {
        return apply(hash_combine<T...>, tp); // 需要C++17
    }
};
int main() {
    using namespace std::literals;
    std::unordered_set<std::tuple<int, int, std::string>> S;
    S.insert({ 1, 2, "asd"s });
    S.insert({ 4, 5, "zxc"s });
    for (auto [a, b, c] : S) {
        std::cout << a << " " << b << " " << c << std::endl;
    }
    // 1 2 asd
    // 4 5 zxc
    return 0;
}