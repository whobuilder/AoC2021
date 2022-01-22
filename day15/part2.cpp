#include <array>
#include <cstdlib>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <limits>
#include <map>
#include <queue>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/stride.hpp>
#include <range/v3/view/transform.hpp>
#include <set>
#include <string>
#include <utility>
using Vertice = std::pair<int, int>;
std::array<Vertice, 4> get_neigbours(const Vertice &vertice)
{
    auto row = vertice.first;
    auto col = vertice.second;
    return { Vertice{ row - 1, col }, Vertice{ row + 1, col }, Vertice{ row, col - 1 }, Vertice{ row, col + 1 } };
}

struct greater_distance
{
    template<typename T, typename U>
    constexpr bool operator()(const std::pair<T, U> &lhs, const std::pair<T, U> &rhs) const
    {
        return lhs.second > rhs.second;
    }
};
int main()
{
    constexpr int map_scale_factor = 5;
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day15/day15.txt" };
    std::string line;
    std::map<Vertice, std::size_t> weight_table;
    std::map<Vertice, std::size_t> distances;
    std::set<Vertice> visited;
    std::priority_queue<std::pair<Vertice, std::size_t>, std::vector<std::pair<Vertice, std::size_t>>, greater_distance> pq;
    int row = 0;
    std::size_t col = 0;
    while (std::getline(input, line)) {
        if (line == "") continue;
        for (col = 0; col < line.size(); ++col) {
            weight_table[{ row, static_cast<int>(col) }] = static_cast<std::size_t>(line[col] - '0');
            distances[{ row, static_cast<int>(col) }] = std::numeric_limits<std::size_t>::max();
        }
        ++row;
    }


    auto rows = ranges::views::iota(0, row * map_scale_factor);
    auto cols = ranges::views::iota(0, static_cast<int>(col * map_scale_factor));
    auto indices = ranges::views::cartesian_product(rows, cols);
    for (const auto &ind : indices) {
        auto [row_quot, row_rem] = std::div(std::get<0>(ind), row);
        auto [col_quot, col_rem] = std::div(std::get<1>(ind), static_cast<int>(col));
        if (row_quot == 0 && col_quot == 0) continue;
        auto original_weight = weight_table[{ row_rem, col_rem }];
        auto weight = original_weight + static_cast<std::size_t>(row_quot + col_quot);
        if (weight > 9) weight = weight % 10 + 1;
        weight_table[{ std::get<0>(ind), std::get<1>(ind) }] = weight;
        distances[{ std::get<0>(ind), std::get<1>(ind) }] = std::numeric_limits<std::size_t>::max();
    }

    Vertice source{ 0, 0 };
    distances[source] = 0;
    visited.insert(source);
    Vertice min_unvisited = source;
    while (visited.size() != distances.size()) {
        const auto neighbours = get_neigbours(min_unvisited);
        for (const auto &n : neighbours) {
            // skip the nonexisting node. i.e {-1,0}
            if (!(n.first >= 0 && n.second >= 0 && n.first < map_scale_factor * row && static_cast<std::size_t>(n.second) < map_scale_factor * col)) continue;
            if (visited.count(n)) continue;
            auto distance = distances[min_unvisited] + weight_table[n];
            if (distance < distances[n]) {
                distances[n] = distance;
                pq.push(std::pair<Vertice, std::size_t>{ n, distance });
            }
        }
        min_unvisited = pq.top().first;
        pq.pop();
        // this check is not necessary!
        // while (visited.count(min_unvisited)) {
        //     min_unvisited = pq.top().first;
        //     pq.pop();
        //     fmt::print("ola");
        // }
        visited.insert(min_unvisited);
    }
    fmt::print("The answer is {}\n", distances[{ map_scale_factor * row - 1, static_cast<int>(map_scale_factor * col - 1) }]);
}
