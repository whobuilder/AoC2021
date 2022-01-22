#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/istream.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day1/day1.txt" };
    auto values = ranges::istream_view<int>(input) | ranges::to<std::vector<int>>;
    auto filtered = values
                    | ranges::views::sliding(3)
                    | ranges::views::transform([](auto v) { return v[0] + v[1] + v[2]; })
                    | ranges::views::sliding(2);
    fmt::print("The answer is {}\n", ranges::count_if(filtered, [](auto p) { return p[1] > p[0]; }));
}
