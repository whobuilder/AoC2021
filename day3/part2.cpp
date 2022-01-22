#include <algorithm>
#include <fmt/format.h>
#include <fstream>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/istream.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <vector>

template<class InputIt>
bool get_most_common_bit(InputIt first, InputIt last, std::size_t number_of_bits, unsigned ind)
{
    auto get_ith_position_bit = [number_of_bits](auto value, unsigned i) { return ((0b1 << (number_of_bits - 1 - i)) & value) >> (number_of_bits - 1 - i); };
    auto ones_no = std::count_if(first, last, [get_ith_position_bit, ind](auto value) { return get_ith_position_bit(value, ind); });
    return static_cast<unsigned>(ones_no) >= (std::distance(first, last) - ones_no);
}

int main()
{
    std::ifstream input{
        "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day3/day3.txt"
    };
    auto values_as_string =
      ranges::istream_view<std::string>(input);
    auto number_of_bits = (*std::begin(values_as_string)).size();
    auto values = values_as_string | ranges::views::transform([](auto value) { return std::stoi(value, nullptr, 2); }) | ranges::to<std::vector>;

    auto get_ith_position_bit = [number_of_bits](auto value, unsigned i) { return ((0b1 << (number_of_bits - 1 - i)) & value) >> (number_of_bits - 1 - i); };

    auto end_iter = std::end(values);
    for (unsigned i = 0; i < number_of_bits; ++i) {
        bool most_common_value = get_most_common_bit(std::begin(values), end_iter, number_of_bits, i);
        end_iter = std::partition(std::begin(values), end_iter, [get_ith_position_bit, i, most_common_value](auto value) { return static_cast<bool>(get_ith_position_bit(value, i)) == most_common_value; });
        if (std::distance(std::begin(values), end_iter) == 1) break;
    }
    auto oxygen_generator_rating = *std::begin(values);
    end_iter = std::end(values);
    for (unsigned i = 0; i < number_of_bits; ++i) {
        bool most_common_value = get_most_common_bit(std::begin(values), end_iter, number_of_bits, i);
        end_iter = std::partition(std::begin(values), end_iter, [get_ith_position_bit, i, most_common_value](auto value) { return static_cast<bool>(get_ith_position_bit(value, i)) != most_common_value; });
        if (std::distance(std::begin(values), end_iter) == 1) break;
    }
    auto co2_scrubber_rating = *std::begin(values);
    fmt::print("The answer is {}\n", oxygen_generator_rating * co2_scrubber_rating);
}
