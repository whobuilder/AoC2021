#include <algorithm>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/istream.hpp>
#include <string>
#include <tuple>
#include <vector>
struct Position
{
    int horizontal = 0;
    int depth = 0;
    int aim = 0;
};
int main()
{
    std::ifstream input{
        "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day2/day2.txt"
    };
    auto values =
      ranges::istream_view<std::string>(input) | ranges::to<std::vector>;
    auto pairs = values | ranges::views::chunk(2);

    auto position = Position{};
    ranges::for_each(pairs, [&position](auto p) {
        if (p[0] == "forward") {
            auto forward = stoi(p[1]);
            position.horizontal += forward;
            position.depth += position.aim * forward;
        } else if (p[0] == "down") {
            position.aim += stoi(p[1]);
        } else if (p[0] == "up") {
            position.aim -= stoi(p[1]);
        }
    });
    fmt::print("The answer is {}\n", position.horizontal * position.depth);
}
