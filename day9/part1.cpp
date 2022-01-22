#include <charconv>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/nth_element.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/istream.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <vector>
auto get_neighbours(const std::vector<int> &values, std::size_t row_size, std::size_t ind)
{
    std::vector<int> neighbours;
    auto top_index = ind + row_size;
    if (top_index < std::size(values)) neighbours.push_back(values[top_index]);
    if (ind >= row_size) neighbours.push_back(values[ind - row_size]);
    // if (&&ind != (std::size(values) - 1)) {
    if ((ind % row_size) != 0 && ind != 0) neighbours.push_back(values[ind - 1]);
    if ((ind % row_size) != (row_size - 1) && ind != (std::size(values) - 1)) neighbours.push_back(values[ind + 1]);

    return neighbours;
}
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day9/day9.txt" };

    std::string line;
    std::vector<int> values;
    std::getline(input, line);
    ranges::for_each(line, [&values](auto c) { values.push_back(c - '0'); });
    auto line_size = std::size(line);
    while (std::getline(input, line)) {
        ranges::for_each(line, [&values](auto c) { values.push_back(c - '0'); });
    }

    auto lowest_points = ranges::views::enumerate(values) | ranges::views::filter([&values, line_size](const auto &iv) {
        auto ind = std::get<0>(iv);
        auto value = std::get<1>(iv);

        auto neighbours = get_neighbours(values, line_size, ind);
        // auto low_point_conditions = neighbours | ranges::views::transform([value](auto n) { return n > value; });
        return ranges::all_of(neighbours, [value](auto n) { return n > value; });
    })
                         | ranges::views::transform([](const auto &iv) { return std::get<1>(iv); });
    auto answer = ranges::accumulate(lowest_points, 0ull, {}, [](auto h) { return h + 1; });
    fmt::print("The answer is {}\n", answer);
}
