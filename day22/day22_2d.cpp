#include <fmt/format.h>
#include <iostream>
#include <optional>
#include <range/v3/numeric/accumulate.hpp>
#include <utility>
#include <vector>

class Rectangle
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

public:
    constexpr Rectangle() = default;
    constexpr Rectangle(std::pair<int, int> Xs, std::pair<int, int> Ys)
      : x1(Xs.first), x2(Xs.second), y1(Ys.first), y2(Ys.second) {}
    constexpr long long area() const { return (x2 - x1) * (y2 - y1); }
    [[nodiscard]] constexpr std::pair<int, int> xs() const { return { x1, x2 }; }
    [[nodiscard]] constexpr std::pair<int, int> ys() const { return { y1, y2 }; }

    friend constexpr std::optional<Rectangle> intersect(const Rectangle &r1,
      const Rectangle &r2)
    {
        const auto &left = r1.x1 < r2.x1 ? r1 : r2;
        const auto &right = r1.x1 < r2.x1 ? r2 : r1;
        const auto &lower = r1.y1 < r2.y1 ? r1 : r2;
        const auto &upper = r1.y1 < r2.y1 ? r2 : r1;

        // const auto &inner = r1.z1 < r2.z1 ? r1 : r2;
        if (left.x2 > right.x1 && upper.y1 < lower.y2) {
            return Rectangle{ { right.x1, std::min(left.x2, right.x2) },
                { upper.y1, std::min(lower.y2, upper.y2) } };
        }
        return {};
    }
    friend std::ostream &operator<<(std::ostream &os, const Rectangle &r)
    {
        os << "Xs: " << r.x1 << ".." << r.x2 << '\n';
        os << "Ys: " << r.y1 << ".." << r.y2 << '\n';
        return os;
    }
};
class CuboidCollection
{
    std::vector<std::pair<Rectangle, bool>> rectangles;
    auto process_intersections(const Rectangle &other) const -> std::vector<std::pair<Rectangle, bool>>
    {
        std::vector<std::pair<Rectangle, bool>> new_rectangles{};
        for (const auto &[r, should_add] : rectangles) {
            auto intersection = intersect(r, other);
            if (intersection) {
                new_rectangles.push_back({ std::move(intersection.value()), !should_add });
            }
        }
        return new_rectangles;
    }

public:
    long long int area() const
    {
        return ranges::accumulate(rectangles, 0LL, ranges::plus{}, [](const auto &rec_info) {
            const auto &[rect, should_add] = rec_info;
            auto area = rect.area();
            if (should_add) { return area; }
            return -area;
        });
    }
    void add(const Rectangle &other)
    {
        std::vector<std::pair<Rectangle, bool>> new_rectangles = process_intersections(other);
        rectangles.insert(rectangles.end(), new_rectangles.begin(), new_rectangles.end());
        rectangles.push_back({ other, true });
    }
    void substract(const Rectangle &other)
    {
        std::vector<std::pair<Rectangle, bool>> new_rectangles = process_intersections(other);
        rectangles.insert(rectangles.end(), new_rectangles.begin(), new_rectangles.end());
    }
};
int main()
{
    constexpr auto r1 = Rectangle({ 2, 14 }, { 4, 10 });
    constexpr auto r2 = Rectangle({ 9, 21 }, { 1, 7 });
    constexpr auto r3 = Rectangle({ 23, 24 }, { 1, 2 });
    constexpr auto r4 = Rectangle({ 13, 15 }, { 5, 6 });
    auto rectangles = CuboidCollection{};
    rectangles.add(r1);
    rectangles.substract(r2);
    rectangles.add(r3);
    rectangles.substract(r4);
    std::cout << rectangles.area() << '\n';
}