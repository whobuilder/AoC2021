#include <array>
#include <cstdlib>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <limits>
#include <map>
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

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day15/day15.txt" };
    std::string line;
    std::map<Vertice, std::size_t> weight_table;
    std::map<Vertice, std::size_t> distances;
    std::set<Vertice> unvisited;
    int row = 0;
    std::size_t col = 0;
    while (std::getline(input, line)) {
        if (line == "") continue;
        for (col = 0; col < line.size(); ++col) {
            weight_table[{ row, static_cast<int>(col) }] = static_cast<std::size_t>(line[col] - '0');
            distances[{ row, static_cast<int>(col) }] = std::numeric_limits<std::size_t>::max();
            unvisited.insert({ row, static_cast<int>(col) });
        }
        ++row;
    }

    Vertice source{ 0, 0 };
    distances[source] = 0;
    while (!unvisited.empty()) {
        auto min_iter = ranges::min_element(unvisited, std::less{}, [&distances](const auto &v) { return distances[v]; });
        auto node = unvisited.extract(min_iter);
        const auto neighbours = get_neigbours(node.value());
        for (const auto &n : neighbours) {
            if (!distances.count(n)) continue;
            if (!unvisited.count(n)) continue;
            auto distance = distances[node.value()] + weight_table[n];
            if (distance < distances[n]) distances[n] = distance;
        }
    }
    fmt::print("The answer is {}\n", distances[{ row - 1, static_cast<int>(col - 1) }]);
}
