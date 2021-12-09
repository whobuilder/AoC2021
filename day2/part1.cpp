#include <algorithm>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/chunk.hpp>
#include <string>
#include <tuple>
#include <vector>
struct Position {
    int horizontal = 0;
    int depth = 0;
};
int main()
{
    std::ifstream input{
        "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day2/day2.txt"};
    auto values =
        ranges::istream_view<std::string>(input) |ranges::to<std::vector>;
    auto pairs = values | ranges::view::chunk(2);

    auto position = Position{};
    ranges::for_each(pairs, [&position](auto p){
        if (p[0] == "forward"){
            position.horizontal += stoi(p[1]);
        } else if (p[0] == "down"){
            position.depth += stoi(p[1]);
        } else if (p[0] == "up"){
            position.depth -= stoi(p[1]);
        }
    });
    fmt::print("The answer is {}\n", position.horizontal*position.depth);
}