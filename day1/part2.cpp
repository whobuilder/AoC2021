#include <fmt/format.h>
#include <fmt/ranges.h>
#include <range/v3/istream_range.hpp>
#include <fstream>
#include <range/v3/to_container.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>

int main(){
    std::ifstream input{"/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day1/day1.txt"};
    auto values = ranges::istream_view<int>(input) | ranges::to<std::vector<int>>;
    auto filtered = values 
        | ranges::view::sliding(3) 
        | ranges::view::transform([](auto v){return v[0]+v[1]+v[2];})
        | ranges::view::sliding(2) 
        | ranges::view::filter([](auto p){return p[1]>p[0];}) | ranges::to<std::vector>;
    fmt::print("The answer is {}\n", std::size(filtered));
}   
