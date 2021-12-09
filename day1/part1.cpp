#include <fmt/format.h>
#include <fstream>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/sliding.hpp>
#include <vector>

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day1/day1.txt" };
    auto values = ranges::istream_view<int>(input) | ranges::to<std::vector<int>>;
    auto filtered = values
                    | ranges::view::sliding(2);
    fmt::print("The answer is {}\n", ranges::count_if(filtered, [](auto p) { return p[1] > p[0]; }));
}
