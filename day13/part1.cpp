#include <charconv>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/for_each.hpp>
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

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day13/day13.txt" };
    std::set<std::pair<int, int>> points;
    std::string line;
    int max_x = 0;
    int max_y = 0;
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
        int x = *ranges::begin(values);
        int y = *ranges::next(ranges::begin(values));
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;
        points.insert({ *ranges::begin(values), *ranges::next(ranges::begin(values)) });
    }

    // fold information
    std::getline(input, line);
    auto fold_info = ranges::views::split(line, '=');
    auto fold_no_range = fold_info | ranges::views::drop(1) | ranges::views::transform([](auto &&rng) {
        return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
    }) | ranges::views::transform([](auto &&sv) {
                                int value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; });
    auto fold_along = *ranges::begin(fold_info) | ranges::to<std::string>;
    auto fold_no = *ranges::begin(fold_no_range);

    std::set<std::pair<int, int>> points_copy = points;
    for (const auto &p : points) {
        if (fold_along == "fold along y") {
            if (p.second > fold_no) {
                auto y_new = fold_no - (p.second - fold_no);
                points_copy.insert({ p.first, y_new });
                points_copy.erase(p);
            }
        } else {// fold along x
            if (p.first > fold_no) {
                auto x_new = fold_no - (p.first - fold_no);
                points_copy.insert({ x_new, p.second });
                points_copy.erase(p);
            }
        }
    }
    fmt::print("The answer is {}\n", std::size(points_copy));
}
