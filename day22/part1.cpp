#include <boost/functional/hash.hpp>
#include <charconv>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <set>
#include <string>
#include <unordered_set>
struct coordinate_borders
{
    std::pair<int, int> x = { 0, 0 };
    std::pair<int, int> y = { 0, 0 };
    std::pair<int, int> z = { 0, 0 };
};
struct coordinates
{
    int x = 0;
    int y = 0;
    int z = 0;
    bool operator==(coordinates const &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};
class CoordHash
{
public:
    std::size_t operator()(const coordinates c) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, c.x);
        boost::hash_combine(seed, c.y);
        boost::hash_combine(seed, c.z);

        return seed;
    }
};

bool operator<(const coordinates &c1, const coordinates &c2)
{
    return std::tie(c1.x, c1.y, c1.z) < std::tie(c2.x, c2.y, c2.z);
}

std::pair<int, int> get_borders(std::string_view border_v)
{
    int value1;
    int value2;
    using namespace std::literals;
    auto border = std::string(border_v);
    auto value1_end = border.find(".."sv);
    std::from_chars(border.data(), border.data() + value1_end, value1);
    std::from_chars(border.data() + value1_end + 2, border.data() + border.size(), value2);
    return { value1, value2 };
}
int main()
{
    std::ifstream input_stream{ R"(D:\cpp_projects\cpp_experiments\AoC2021\day22\day22.txt)" };
    std::string line;
    std::unordered_set<coordinates, CoordHash> lights_on_cubes{};
    // std::unordered_set<coordinates, CoordHash> foo{};
    while (std::getline(input_stream, line)) {
        if (line == "") continue;
        using namespace std::literals;
        auto line_view = std::string_view(line);
        auto status = line_view.substr(0, line_view.find_first_of(" "sv));
        auto x_start = line.find("x"sv) + 2u;
        auto x_end = line_view.substr(x_start, std::size(line) - x_start).find_first_of(","sv);
        auto x_borders = line_view.substr(x_start, x_end);
        auto y_start = line.find("y"sv) + 2u;
        auto y_end = line_view.substr(y_start, std::size(line) - y_start).find_first_of(","sv);
        auto y_borders = line_view.substr(y_start, y_end);
        auto z_start = line.find("z"sv) + 2u;
        auto z_borders = line_view.substr(z_start, std::size(line) - z_start);

        auto [x1, x2] = get_borders(x_borders);
        auto [y1, y2] = get_borders(y_borders);
        auto [z1, z2] = get_borders(z_borders);
        if (x2 < -50) continue;
        if (x1 > 50) continue;
        if (y2 < -50) continue;
        if (y1 > 50) continue;
        if (z2 < -50) continue;
        if (z1 > 50) continue;
        auto xs1 = std::max(x1, -50);
        auto xs2 = std::min(x2, 50);
        auto ys1 = std::max(y1, -50);
        auto ys2 = std::min(y2, 50);
        auto zs1 = std::max(z1, -50);
        auto zs2 = std::min(z2, 50);
        auto coords_view = ranges::views::cartesian_product(
          ranges::views::iota(xs1, xs2 + 1),
          ranges::views::iota(ys1, ys2 + 1),
          ranges::views::iota(zs1, zs2 + 1));

        ranges::for_each(coords_view, [&lights_on_cubes, status](const auto &c) {
            auto coord = coordinates{ ranges::get<0>(c), ranges::get<1>(c), ranges::get<2>(c) };
            if (status == "on" && coord.x >= -50 && coord.x <= 50 && coord.y >= -50 && coord.y <= 50 && coord.z >= -50 && coord.z <= 50) {
                lights_on_cubes.insert(
                  coord);
            } else if (status == "off") {
                auto it = lights_on_cubes.find(coord);
                if (it != lights_on_cubes.end()) lights_on_cubes.erase(it);
            }
        });
        // fmt::print("x1: {}\n", x1);
        // fmt::print("x2: {}\n", x2);
        // fmt::print("y1: {}\n", y1);
        // fmt::print("y2: {}\n", y2);
        // fmt::print("z1: {}\n", z1);
        // fmt::print("z2: {}\n", z2);
        // fmt::print("{}\n", status);
    }

    fmt::print("{}\n", lights_on_cubes.size());
}
