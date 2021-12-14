#include <charconv>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/nth_element.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <vector>
template<class Rng>
int median(Rng values)
{
    ranges::nth_element(values, ranges::begin(values) + static_cast<typename Rng::difference_type>(ranges::size(values) / 2));
    return values[ranges::size(values) / 2];
}

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day7/day7.txt" };

    std::string line;
    std::getline(input, line);
    // https://stackoverflow.com/a/48403210/15697391
    auto values = ranges::split_view(line, ',')
                  | ranges::view::transform([](auto &&rng) {
                        return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
                    })
                  | ranges::view::transform([](auto &&sv) {
                                int value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; })
                  | ranges::to<std::vector>;

    auto m = median(values);
    auto answer = ranges::accumulate(values, 0, std::plus{}, [m](auto v) { return std::abs(v - m); });
    fmt::print("The answer is {}\n", answer);
}
