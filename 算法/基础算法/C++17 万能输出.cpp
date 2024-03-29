#include <iostream>
#include <tuple>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
template<typename T> void print_element(const T& param);
template<typename A, typename B> void print_element(const std::pair<A, B>& param);
template<typename ...T> void print_element(const std::tuple<T...>& param);
template<typename T, int SIZE> void print_element(const T(&param)[SIZE]);
template<typename T> void print_element(const std::vector<T>& param);
template<typename T> void print_element(const std::set<T>& param);
template<typename K, typename V> void print_element(const std::map<K, V>& param);
template<typename T> void print_element(const std::unordered_set<T>& param);
template<typename K, typename V> void print_element(const std::unordered_map<K, V>& param);
template<typename T> void print_element(const T& param) {
    std::cout << param;
}
template<typename A, typename B> void print_element(const std::pair<A, B>& param) {
    std::cout << '(';
    print_element(param.first);
    print_element(param.second);
    std::cout << ')';
}
template<typename ...T> void print_element(const std::tuple<T...>& param) {
    std::cout << '(';
    std::apply([](auto&&... args) {
        (..., (print_element(args), std::cout << ' '));
    }, param);
    std::cout << "\b)";
}
template<typename T, int SIZE> void print_element(const T(&param)[SIZE]) {
    std::cout << '[';
    for (int i = 0; i < SIZE; ++i) {
        if (i) {
            std::cout << ' ';
        }
        print_element(param[i]);
    }
    std::cout << ']';
}
template<typename T> void print_element(const std::vector<T>& param) {
    std::cout << '[';
    for (int i = 0; i < param.size(); ++i) {
        if (i) {
            std::cout << ' ';
        }
        print_element(param[i]);
    }
    std::cout << ']';
}
template<typename T> void print_element(const std::set<T>& param) {
    std::cout << '{';
    for (auto iter = param.begin(); iter != param.end(); ++iter) {
        if (iter != param.begin()) {
            std::cout << ' ';
        }
        print_element(*iter);
    }
    std::cout << '}';
}
template<typename K, typename V> void print_element(const std::map<K, V>& param) {
    std::cout << '{';
    for (auto iter = param.begin(); iter != param.end(); ++iter) {
        if (iter != param.begin()) {
            std::cout << ' ';
        }
        print_element(*iter);
    }
    std::cout << '}';
}
template<typename T> void print_element(const std::unordered_set<T>& param) {
    std::cout << '{';
    for (auto iter = param.begin(); iter != param.end(); ++iter) {
        if (iter != param.begin()) {
            std::cout << ' ';
        }
        print_element(*iter);
    }
    std::cout << '}';
}
template<typename K, typename V> void print_element(const std::unordered_map<K, V>& param) {
    std::cout << '{';
    for (auto iter = param.begin(); iter != param.end(); ++iter) {
        if (iter != param.begin()) {
            std::cout << ' ';
        }
        print_element(*iter);
    }
    std::cout << '}';
}
template<typename ...T> void print(const T&... params) {
    (..., (print_element(params), std::cout << " "));
    std::cout << std::endl;
}
int main() {
    auto PR = std::make_tuple(123, "aa", std::vector<float>(3, 0.5));
    std::unordered_set<int> S{ 6, 7, 8 };
    std::vector<std::unordered_set<int>> V(2, S);
    print(PR, V);
    return 0;
}