#include <charconv>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop_while.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/take_while.hpp>
#include <set>
#include <utility>
#include <vector>

template<typename T>
auto get_y_positions_and_time(T velocities, int min_y, int max_y)
{
    return velocities
           | ranges::views::partial_sum
           | ranges::views::take_while([max_y](const auto &v) { return v <= max_y; })
           | ranges::views::enumerate
           | ranges::views::filter([min_y](const auto &f) { return f.second >= min_y; });
}
void get_time_and_speed_y_positive_start(std::vector<std::pair<std::size_t, int>> &time_and_vertical_velocity, int min_y, int max_y)
{

    for (int initial_vel = 0; initial_vel <= max_y - 1; ++initial_vel) {
        auto pos_start_velocities = ranges::views::iota(initial_vel + 1);

        auto positions_for_positive_velocities = get_y_positions_and_time(pos_start_velocities, min_y, max_y);

        for (auto pos : positions_for_positive_velocities) {
            auto time_total = static_cast<std::size_t>(initial_vel) * 2 + 1 + pos.first + 1;
            time_and_vertical_velocity.emplace_back(time_total, initial_vel + 1);
        }
    }
}

void get_time_and_speed_y_negative_start(std::vector<std::pair<std::size_t, int>> &time_and_vertical_velocity, int min_y, int max_y)
{
    for (int initial_vel = 1; initial_vel <= max_y; ++initial_vel) {
        auto pos_start_velocities = ranges::views::iota(initial_vel);

        auto positions_for_negative_velocities = get_y_positions_and_time(pos_start_velocities, min_y, max_y);

        for (auto pos : positions_for_negative_velocities) {
            auto time_total = pos.first + 1;
            time_and_vertical_velocity.emplace_back(time_total, -initial_vel);
        }
    }
}

std::set<std::pair<int, int>> get_x_y_velocities(const std::vector<std::pair<std::size_t, int>> &time_and_vertical_velocity, int min_x, int max_x)
{
    std::set<std::pair<int, int>> v_x_y;
    for (const auto &[time, v_y] : time_and_vertical_velocity) {
        for (int v_x = max_x; v_x > 0; --v_x) {
            auto velocities = ranges::views::iota(0, v_x + 1) | ranges::views::reverse | ranges::views::take(time);
            auto pos_x = ranges::accumulate(velocities, 0);
            if (pos_x < min_x) break;
            if (pos_x > max_x) continue;
            v_x_y.emplace(v_x, v_y);
        }
    }

    return v_x_y;
}

int main()
{
    int min_x = 70, max_x = 96, min_y = 124, max_y = 179;
    std::vector<std::pair<std::size_t, int>> time_and_vertical_velocities;
    get_time_and_speed_y_positive_start(time_and_vertical_velocities, min_y, max_y);
    get_time_and_speed_y_negative_start(time_and_vertical_velocities, min_y, max_y);
    auto velocities = get_x_y_velocities(time_and_vertical_velocities, min_x, max_x);
    auto v_y_max = *ranges::max_element(velocities, {}, &std::pair<int, int>::second);
    auto v_y_max_view = ranges::views::iota(0, v_y_max.second);
    fmt::print("Part 1 answer is {}\n", ranges::accumulate(v_y_max_view, 0));
    fmt::print("Part 2 answer is {}\n", velocities.size());
}
