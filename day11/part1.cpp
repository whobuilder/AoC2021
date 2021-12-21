#include <charconv>
#include <deque>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/iterator/insert_iterators.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>
#include <unordered_map>
#include <vector>

template<std::size_t RowSize = 10>
auto is_neighbour_of(std::size_t origin_ind, std::size_t current_ind)
{

    auto bottom_index = origin_ind + RowSize;
    auto top_index = origin_ind - RowSize;
    if (current_ind == bottom_index) return true;//bottom neighbour
    if ((origin_ind % RowSize) != 0 && bottom_index - 1 == current_ind) return true;// bottom left neighbour
    if ((origin_ind % RowSize) != (RowSize - 1) && bottom_index + 1 == current_ind) return true;// bottom right neighbour
    if (top_index == current_ind) return true;// top neighbour
    if ((origin_ind % RowSize) != 0 && top_index - 1 == current_ind) return true;// top left neighbour
    if ((origin_ind % RowSize) != (RowSize - 1) && top_index + 1 == current_ind) return true;// top right neighbour
    if ((origin_ind % RowSize) != 0 && origin_ind - 1 == current_ind) return true;//left neighbour
    if ((origin_ind % RowSize) != (RowSize - 1) && origin_ind + 1 == current_ind) return true;//right neighbour

    return false;
}
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day11/day11.txt" };
    auto energy_levels = ranges::istream_view<char>(input)
                         | ranges::views::transform([](char c) {
                               return c - '0';
                           })
                         | ranges::views::enumerate
                         | ranges::to<std::vector>;
    std::size_t number_of_flashes = 0;
    for (std::size_t step = 0; step < 100; ++step) {
        //First, the energy level of each octopus increases by 1.
        ranges::for_each(energy_levels, [](auto &v) { ++v.second; });
        // fmt::print("{}\n", energy_levels);
        std::unordered_map<std::size_t, int> flashed_octopuses;
        ranges::for_each(energy_levels | ranges::views::filter([](auto v) { return v.second == 10; }), [&flashed_octopuses](auto octopus) { flashed_octopuses[octopus.first] = octopus.second; });
        auto flashed_indices = ranges::views::keys(flashed_octopuses) | ranges::to<std::deque>;
        //Second, do the flashing an increase neighbouring energy levels
        while (std::size(flashed_indices)) {
            auto flashed_index = flashed_indices.front();
            auto neighbours = energy_levels | ranges::views::filter([flashed_index](auto octopus) { return is_neighbour_of<10>(flashed_index, octopus.first); });
            ranges::for_each(neighbours, [&energy_levels](auto &v) {
                ++(energy_levels[v.first].second);
            });
            ranges::for_each(energy_levels | ranges::views::filter([](auto v) { return v.second == 10; }), [&flashed_octopuses, &flashed_indices](auto octopus) {
                if (!flashed_octopuses.count(octopus.first)) {
                    flashed_octopuses[octopus.first] = octopus.second;
                    flashed_indices.push_back(octopus.first);
                }
            });
            flashed_indices.pop_front();
        }
        // reset energy level
        ranges::for_each(ranges::views::keys(flashed_octopuses), [&energy_levels](auto ind) { energy_levels[ind].second = 0; });
        number_of_flashes += std::size(flashed_octopuses);
    }


    fmt::print("The answer is {}\n", number_of_flashes);
}
