#include <fmt/format.h>
#include <fstream>
#include <map>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/minmax_element.hpp>
#include <range/v3/view/sliding.hpp>
#include <string>
#include <unordered_map>
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day14/day14.txt" };
    std::string polymer_template;
    std::getline(input, polymer_template);
    std::string line;
    std::map<std::pair<char, char>, char> insertion_rules;
    while (std::getline(input, line)) {
        if (line == "") continue;
        insertion_rules[{ line[0], line[1] }] = line[6];
    }
    std::map<std::pair<char, char>, std::size_t> polymer;
    for (const auto &p : ranges::views::sliding(polymer_template, 2)) {
        auto couple = std::pair<char, char>{ *ranges::begin(p), *ranges::next(ranges::begin(p)) };
        if (!polymer.count(couple)) polymer[couple] = 0;
        ++polymer[couple];
    }
    auto polymer_copy = polymer;
    for (int i = 0; i < 40; ++i) {
        for (const auto &[p, count] : polymer) {

            auto new_element = insertion_rules[p];
            auto new_first_pair = std::pair<char, char>{ p.first, new_element };
            auto new_second_pair = std::pair<char, char>{ new_element, p.second };
            polymer_copy[new_first_pair] += count;
            polymer_copy[new_second_pair] += count;
            polymer_copy[p] -= count;
            if (polymer_copy[p] < 1) {
                polymer_copy.erase(p);
            }
        }
        polymer = polymer_copy;
    }
    std::unordered_map<char, std::size_t> occurences;
    occurences[polymer_template.back()] = 1;
    ranges::for_each(polymer, [&occurences](auto c) {
        const auto&[p,count] =c;
        if (!occurences.count(p.first)) {
            occurences[p.first]=0;
        }

        occurences[p.first]+=count; });
    auto [min, max] = ranges::minmax_element(occurences, std::less<>{}, [](auto c) { return c.second; });
    fmt::print("The answer is {}\n", (*max).second - (*min).second);
}
