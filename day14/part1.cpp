#include <fmt/format.h>
#include <fstream>
#include <map>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/minmax_element.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/transform.hpp>
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
    for (int i = 0; i < 10; ++i) {
        auto template_pair = ranges::view::sliding(polymer_template, 2) | ranges::view::transform([](const auto &p) { return std::pair<char, char>{ *ranges::begin(p), *ranges::next(ranges::begin(p)) }; });
        std::string template_copy = polymer_template;
        for (const auto &p : ranges::view::enumerate(template_pair)) {
            auto ind = 2 * p.first + 1;
            template_copy.insert(ind, 1, insertion_rules[p.second]);
        }
        polymer_template = template_copy;
    }
    std::unordered_map<char, std::size_t> occurences;
    ranges::for_each(polymer_template, [&occurences](auto c) {
        if (!occurences.count(c)) occurences[c]=0;
        ++occurences[c]; });
    auto [min, max] = ranges::minmax_element(occurences, std::less<>{}, [](auto c) { return c.second; });

    fmt::print("The answer is {}\n", (*max).second - (*min).second);
}
