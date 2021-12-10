#include <fmt/format.h>
#include <fstream>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <vector>
int main()
{
    std::ifstream input{
        "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day3/day3.txt"
    };
    auto values_as_string =
      ranges::istream_view<std::string>(input);
    auto number_of_bits = (*std::begin(values_as_string)).size();

    auto values = values_as_string | ranges::view::transform([](auto value) { return std::stoi(value, nullptr, 2); }) | ranges::to<std::vector>;
    auto size = std::size(values);
    auto get_ith_bit = [](auto value, unsigned i) { return ((0b1 << i) & value) >> i; };

    std::vector<bool> gamma_rate_bits;
    for (unsigned i = 0; i < number_of_bits; ++i) {
        auto ones_no = ranges::count_if(values, [get_ith_bit, i](auto value) { return get_ith_bit(value, i); });
        gamma_rate_bits.push_back(static_cast<unsigned>(ones_no) > size / 2);
    }

    auto gamma_rate = ranges::accumulate(gamma_rate_bits | ranges::view::reverse, 0, [](auto acc, auto v) { return (acc << 1) | v; });

    auto epsilon_rate = ranges::accumulate(gamma_rate_bits | ranges::view::reverse, 0, [](auto acc, auto v) { return (acc << 1) | (v ^ 1); });
    fmt::print("The answer is: {}\n", gamma_rate * epsilon_rate);
}
