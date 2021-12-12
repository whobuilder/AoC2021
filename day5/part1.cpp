#include <charconv>
#include <fmt/format.h>
#include <fstream>
#include <map>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/minmax.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <utility>
#include <vector>

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day5/day5.txt" };
    auto is_view = ranges::istream_view<std::string>(input) | ranges::to<std::vector>;
    // https://stackoverflow.com/a/48403210/15697391
    auto lines = is_view
                 | ranges::views::filter([](const auto &s) { return s != "->"; })// get rid of "->"
                 | ranges::views::transform([](auto &&c) {// create x,y pairs
                       int x, y;
                       auto comma = ranges::find(c, ',');
                       std::from_chars(&*std::begin(c), &*comma, x);
                       std::from_chars(&*ranges::next(comma), &*std::end(c), y);
                       return std::pair<int, int>{ x, y };
                   })
                 | ranges::views::chunk(2);// create coordinate pairs

    auto horizontal_lines = lines | ranges::views::filter([](const auto &line) {
        const auto& p1 = *ranges::begin(line);
        const auto& p2 = *ranges::next(ranges::begin(line));
        return p1.second == p2.second; });
    auto vertical_lines = lines | ranges::views::filter([](const auto &line) {
        const auto& p1 = *ranges::begin(line);
        const auto& p2 = *ranges::next(ranges::begin(line));
        return p1.first == p2.first; });

    std::map<std::pair<int, int>, int> grid_points;
    ranges::for_each(horizontal_lines, [&grid_points](const auto &line) {
        const auto &p1 = *ranges::begin(line);
        const auto &p2 = *ranges::next(ranges::begin(line));
        auto minmax = ranges::minmax(p1.first, p2.first);
        auto x_points = ranges::views::iota(minmax.min, minmax.max + 1);
        ranges::for_each(x_points, [&grid_points, &p1](const auto &x) {
            std::pair<int, int> new_point = { x, p1.second };
            if (grid_points.count(new_point)) {
                grid_points[new_point] += 1;
            } else {
                grid_points[new_point] = 1;
            }
        });
    });

    ranges::for_each(vertical_lines, [&grid_points](const auto &line) {
        const auto &p1 = *ranges::begin(line);
        const auto &p2 = *ranges::next(ranges::begin(line));
        auto minmax = ranges::minmax(p1.second, p2.second);
        auto y_points = ranges::views::iota(minmax.min, minmax.max + 1);
        ranges::for_each(y_points, [&grid_points, &p1](const auto &y) {
            std::pair<int, int> new_point = { p1.first, y };
            if (grid_points.count(new_point)) {
                grid_points[new_point] += 1;
            } else {
                grid_points[new_point] = 1;
            }
        });
    });

    auto answer = ranges::count_if(grid_points | ranges::views::values, [](auto overlap_count) { return overlap_count > 1; });
    fmt::print("The answer is {}\n", answer);
}
