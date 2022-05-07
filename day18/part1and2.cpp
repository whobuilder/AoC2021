#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <utility>
#include <vector>

enum class ACTION {
    EXPLODE,
    SPLIT,
    NOACTION
};

using SnailNumber = std::vector<std::pair<int, int>>;
SnailNumber parse(std::string_view snail_number)
{
    int depth = 0;
    std::vector<std::pair<int, int>> numbers{};
    for (const auto c : snail_number) {
        int value = c - '0';
        if (value == 43 /*[*/) {
            ++depth;
        } else if (value == 45 /*]*/) {
            --depth;
        } else if (value >= 0 && value <= 9) {
            numbers.emplace_back(value, depth);
        }
    }
    return numbers;
}

SnailNumber add_numbers(const SnailNumber &left, const SnailNumber &right)
{
    return ranges::views::concat(left, right)
           | ranges::views::transform([](const auto &p) { return std::pair<int, int>{ p.first, p.second + 1 }; })
           | ranges::to<std::vector>;
}

void explode(SnailNumber &number, std::size_t pair_first_element_ind)
{
    std::size_t pair_second_element_ind = pair_first_element_ind + 1;
    std::size_t left_to_pair_ind = pair_first_element_ind - 1;
    std::size_t right_to_pair_ind = pair_second_element_ind + 1;
    if (pair_first_element_ind > 0) {
        number[left_to_pair_ind].first += number[pair_first_element_ind].first;
    }
    if (right_to_pair_ind < number.size()) {
        number[right_to_pair_ind].first += number[pair_second_element_ind].first;
    }
    number[pair_first_element_ind].first = 0;
    --number[pair_first_element_ind].second;
    number.erase(std::next(number.begin(), static_cast<std::make_signed_t<std::size_t>>(pair_second_element_ind)));
}

void split(SnailNumber &number, std::size_t ind)
{
    auto value = number[ind].first;
    auto depth = number[ind].second;
    auto left = std::pair<int, int>{ (value / 2), depth + 1 };
    auto right = value % 2 ? std::pair<int, int>{ left.first + 1, left.second } : std::pair<int, int>{ left.first, left.second };
    number[ind] = left;
    number.insert(std::next(number.begin(), static_cast<std::make_signed_t<std::size_t>>(ind + 1)), right);
}

auto decide(const SnailNumber &number)
{
    auto enumerated_number = ranges::views::enumerate(number);
    for (const auto &[ind, p] : enumerated_number) {
        if (p.second > 4) return std::pair{ ACTION::EXPLODE, ind };
    }
    for (const auto &[ind, p] : enumerated_number) {
        if (p.first > 9) return std::pair{ ACTION::SPLIT, ind };
    }
    return std::pair{ ACTION::NOACTION, static_cast<std::size_t>(0) };
}

void reduce(SnailNumber &number)
{
    while (true) {
        auto [action, ind] = decide(number);
        if (action == ACTION::NOACTION) break;
        if (action == ACTION::EXPLODE)
            explode(number, ind);
        else if (action == ACTION::SPLIT)
            split(number, ind);
    }
}
auto pairs_at_depth(const SnailNumber &number, int depth)
{
    return ranges::views::enumerate(number)
           | ranges::views::sliding(2)
           | ranges::views::filter([depth](const auto &p) {
                 return p[0].second.second == p[1].second.second && p[0].second.second == depth;
             })
           | ranges::views::transform([](const auto &p) { return p[0].first; });
}

int get_max_depth(const SnailNumber &number)
{
    auto [_, max_depth] = *ranges::max_element(number, std::less{}, &std::pair<int, int>::second);
    return max_depth;
}

int sum(SnailNumber &number)
{
    auto depth = get_max_depth(number);
    auto depth_range = ranges::views::iota(1, depth + 1) | ranges::views::reverse;
    for (auto d : depth_range) {
        auto pair_indices = pairs_at_depth(number, d);
        std::vector<std::size_t> to_delete_indices{};
        for (auto ind : pair_indices) {
            if (number[ind].second != d) continue;
            number[ind].first = number[ind].first * 3 + number[ind + 1].first * 2;
            --number[ind].second;
            --number[ind + 1].second;
            to_delete_indices.push_back(ind + 1);
        }
        for (const auto [i, ind] : ranges::views::enumerate(to_delete_indices)) {
            number.erase(std::next(number.begin(), static_cast<std::make_signed_t<std::size_t>>(ind - i)));
        }
    }
    return number[0].first;
}

void part1(std::vector<SnailNumber> numbers)
{
    auto left = numbers[0];
    for (auto right : numbers | ranges::views::drop(1)) {
        left = add_numbers(left, right);
        reduce(left);
    }
    fmt::print("The answer is {}\n", sum(left));
}

void part2(std::vector<SnailNumber> numbers)
{
    int max_sum = -1;
    for (auto [left, right] : ranges::views::cartesian_product(numbers, numbers)) {
        auto n = add_numbers(left, right);
        reduce(n);
        auto res = sum(n);
        if (res > max_sum) max_sum = res;
    }
    fmt::print("The answer is {}\n", max_sum);
}
int main()
{
    std::ifstream input_stream{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day18/day18.txt" };
    std::string snail_number;
    std::vector<SnailNumber> numbers{};
    while (std::getline(input_stream, snail_number)) {
        numbers.push_back(parse(snail_number));
    }
    part1(numbers);
    part2(numbers);
}
