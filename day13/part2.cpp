#include <charconv>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/istream.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>


auto get_points(std::ifstream &input)
{
    std::set<std::pair<int, int>> points;
    std::string line;
    while (std::getline(input, line)) {
        if (line == "") {
            break;
        }
        auto values = ranges::split_view(line, ',')
                      | ranges::views::transform([](auto &&rng) {
                            return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
                        })
                      | ranges::views::transform([](auto &&sv) {
                                int value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; });


        points.insert({ *ranges::begin(values), *ranges::next(ranges::begin(values)) });
    }
    return points;
}

auto fold_along_x(const std::set<std::pair<int, int>> &points, int fold_axis_no)
{
    std::set<std::pair<int, int>> points_copy = points;
    for (const auto &p : points) {
        if (p.first > fold_axis_no) {
            auto x_new = fold_axis_no - (p.first - fold_axis_no);
            points_copy.insert({ x_new, p.second });
            points_copy.erase(p);
        }
    }
    return points_copy;
}

auto fold_along_y(const std::set<std::pair<int, int>> &points, int fold_axis_no)
{
    std::set<std::pair<int, int>> points_copy = points;
    for (const auto &p : points) {
        if (p.second > fold_axis_no) {
            auto y_new = fold_axis_no - (p.second - fold_axis_no);
            points_copy.insert({ p.first, y_new });
            points_copy.erase(p);
        }
    }
    return points_copy;
}

void print_points(const std::set<std::pair<int, int>> &points)
{
    auto x_max = (*ranges::max_element(points, std::less<int>{}, [](const auto &p) { return p.first; })).first;
    auto y_max = (*ranges::max_element(points, std::less<int>{}, [](const auto &p) { return p.second; })).second;
    fmt::print("The answer is:\n");
    for (int i = 0; i <= y_max; ++i) {
        for (int j = 0; j <= x_max; ++j) {
            if (points.count({ j, i })) {
                fmt::print("#");
            } else {
                fmt::print(" ");
            }
        }
        fmt::print("\n");
    }
}

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day13/day13.txt" };

    auto points = get_points(input);
    std::string line;
    while (std::getline(input, line)) {


        auto fold_info = ranges::views::split(line, '=');
        auto fold_no_range = fold_info | ranges::views::drop(1) | ranges::views::transform([](auto &&rng) {
            return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
        }) | ranges::views::transform([](auto &&sv) {
                                int value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; });
        auto fold_along = *ranges::begin(fold_info) | ranges::to<std::string>;
        auto fold_no = *ranges::begin(fold_no_range);

        if (fold_along == "fold along y") {
            points = fold_along_y(points, fold_no);
        } else {// fold along x
            points = fold_along_x(points, fold_no);
        }
    }
    print_points(points);
}
