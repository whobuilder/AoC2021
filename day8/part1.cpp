#include <bitset>
#include <charconv>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/nth_element.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/istream.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <vector>
struct Display
{
    std::vector<std::bitset<7>> hints;
    std::vector<std::bitset<7>> outputs;
};
std::bitset<7> to_bitset(std::string_view input)
{
    std::bitset<7> output;
    for (auto c : input) {
        output[static_cast<std::size_t>(c - 'a')] = true;
    }
    return output;
}

std::vector<Display> parse_input(std::ifstream &input)
{
    std::string line{};
    std::vector<Display> displays;
    while (std::getline(input, line)) {

        auto splitted = line | ranges::views::split(' ') | ranges::views::transform([](auto &&rng) {
            return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
        });
        auto hints = splitted | ranges::views::take(10) | ranges::views::transform([](auto &&d) { return to_bitset(d); }) | ranges::to<std::vector>;
        auto outputs = splitted | ranges::views::drop(11) | ranges::views::transform([](auto &&d) { return to_bitset(d); }) | ranges::to<std::vector>;
        // fmt::print("{}\n", hints);
        displays.push_back(Display{ std::move(hints), std::move(outputs) });
    }
    return displays;
}

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day8/day8.txt" };
    auto displays = parse_input(input);
    auto answer = ranges::accumulate(displays, 0, std::plus{}, [](const auto &d) { return ranges::count_if(d.outputs, [](const auto &o) { return o.count() == 2 || o.count() == 4 || o.count() == 3 || o.count() == 7; }); });
    fmt::print("The answer is {}\n", answer);
}
