#include <charconv>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iterator>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/iterator/insert_iterators.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/istream.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <vector>
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day6/day6.txt" };

    std::string line;
    std::getline(input, line);
    // https://stackoverflow.com/a/48403210/15697391
    auto values = ranges::split_view(line, ',')
                  | ranges::views::transform([](auto &&rng) {
                        return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
                    })
                  | ranges::views::transform([](auto &&sv) {
                                int value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; })
                  | ranges::to<std::vector>;

    std::vector<int> queue;
    constexpr int days_to_pass = 80;
    for (int i = 0; i < days_to_pass; ++i) {
        ranges::for_each(values, [&queue](auto &v) {
            if (v == 0) {
                queue.push_back(8);
                v = 6;
            } else {
                --v;
            }
        });
        ranges::copy(queue, ranges::back_inserter(values));
        queue.clear();
    }
    fmt::print("The answer is {}\n", std::size(values));
}
