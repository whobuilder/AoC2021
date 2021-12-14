#include <charconv>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iterator>
#include <range/v3/action/sort.hpp>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/rotate.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/iterator/insert_iterators.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/group_by.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <utility>
#include <vector>
int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day6/day6.txt" };

    std::string line;
    std::getline(input, line);
    // https://stackoverflow.com/a/48403210/15697391
    auto remaining_days = ranges::split_view(line, ',')
                          | ranges::view::transform([](auto &&rng) {
                                return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
                            })
                          | ranges::view::transform([](auto &&sv) {
                                unsigned value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; })
                          | ranges::to<std::vector>;

    std::array<unsigned long long, 9> buckets{};
    for (auto d : remaining_days) {
        buckets[d]++;
    }

    constexpr std::size_t days_to_pass = 256;
    for (std::size_t i = 0; i < days_to_pass; ++i) {
        ranges::rotate(buckets, ranges::next(ranges::begin(buckets)));
        buckets[6] += buckets[8];
    }
    auto answer = ranges::accumulate(buckets, 0ull);
    fmt::print("The answer is {}\n", answer);
}
