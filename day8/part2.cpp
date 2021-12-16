#include <bitset>
#include <charconv>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <unordered_map>
#include <vector>
struct Display
{
    std::vector<std::bitset<7>> hints;
    std::vector<std::bitset<7>> outputs;
    std::unordered_map<int, std::bitset<7>> mapping;
};

std::bitset<7> to_bitset(std::string_view input)
{
    std::bitset<7> output;
    for (auto c : input) {
        output[static_cast<std::size_t>(c - 'a')] = true;
    }
    return output;
}

auto covers(std::bitset<7> a, std::bitset<7> b)
{
    return (a & b) == b;
}

auto do_mapping(const std::vector<std::bitset<7>> &hints)
{
    std::unordered_map<int, std::bitset<7>> mapping;
    mapping[1] = *ranges::find_if(hints, [](const auto &h) { return h.count() == 2; });
    mapping[4] = *ranges::find_if(hints, [](const auto &h) { return h.count() == 4; });
    mapping[7] = *ranges::find_if(hints, [](const auto &h) { return h.count() == 3; });
    mapping[8] = *ranges::find_if(hints, [](const auto &h) { return h.count() == 7; });
    mapping[9] = *ranges::find_if(hints, [&mapping](const auto &h) { return h.count() == 6 && covers(h, mapping[4]); });
    mapping[0] = *ranges::find_if(hints, [&mapping](const auto &h) { return h.count() == 6 && covers(h, mapping[7]) && !covers(h, mapping[4]); });
    mapping[6] = *ranges::find_if(hints, [&mapping](const auto &h) { return h.count() == 6 && !covers(h, mapping[7]) && !covers(h, mapping[4]); });
    mapping[3] = *ranges::find_if(hints, [&mapping](const auto &h) { return h.count() == 5 && covers(h, mapping[7]); });
    mapping[5] = *ranges::find_if(hints, [&mapping](const auto &h) { return h.count() == 5 && covers(mapping[9], h) && !covers(h, mapping[7]); });
    mapping[2] = *ranges::find_if(hints, [&mapping](const auto &h) { return h.count() == 5 && !covers(mapping[9], h); });


    return mapping;
}

std::vector<Display> parse_input(std::ifstream &input)
{
    std::string line{};
    std::vector<Display> displays;
    while (std::getline(input, line)) {

        auto splitted = line | ranges::views::split(' ') | ranges::view::transform([](auto &&rng) {
            return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
        });
        auto hints = splitted | ranges::views::take(10) | ranges::views::transform([](auto &&d) { return to_bitset(d); }) | ranges::to<std::vector>;
        auto outputs = splitted | ranges::views::drop(11) | ranges::views::transform([](auto &&d) { return to_bitset(d); }) | ranges::to<std::vector>;
        // fmt::print("{}\n", hints);
        displays.push_back(Display{ std::move(hints), std::move(outputs), {} });
    }
    return displays;
}
int to_digit(const std::unordered_map<int, std::bitset<7>> &mapping, const std::bitset<7> &bits)
{
    return (*ranges::find_if(mapping, [&bits](const auto &m) { return m.second == bits; })).first;
}
int to_number(const Display &d)
{
    return ranges::accumulate(ranges::view::enumerate(d.outputs), 0, std::plus{}, [&d](const auto output) { return std::pow(10, (3 - std::get<0>(output))) * to_digit(d.mapping, std::get<1>(output)); });
}
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day8/day8.txt" };
    auto displays = parse_input(input);
    ranges::for_each(displays, [](auto &d) { d.mapping = do_mapping(d.hints); });
    auto answer = ranges::accumulate(displays, 0, std::plus{}, [](const auto &d) { return to_number(d); });
    fmt::print("The answer is {}\n", answer);
}
