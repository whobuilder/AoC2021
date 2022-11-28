#include <charconv>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <range/v3/numeric/accumulate.hpp>
#include <utility>
#include <vector>
class Cuboid
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
    int z1 = 0;
    int z2 = 0;

public:
    constexpr Cuboid() = default;
    constexpr Cuboid(std::pair<int, int> Xs, std::pair<int, int> Ys, std::pair<int, int> Zs)
      : x1(Xs.first), x2(Xs.second), y1(Ys.first), y2(Ys.second), z1(Zs.first), z2(Zs.second) {}
    constexpr std::size_t volume() const { return static_cast<std::size_t>(x2 - x1 + 1) * static_cast<std::size_t>(y2 - y1 + 1) * static_cast<std::size_t>(z2 - z1 + 1); }
    [[nodiscard]] constexpr std::pair<int, int> xs() const { return { x1, x2 }; }
    [[nodiscard]] constexpr std::pair<int, int> ys() const { return { y1, y2 }; }
    [[nodiscard]] constexpr std::pair<int, int> zs() const { return { z1, z2 }; }

    friend constexpr std::optional<Cuboid> intersect(const Cuboid &c1,
      const Cuboid &c2)
    {
        const auto &left = c1.x1 < c2.x1 ? c1 : c2;
        const auto &right = c1.x1 < c2.x1 ? c2 : c1;
        const auto &lower = c1.y1 < c2.y1 ? c1 : c2;
        const auto &upper = c1.y1 < c2.y1 ? c2 : c1;

        const auto &below = c1.z1 < c2.z1 ? c1 : c2;
        const auto &above = c1.z1 < c2.z1 ? c2 : c1;

        if (left.x2 > right.x1 && upper.y1 < lower.y2 && below.z2 > above.z1) {
            return Cuboid{
                { right.x1, std::min(left.x2, right.x2) },
                { upper.y1, std::min(lower.y2, upper.y2) },
                { above.z1, std::min(below.z2, above.z2) }
            };
        }
        return {};
    }
    friend std::ostream &operator<<(std::ostream &os, const Cuboid &c)
    {
        os << "Xs: " << c.x1 << ".." << c.x2 << '\n';
        os << "Ys: " << c.y1 << ".." << c.y2 << '\n';
        os << "Zs: " << c.z1 << ".." << c.z2 << '\n';
        return os;
    }
};
class CuboidCollection
{
    std::vector<std::pair<Cuboid, bool>> cuboids;
    auto process_intersections(const Cuboid &other) const -> std::vector<std::pair<Cuboid, bool>>
    {
        std::vector<std::pair<Cuboid, bool>> new_rectangles{};
        for (const auto &[r, should_add] : cuboids) {
            auto intersection = intersect(r, other);
            if (intersection) {
                new_rectangles.emplace_back(std::move(intersection.value()), !should_add);
            }
        }
        return new_rectangles;
    }

public:
    std::size_t volume() const
    {
        std::size_t result = 0;
        for (const auto &[c, should_add] : cuboids) {
            if (should_add) {
                result += c.volume();
            } else {
                result -= c.volume();
            }
        }
        return result;
        // return ranges::accumulate(cuboids, std::size_t{ 0 }, ranges::plus{}, [](const auto &cuboid_info) {
        //     const auto &[cub, should_add] = cuboid_info;
        //     auto volume = cub.volume();
        //     if (should_add) { return volume; }
        //     return -volume;
        // });
    }
    void add(const Cuboid &other)
    {
        std::vector<std::pair<Cuboid, bool>> new_rectangles = process_intersections(other);
        cuboids.insert(cuboids.end(), new_rectangles.begin(), new_rectangles.end());
        cuboids.emplace_back(other, true);
    }
    void substract(const Cuboid &other)
    {
        std::vector<std::pair<Cuboid, bool>> new_rectangles = process_intersections(other);
        cuboids.insert(cuboids.end(), new_rectangles.begin(), new_rectangles.end());
    }
};


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
    CuboidCollection cuboids{};
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

        Cuboid c{ get_borders(x_borders), get_borders(y_borders), get_borders(z_borders) };
        if (status == "on") {
            cuboids.add(c);
        } else if (status == "off") {
            cuboids.substract(c);
        }
    }
    fmt::print("The answer is {}\n", cuboids.volume());
}