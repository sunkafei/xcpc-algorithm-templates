#include <type_traits>
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
#include <iterator>
#include <iomanip>
template<class> inline constexpr bool always_false_v = false;
struct placeholder {
    template<typename T> operator T() {
        return T{};
    }
};
template<typename T> consteval int struct_size(auto&&... params) {
    if constexpr (requires {T{ params... };}) {
        return struct_size<T>(params..., placeholder{});
    }
    else {
        return sizeof...(params) - 1;
    }
}
template<typename T> auto pack_struct(auto&& param) {
    constexpr int size = struct_size<T>();
    if constexpr (size == 0) {
        return std::make_tuple();
    }
    else if constexpr (size == 1) {
        auto&& [v1] = param;
        return std::make_tuple(v1);
    }
    else if constexpr (size == 2) {
        auto&& [v1, v2] = param;
        return std::make_tuple(v1, v2);
    }
    else if constexpr (size == 3) {
        auto&& [v1, v2, v3] = param;
        return std::make_tuple(v1, v2, v3);
    }
    else if constexpr (size == 4) {
        auto&& [v1, v2, v3, v4] = param;
        return std::make_tuple(v1, v2, v3, v4);
    }
    else if constexpr (size == 5) {
        auto&& [v1, v2, v3, v4, v5] = param;
        return std::make_tuple(v1, v2, v3, v4, v5);
    }
    else if constexpr (size == 6) {
        auto&& [v1, v2, v3, v4, v5, v6] = param;
        return std::make_tuple(v1, v2, v3, v4, v5, v6);
    }
    else if constexpr (size == 7) {
        auto&& [v1, v2, v3, v4, v5, v6, v7] = param;
        return std::make_tuple(v1, v2, v3, v4, v5, v6, v7);
    }
    else if constexpr (size == 8) {
        auto&& [v1, v2, v3, v4, v5, v6, v7, v8] = param;
        return std::make_tuple(v1, v2, v3, v4, v5, v6, v7, v8);
    }
    else {
        static_assert(always_false_v<decltype(param)>, "There are too many members in the structure.");
    }
}
template<bool repr=true> auto print_element(auto&& param) {
    using T = std::remove_cvref_t<decltype(param)>;
    if constexpr (requires {std::cout << param; }) { //normal types
        if constexpr (std::is_same_v<T, char>) {
            if constexpr (repr) {
                std::cout << '\'' << param << '\'';
            }
            else {
                std::cout << param;
            }
        }
        else if constexpr (requires{{param[0]} -> std::convertible_to<char>;}) {
            if constexpr (repr) {
                std::cout << std::quoted(param);
            }
            else {
                std::cout << param;
            }
        }
        else if constexpr (std::is_array_v<T>) {
            std::cout << '[';
            for (auto iter = std::begin(param); iter != std::end(param); ++iter) {
                if (iter != std::begin(param))
                    std::cout << ", ";
                print_element(*iter);
            }
            std::cout << ']';
        }
        else {  
            std::cout << param;
        }
    }
    else if constexpr (requires {param.first; param.second; }) { //std::pair
        std::cout << '(' << param.first << ", " << param.second << ')';
    }
    else if constexpr (requires {std::get<0>(param); }) {
        if constexpr (requires {param.index(); }) { //std::variant
            std::visit([](auto&& arg) {
                print_element(arg);
            }, param);
        }
        else { //std::tuple
            std::cout << '(';
            std::apply([](auto&&... args) {
                (..., (print_element(args), std::cout << ", "));
            }, param);
            std::cout << "\b\b)";
        }
    }
    else if constexpr (requires {param.push_back(param[0]); }) { //container
        std::cout << '[';
        for (auto iter = param.begin(); iter != param.end(); ++iter) {
            if (iter != param.begin())
                std::cout << ", ";
            print_element(*iter);
        }
        std::cout << ']';
    }
    else if constexpr (requires {typename T::key_type; }) { //associate container
        std::cout << '{';
        for (auto iter = param.begin(); iter != param.end(); ++iter) {
            if (iter != param.begin())
                std::cout << ", ";
            print_element(*iter);
        }
        std::cout << '}';
    }
    else {
        auto tuple = pack_struct<T>(param);
        print_element(tuple);
    }
}
auto print(auto&&... params) {
    (..., (print_element<false>(std::forward<decltype(params)>(params)), std::cout << ' '));
    std::cout << std::endl;
}
auto render(auto&&... params) {
    std::cout << "\033[1;32m";
    print(std::forward<decltype(params)>(params)...);
    std::cout << "\033[0m";
}
int main() {
    struct {
        int x;
        double y;
        std::string z;
    } user_defined{ 0, 1.2, "aa\\a" };
    const char* arr[3] = {"a\\a", "bb", "cc"};
    std::map<int, int> map = { {1, 2}, {3, 4}, {5, 6} };
    render('e', "z\\zz", arr, map, user_defined);
    
    auto T = std::make_tuple('w', "aa", std::vector<float>(3, 0.5));
    std::unordered_set<int> S{ 6, 7, 8 };
    std::vector<std::unordered_set<int>> V(2, S);
    print(T, V);
    return 0;
}