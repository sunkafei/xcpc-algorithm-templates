#include <iostream>
#include <tuple>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <variant>
#include <string>
template<class> inline constexpr bool always_false_v = false;
auto output(auto&& param) {
    if constexpr (requires {std::cout << param; }) { //general
        std::cout << param;
    }
    else if constexpr (requires {param.first; param.second; }) { //std::pair
        std::cout << '(' << param.first << ' ' << param.second << ')';
    }
    else if constexpr (requires {std::get<0>(param); }) { //std::tuple and std::variant
        if constexpr (requires {param.index(); }) { //std::variant
            std::visit([](auto&& arg) {
                output(arg);
            }, param);
        }
        else { //std::tuple
            std::cout << '(';
            std::apply([](auto&&... args) {
                (..., (output(args), std::cout << ' '));
            }, param);
            std::cout << "\b)";
        }
    }
    else if constexpr (requires {param.push_back(param[0]); }) {
        std::cout << '[';
        for (auto iter = param.begin(); iter != param.end(); ++iter) {
            if (iter != param.begin())
                std::cout << ' ';
            output(*iter);
        }
        std::cout << ']';
    }
    else if constexpr (requires {param.count(*param.begin()); }) {
        std::cout << '{';
        for (auto iter = param.begin(); iter != param.end(); ++iter) {
            if (iter != param.begin())
                std::cout << ' ';
            output(*iter);
        }
        std::cout << '}';
    }
    else {
        static_assert(always_false_v<decltype(param)>, "无法输出的类型");
    }
}
auto print(auto&&... params) {
    (..., (output(std::forward<decltype(params)>(params)), std::cout << ' '));
    std::cout << std::endl;
}
auto render(auto&&... params) {
    std::cout << "\033[1;32m";
    print(std::forward<decltype(params)>(params)...);
    std::cout << "\033[0m";
}
int main() {
    std::variant<int, std::string> var = "zzzzz";
    std::vector vec{ 1, 2, 3, 4, 5 };
    render(123, std::make_tuple(123, "asd", "zxc"), 123, var, vec);
    auto PR = std::make_tuple(123, "aa", std::vector<float>(3, 0.5));
    std::unordered_set<int> S{ 6, 7, 8 };
    std::vector<std::unordered_set<int>> V(2, S);
    print(PR, V);
    return 0;
}