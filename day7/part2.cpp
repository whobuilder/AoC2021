#include <charconv>
#include <cmath>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/algorithm/minmax.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <vector>

int fuel_cost(int current, int dest)
{
    auto dist = std::abs(dest - current);
    return dist * (dist + 1) / 2;
}

template<class RNG>
int total_cost(RNG &&rng, int dest)
{

    return ranges::accumulate(rng | ranges::views::transform([&](auto v) { return fuel_cost(v, dest); }), 0);
}
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day7/day7.txt" };

    std::string line;
    std::getline(input, line);
    // https://stackoverflow.com/a/48403210/15697391
    auto values = ranges::split_view(line, ',')
                  | ranges::view::transform([](auto &&rng) {
                        return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
                    })
                  | ranges::view::transform([](auto &&sv) {
                                int value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; })
                  | ranges::to<std::vector>;
    auto minmax = ranges::minmax(values);
    auto destinations = ranges::views::iota(minmax.min, minmax.max + 1);
    auto all_costs = destinations | ranges::views::transform([&](auto dest) { return total_cost(values, dest); });
    fmt::print("The answer is {}\n", *ranges::min_element(all_costs));
}
