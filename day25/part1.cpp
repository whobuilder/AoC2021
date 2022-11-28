#include <boost/functional/hash.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/view/enumerate.hpp>
#include <string>
#include <string_view>
#include <utility>

#include <unordered_set>

using CoordinateValueType = long long;
using Coordinate = std::pair<CoordinateValueType, CoordinateValueType>;

class CoordinateHash
{
public:
    auto operator()(const Coordinate &pair) const -> std::size_t
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, pair.first);
        boost::hash_combine(seed, pair.second);


        return seed;
    }
};

using CoordinateContainer = std::unordered_set<Coordinate, CoordinateHash>;

struct ocean_map
{
    CoordinateContainer right_arrows;
    CoordinateContainer down_arrows;
    CoordinateContainer dots;
    long long max_row_index = 0;
    long long max_column_index = 0;
};

auto parse(const std::string &input_path) -> ocean_map
{
    std::ifstream input_stream{ input_path };
    std::string line;
    ocean_map map{};
    long long row_no = 0;
    while (std::getline(input_stream, line)) {
        for (const auto &[column_no, value] : ranges::views::enumerate(line)) {
            switch (value) {
            case ('>'): {
                map.right_arrows.emplace(row_no, column_no);
                break;
            }
            case ('v'): {
                map.down_arrows.emplace(row_no, column_no);
                break;
            }
            case ('.'): {
                map.dots.emplace(row_no, column_no);
                break;
            }
            default:
                throw std::runtime_error("unexpected character");
            }
        }
        ++row_no;
    }
    map.max_row_index = row_no - 1;
    map.max_column_index = std::size(line) - 1;
    return map;
}

auto right_next(const Coordinate &coord, CoordinateValueType column_max) -> Coordinate
{
    if (coord.second == column_max) return { coord.first, 0 };
    return { coord.first, coord.second + 1 };
}

auto below_next(const Coordinate &coord, CoordinateValueType row_max) -> Coordinate
{
    if (coord.first == row_max) return { 0, coord.second };
    return { coord.first + 1, coord.second };
}

template<typename GoFunc>
auto next_move(const CoordinateContainer &right_arrows,
  const CoordinateContainer &dots,
  CoordinateValueType max_index,
  GoFunc get_next_coordinate) -> std::tuple<CoordinateContainer, CoordinateContainer, bool>
{
    auto right_arrows_modified = right_arrows;
    auto dots_modified = dots;
    bool moved = false;
    for (const auto &coord : right_arrows) {
        auto next_pos = get_next_coordinate(coord, max_index);
        // important you should check the original dots, because they all move at the same time
        if (dots.count(next_pos) == 0) continue;// it cannot move to right
        moved = true;
        auto dot_coord_handle = dots_modified.extract(next_pos);
        auto right_arrow_handle = right_arrows_modified.extract(coord);
        dot_coord_handle.value() = coord;
        right_arrow_handle.value() = next_pos;
        dots_modified.insert(std::move(dot_coord_handle));
        right_arrows_modified.insert(std::move(right_arrow_handle));
    }
    return { std::move(right_arrows_modified), std::move(dots_modified), moved };
}


int main()
{
    auto [right_arrows, down_arrows, dots, row_max, column_max] = parse(R"(D:\cpp_projects\cpp_experiments\AoC2021\day25\day25.txt)");
    bool moved = true;
    std::size_t step_no = 0;
    while (moved) {
        auto [right_arrows_new, dots_part1, moved_part1] = next_move(right_arrows, dots, column_max, right_next);
        auto [down_arrows_new, dots_new, moved_part2] = next_move(down_arrows, dots_part1, row_max, below_next);
        moved = moved_part1 || moved_part2;
        right_arrows = std::move(right_arrows_new);
        down_arrows = std::move(down_arrows_new);
        dots = std::move(dots_new);
        ++step_no;
    }
    fmt::print("The answer is {}\n", step_no);
}
