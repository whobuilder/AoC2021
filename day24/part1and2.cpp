#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/split.hpp>
#include <string>
#include <tuple>
#include <unordered_map>

long long get_z(long long previous_z, int digit, int line5, int line6, int line16)
{
    if (previous_z % 26 + line6 == digit)
        return previous_z / line5;
    else
        return (previous_z / line5) * 26 + digit + line16;
}
using ParamType = std::array<int, 14>;


long long search(const ParamType &line5, const ParamType &line6, const ParamType &line16, std::function<long long(long long, long long)> func)
{
    std::unordered_map<long long, long long> previous_zs;
    std::unordered_map<long long, long long> zs;
    previous_zs.insert(std::make_pair(0, 0));

    for (std::size_t index = 0; index < 14; index++) {
        for (auto [previous_z, previous_value] : previous_zs) {
            for (int digit = 1; digit < 10; digit++) {
                long long value = previous_value * 10 + digit;
                long long z = get_z(previous_z, digit, line5[index], line6[index], line16[index]);

                auto [it, flag] = zs.emplace(z, value);
                if (!flag)
                    it->second = func(it->second, value);
            }
        }
        previous_zs = std::move(zs);
        zs.clear();
    }
    return previous_zs[0];
}

int main()
{
    std::ifstream input_stream{ R"(D:\cpp_projects\cpp_experiments\AoC2021\day24\day24.txt)" };
    std::string line;
    int ind = 1;
    std::size_t input_no = 0;
    ParamType line5{};
    ParamType line6{};
    ParamType line16{};
    while (std::getline(input_stream, line)) {

        if (ind == 5) {
            auto v = ranges::views::split(line, ' ') | ranges::to<std::vector<std::string>>;
            line5[input_no] = std::stoi(v[2]);
        } else if (ind == 6) {
            auto v = ranges::views::split(line, ' ') | ranges::to<std::vector<std::string>>;
            line6[input_no] = std::stoi(v[2]);
        } else if (ind == 16) {
            auto v = ranges::views::split(line, ' ') | ranges::to<std::vector<std::string>>;
            line16[input_no] = std::stoi(v[2]);
        } else if (line[0] == 'i') {
            ind = 1;
            if (input_no != 0) ++input_no;
        }
        ++ind;
    }
    fmt::print("The part 1 answer is {}\n", search(line5, line6, line16, [](auto a, auto b) { return std::max(a, b); }));
    fmt::print("The part 2 answer is {}\n", search(line5, line6, line16, [](auto a, auto b) { return std::min(a, b); }));
}
