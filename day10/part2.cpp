#include <deque>
#include <fmt/format.h>
#include <fstream>
#include <iterator>
#include <numeric>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/nth_element.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/transform.hpp>
#include <unordered_map>
#include <vector>
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day10/day10.txt" };

    auto iview = ranges::istream_view<std::string>(input);
    std::unordered_map<char, char> matching_characters{ { '(', ')' }, { '[', ']' }, { '{', '}' }, { '<', '>' } };
    std::vector<unsigned long long> scores;

    ranges::for_each(iview, [&matching_characters, &scores](const auto &line) {
        bool corrupted = false;
        std::deque<char> opening_char;
        for (char c : line) {
            if (matching_characters.count(c)) {
                opening_char.push_back(c);
            } else {
                if (opening_char.size() == 0 || matching_characters[opening_char.back()] != c) {
                    corrupted = true;
                    break;// corrupted line
                } else if (matching_characters[opening_char.back()] == c) {
                    opening_char.pop_back();
                }
            }
        }
        if (!corrupted) {
            auto closing_chars = ranges::views::reverse(opening_char)
                                 | ranges::views::transform([&matching_characters](auto c) { return matching_characters[c]; });
            scores.push_back(ranges::accumulate(closing_chars, 0ull, [](auto sum, auto c) {
                auto res = sum * 5;
                if (c == ')') res += 1;
                if (c == ']') res += 2;
                if (c == '}') res += 3;
                if (c == '>') res += 4;
                return res;
            }));
        }
    });
    ranges::nth_element(scores, std::begin(scores) + static_cast<long long>(std::size(scores) / 2));
    auto answer = scores[std::size(scores) / 2];
    fmt::print("The answer is {}\n", answer);
}
