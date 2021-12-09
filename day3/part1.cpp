#include <fmt/format.h>
#include <fstream>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/to_container.hpp>
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

    std::vector<bool> gamma_rate_bits;
    for (unsigned i = 0; i < number_of_bits; ++i) {
        auto ones_no = ranges::count_if(values, [i](auto value) { return ((0b1 << i) & value) >> i; });
        gamma_rate_bits.push_back(static_cast<unsigned>(ones_no) > size / 2);
    }


    auto bits_to_decimal = [](auto bits, bool reverse) {
        auto as_decimal = 0;
        for (unsigned i = 0; i < std::size(bits); ++i) {
            bool is_one_bit = bits[i];
            if (reverse) is_one_bit = !is_one_bit;
            if (is_one_bit) {
                as_decimal |= (0b1 << i);
            }
        }
        return as_decimal;
    };

    auto gamma_rate = bits_to_decimal(gamma_rate_bits, false);
    auto epsilon_rate = bits_to_decimal(gamma_rate_bits, true);
    fmt::print("The answer is: {}\n", gamma_rate * epsilon_rate);
}
