#include <deque>
#include <fmt/format.h>
#include <fstream>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <unordered_map>
#include <vector>
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day10/day10.txt" };

    auto iview = ranges::istream_view<std::string>(input);
    std::unordered_map<char, char> matching_characters{ { '(', ')' }, { '[', ']' }, { '{', '}' }, { '<', '>' } };
    std::vector<char> breaking_chars;
    ranges::for_each(iview, [&matching_characters, &breaking_chars](const auto &line) {
        std::deque<char> opening_char;
        for (char c : line)
            if (matching_characters.count(c)) {
                opening_char.push_back(c);
            } else {
                if (opening_char.size() == 0) {
                    breaking_chars.push_back(c);
                    break;
                } else if (matching_characters[opening_char.back()] == c) {
                    opening_char.pop_back();
                } else {
                    breaking_chars.push_back(c);
                    break;
                }
            }
    });

    // ): 3 points.
    // ]: 57 points.
    // }: 1197 points.
    // >: 25137 points.
    auto answer = ranges::accumulate(breaking_chars, 0, {}, [](auto c) {
        if (c == ')') return 3;
        if (c == ']') return 57;
        if (c == '}') return 1197;
        if (c == '>') return 25137;
        throw std::exception{};
    });
    fmt::print("The answer is {}\n", answer);
}
