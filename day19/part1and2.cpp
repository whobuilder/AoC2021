#include "helper_functions.hpp"
#include "rotations.hpp"
#include <algorithm>
#include <blaze/Math.h>
#include <charconv>
#include <cmath>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <map>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

void part1(const std::vector<ScannerInfo> &scanners_info)
{
    std::set<std::array<int, 3>> all_beacons;
    for (const auto &s : scanners_info) {
        for (std::size_t i = 0; i < s.modified_report.rows(); ++i) {
            auto beacon = blaze::row(s.modified_report, i);
            all_beacons.insert({ beacon[0], beacon[1], beacon[2] });
        }
    }
    fmt::print("Part 1 Answer is {}\n", all_beacons.size());
}

void part2(const std::vector<ScannerInfo> &scanners_info)
{
    auto scanner_pairs = ranges::views::cartesian_product(scanners_info, scanners_info);
    auto calculate_manhattan_distance = [](const auto &pair) {
        auto diff = ranges::get<0>(pair).relative_position - ranges::get<1>(pair).relative_position;
        return std::abs(diff[0]) + std::abs(diff[1]) + std::abs(diff[2]);
    };
    auto max_pair = *ranges::max_element(
      scanner_pairs,
      std::less{},
      calculate_manhattan_distance);
    auto answer = calculate_manhattan_distance(max_pair);
    fmt::print("Part 2 Answer is {}\n", answer);
}
int main()
{
    auto scanners_info = get_scanners_info("/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day19/day19.txt");
    std::set<std::string, std::less<>> found_scanners{ scanners_info[0].name };
    scanners_info[0].modified_report = scanners_info[0].original_report;
    std::vector<ScannerInfo> found_scanners_info{ scanners_info[0] };
    std::stack<ScannerInfo> scanners_to_match;
    scanners_to_match.push(scanners_info[0]);
    while (found_scanners.size() != scanners_info.size()) {
        fmt::print("number of remaining scanners: {}\n", scanners_info.size() - found_scanners.size());
        if (scanners_to_match.empty()) {
            fmt::print("Unexpected behaviour!!\n");
            break;
        }
        auto s1 = scanners_to_match.top();
        scanners_to_match.pop();
        for (auto &s2 : scanners_info) {
            bool matched_found = false;
            if (s1.name == s2.name || found_scanners.count(s2.name)) { continue; }
            for (std::size_t s1_ind = 0; s1_ind < s1.modified_report.rows(); ++s1_ind) {
                if (matched_found) { break; }
                auto beacon1 = blaze::row(s1.modified_report, s1_ind);
                for (const auto &rot : rotations_hard_coded) {
                    if (matched_found) { break; }
                    auto rotated = blaze::trans(rot * blaze::trans(s2.original_report));
                    for (std::size_t s2_ind = 0; s2_ind < rotated.rows(); ++s2_ind) {
                        if (matched_found) { break; }
                        auto beacon2 = blaze::row(rotated, s2_ind);
                        auto diff = beacon2 - beacon1;
                        blaze::DynamicMatrix<int>
                          diff_matrix = blaze::generate(
                            rotated.rows(),
                            3,
                            [&diff](std::size_t /*i*/, std::size_t j) { return diff[j]; });
                        blaze::DynamicMatrix<int> s2_shifted = rotated - diff_matrix;
                        if (count_matched_beacons(s1.modified_report, s2_shifted) >= 12) {
                            matched_found = true;
                            s2.modified_report = s2_shifted;
                            s2.relative_position = { diff[0], diff[1], diff[2] };
                            found_scanners.insert(s2.name);
                            scanners_to_match.push(s2);
                        }
                    }
                }
            }
        }
    }
    part1(scanners_info);
    part2(scanners_info);
}
