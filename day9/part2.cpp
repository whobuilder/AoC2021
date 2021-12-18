#include <charconv>
#include <deque>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/nth_element.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/common.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <vector>
auto get_neighbours(const std::vector<int> &values, std::size_t row_size, std::size_t ind)
{
    std::vector<std::pair<std::size_t, int>> neighbours;
    auto top_index = ind + row_size;
    if (top_index < std::size(values)) neighbours.emplace_back(top_index, values[top_index]);
    if (ind >= row_size) neighbours.emplace_back(ind - row_size, values[ind - row_size]);
    // if (&&ind != (std::size(values) - 1)) {
    if ((ind % row_size) != 0 && ind != 0) neighbours.emplace_back(ind - 1, values[ind - 1]);
    if ((ind % row_size) != (row_size - 1) && ind != (std::size(values) - 1)) neighbours.emplace_back(ind + 1, values[ind + 1]);

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
        return ranges::all_of(neighbours, [value](auto n) { return n.second > value; });
    });
    std::vector<std::size_t> basin_sizes;
    ranges::for_each(lowest_points, [&](const auto &p) {
        std::vector<std::pair<std::size_t, int>> basin;
        std::deque<std::pair<std::size_t, int>> basin_copy;
        basin.emplace_back(p);
        basin_copy.emplace_back(p);
        while (std::size(basin_copy)) {
            auto point = basin_copy[0];
            basin_copy.pop_front();
            auto neighbours = get_neighbours(values, line_size, point.first);
            auto filtered_neighbours = neighbours | ranges::views::filter([&point](auto n) { return n.second > point.second && n.second != 9; }) | ranges::views::common;
            basin_copy.insert(basin_copy.end(), ranges::begin(filtered_neighbours), ranges::end(filtered_neighbours));
            basin.insert(basin.end(), ranges::begin(filtered_neighbours), ranges::end(filtered_neighbours));
        }
        ranges::sort(basin, {}, [](auto b) { return b.first; });
        auto first = ranges::unique(basin, {}, [](auto b) { return b.first; });
        basin.erase(first, std::end(basin));
        if (std::size(basin_sizes) < 3) {
            basin_sizes.push_back(ranges::size(basin));
        } else if (ranges::size(basin) > *ranges::min_element(basin_sizes)) {
            *ranges::min_element(basin_sizes) = ranges::size(basin);
        }
    });
    auto answer = ranges::accumulate(basin_sizes, 1, std::multiplies{});
    fmt::print("The answer is {}\n", answer);
}
