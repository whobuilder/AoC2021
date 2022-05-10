#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <map>
#include <range/v3/numeric/accumulate.hpp>
#include <utility>
#include <vector>

using InputImage = std::map<std::pair<std::size_t, std::size_t>, unsigned short>;
struct ImageInfo
{
    std::string image_enhancement_algorithm;
    InputImage input_image;
    std::size_t rows;
    std::size_t columns;
};

ImageInfo read_input()
{
    std::ifstream input_stream{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day20/day20.txt" };
    std::string line;

    std::string image_enhancement_algorithm;
    std::getline(input_stream, image_enhancement_algorithm);
    // std::vector<std::vector<short>> input_image;
    InputImage input_image;
    std::size_t i = 0;
    std::size_t j = 0;
    while (std::getline(input_stream, line)) {
        if (line == "") continue;
        // std::vector<short> row;
        j = 0;
        for (const char &c : line) {
            if (c == '#') {
                input_image[{ i, j }] = 1;
                // row.push_back(1);
            } else if (c == '.') {
                // row.push_back(0);
                input_image[{ i, j }] = 0;
            }
            ++j;
        }
        // input_image.push_back(row);
        ++i;
    }
    return { image_enhancement_algorithm, input_image, i, j };
}

ImageInfo enhance_image(const ImageInfo &image, bool lighted)
{
    ImageInfo enhanced_image;
    enhanced_image.columns = image.columns + 2;
    enhanced_image.rows = image.rows + 2;
    enhanced_image.image_enhancement_algorithm = image.image_enhancement_algorithm;
    for (const auto &node : image.input_image) {
        const auto &[coord, value] = node;
        auto x = coord.first;
        auto y = coord.second;
        std::pair<std::size_t, std::size_t> new_coord{ x + 1, y + 1 };
        enhanced_image.input_image[new_coord] = value;
    }
    for (std::size_t j = 0; j < enhanced_image.columns; ++j) {
        enhanced_image.input_image[{ 0, j }] = lighted;
        // enhanced_image.input_image[{ 1, j }] = lighted;
        enhanced_image.input_image[{ enhanced_image.rows - 1, j }] = lighted;
        // enhanced_image.input_image[{ enhanced_image.rows - 2, j }] = lighted;
    }

    for (std::size_t i = 0; i < enhanced_image.rows; ++i) {
        enhanced_image.input_image[{ i, 0 }] = lighted;
        // enhanced_image.input_image[{ i, 1 }] = lighted;
        enhanced_image.input_image[{ i, enhanced_image.columns - 1 }] = lighted;
        // enhanced_image.input_image[{ i, enhanced_image.columns - 2 }] = lighted;
    }
    return enhanced_image;
}
unsigned short get_output_pixel(const ImageInfo &image, std::pair<std::size_t, std::size_t> coord, bool filler = false)
{
    std::size_t n = 9;
    std::size_t sum = 0;
    for (long long i = static_cast<long long>(coord.first) - 1; i < static_cast<long long>(coord.first + 2); ++i) {
        for (long long j = static_cast<long long>(coord.second) - 1; j < static_cast<long long>(coord.second + 2); ++j) {
            --n;
            unsigned short value = filler;
            try {
                value = image.input_image.at({ i, j });
            } catch (const std::out_of_range &) {
                value = filler;
            }
            sum = sum + static_cast<std::size_t>(value << n);
        }
    }
    // fmt::print("sum: {}\n", sum);

    if (char pixel = image.image_enhancement_algorithm[sum]; pixel == '#') {
        return 1;
    }
    return 0;
}
ImageInfo apply_image_enhancement_algorithm(const ImageInfo &image_info, bool lighted)
{
    ImageInfo enhanced_image_info = image_info;
    for (std::size_t i = 0; i < enhanced_image_info.rows; ++i) {
        for (std::size_t j = 0; j < enhanced_image_info.columns; ++j) {
            enhanced_image_info.input_image[{ i, j }] = get_output_pixel(image_info, { i, j }, lighted);
        }
    }
    return enhanced_image_info;
}

std::size_t count_light_pixels(const ImageInfo &image)
{
    return ranges::accumulate(image.input_image, 0ULL, std::plus{}, [](const auto &node) { return node.second; });
}

int main()
{
    auto image_info = read_input();

    std::size_t max_step = 50;

    for (std::size_t i = 1; i < max_step + 1; ++i) {
        bool lighted = (i % 2 == 0) && image_info.image_enhancement_algorithm[0] == '#';

        auto current_enhanced = enhance_image(image_info, lighted);

        image_info = apply_image_enhancement_algorithm(current_enhanced, lighted);
    }


    fmt::print("{}\n", count_light_pixels(image_info));
}
