#include <fmt/format.h>
#include <fstream>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/istream.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool is_small_case(std::string_view str)
{
    return ranges::all_of(str, [](const auto &c) { return std::islower(c); });
}

using cave_connections_t = std::unordered_multimap<std::string, std::string>;

void next_cave(const cave_connections_t &cave_connections,
  const std::string &current_cave,
  std::vector<std::string> path,
  std::vector<std::vector<std::string>> &paths,
  std::unordered_set<std::string> visited_small_caves)
{
    auto next_caves = cave_connections.equal_range(current_cave);
    ranges::for_each(ranges::subrange(next_caves.first, next_caves.second), [&](const auto &pair) {
        const auto &[current, next] = pair;
        bool is_path_ended = false;
        if (next == "end" || visited_small_caves.count(next)) {
            is_path_ended = true;
        }
        path.push_back(next);
        if (is_small_case(next) && !visited_small_caves.count(next)) {
            visited_small_caves.insert(next);
        }
        if (!is_path_ended) {
            next_cave(cave_connections, next, path, paths, visited_small_caves);
        }
        if (next == "end") {
            paths.push_back(path);
        }
        path.pop_back();
        if (ranges::find(path, next) == ranges::end(path))
            visited_small_caves.erase(next);
    });
}

auto depth_first_search(const cave_connections_t &cave_connections, std::string start_node)
{
    std::vector<std::vector<std::string>> paths;
    next_cave(cave_connections, start_node, { start_node }, paths, {});
    return paths;
}

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day12/day12.txt" };
    cave_connections_t cave_connections;
    for (const auto &cave : ranges::istream_view<std::string>(input)) {
        auto f = ranges::views::split(cave, '-') | ranges::views::transform([](auto &&rng) {
            return std::string(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
        });
        if (*ranges::begin(f) != "end" && *ranges::next(ranges::begin(f)) != "start")
            cave_connections.insert({ *ranges::begin(f), *ranges::next(ranges::begin(f)) });
        if (*ranges::begin(f) != "start" && *ranges::next(ranges::begin(f)) != "end")
            cave_connections.insert({ *ranges::next(ranges::begin(f)), *ranges::begin(f) });
    }
    auto paths = depth_first_search(cave_connections, "start");
    fmt::print("The answer is {}\n", paths.size());
}
