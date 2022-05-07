#include <algorithm>
#include <array>
#include <blaze/Math.h>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <set>
#include <string>
#include <string_view>
#include <vector>
using RotationMatrix = blaze::StaticMatrix<int, 3UL, 3UL>;
auto cos_value = [](double angle) { return static_cast<int>(std::round(std::cos(angle))); };
auto sin_value = [](double angle) { return static_cast<int>(std::round(std::sin(angle))); };
struct ScannerInfo
{
    std::string name;
    blaze::DynamicMatrix<int> original_report;
    blaze::DynamicMatrix<int> modified_report;
    blaze::StaticVector<int, 3UL> relative_position;
};

RotationMatrix get_r_x(double alfa)
{
    return {
        { 1, 0, 0 },
        { 0, cos_value(alfa), -sin_value(alfa) },
        { 0, sin_value(alfa), cos_value(alfa) }
    };
}

RotationMatrix get_r_y(double beta)
{
    return {
        { cos_value(beta), 0, sin_value(beta) },
        { 0, 1, 0 },
        { -sin_value(beta), 0, cos_value(beta) }
    };
}

RotationMatrix get_r_z(double gamma)
{
    return {
        { cos_value(gamma), -sin_value(gamma), 0 },
        { sin_value(gamma), cos_value(gamma), 0 },
        { 0, 0, 1 }
    };
}


std::vector<ScannerInfo> get_scanners_info(const std::string &input_file)
{
    std::ifstream input_stream{ input_file };
    std::string line;
    std::vector<ScannerInfo> scanners_info{};
    std::vector<std::array<int, 3>> scanner_report;
    std::string scanner_name;
    while (std::getline(input_stream, line)) {
        if (line == "") {
            blaze::DynamicMatrix<int> scanner = blaze::generate(scanner_report.size(), 3UL, [&scanner_report](std::size_t i, std::size_t j) { return scanner_report[i][j]; });
            ScannerInfo info;
            info.original_report = scanner;
            info.name = scanner_name;
            scanners_info.push_back(info);
            scanner_report.clear();
        } else if (line[0] == '-' && line[1] == '-') {
            scanner_name = line;

        } else {
            auto values = ranges::views::split(line, ',')
                          | ranges::views::transform([](const auto &rng) {
                                return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
                            })
                          | ranges::views::transform([](const auto &sv) {
                                int value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; });
            std::array<int, 3UL> coordinates;
            ranges::copy(values, std::begin(coordinates));
            scanner_report.push_back(coordinates);
        }
    }
    if (!scanner_report.empty()) {
        blaze::DynamicMatrix<int> scanner = blaze::generate(scanner_report.size(), 3UL, [&scanner_report](std::size_t i, std::size_t j) { return scanner_report[i][j]; });
        ScannerInfo info;
        info.original_report = scanner;
        info.name = scanner_name;
        scanners_info.push_back(info);
        scanner_report.clear();
    }
    return scanners_info;
}

std::size_t count_matched_beacons(const blaze::DynamicMatrix<int> &left, const blaze::DynamicMatrix<int> &right)
{
    std::size_t matched_count = 0;
    for (std::size_t left_ind = 0; left_ind < left.rows(); ++left_ind) {
        auto left_row = blaze::row(left, left_ind);
        for (std::size_t right_ind = 0; right_ind < right.rows(); ++right_ind) {
            auto right_row = blaze::row(right, right_ind);
            if (left_row == right_row) {
                ++matched_count;
                break;
            }
        }
    }
    return matched_count;
}

template<typename T>
void print_out(const T &vec)
{
    std::cout << vec << '\n';
}
