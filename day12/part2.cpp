#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/istream_range.hpp>
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
  std::unordered_set<std::string> visited_small_caves,
  std::pair<std::string, int> exception_cave_occurence = {})
{
    auto next_caves = cave_connections.equal_range(current_cave);
    ranges::for_each(ranges::subrange(next_caves.first, next_caves.second), [&](const auto &pair) {
        const auto &[current, next] = pair;
        // if (exception_cave_occurence.first == "") {
        //     exception_cave_occurence.first = next;
        // }
        if (exception_cave_occurence.first == next) {
            ++exception_cave_occurence.second;
        }
        bool is_path_ended = false;
        if (next == "end" || (exception_cave_occurence.first != next && visited_small_caves.count(next)) || (exception_cave_occurence.first == next && exception_cave_occurence.second > 2)) {
            is_path_ended = true;
        }
        path.push_back(next);
        if (is_small_case(next) && !visited_small_caves.count(next)) {
            visited_small_caves.insert(next);
        }
        if (!is_path_ended) {
            next_cave(cave_connections, next, path, paths, visited_small_caves, exception_cave_occurence);
        }
        if (next == "end") {
            if (ranges::find(paths, path) == ranges::end(paths)) {
                paths.push_back(path);
                // fmt::print("{}\n", path);
            }
        }
        path.pop_back();
        if (ranges::find(path, next) == ranges::end(path))
            visited_small_caves.erase(next);
        if (next == exception_cave_occurence.first)
            --exception_cave_occurence.second;
    });
}

auto depth_first_search(const cave_connections_t &cave_connections, std::string start_node, std::unordered_set<std::string> small_caves)
{
    std::vector<std::vector<std::string>> paths;
    for (const auto &small_cave : small_caves)
        next_cave(cave_connections, start_node, { start_node }, paths, {}, { small_cave, 0 });
    return paths;
}

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day12/day12.txt" };
    cave_connections_t cave_connections;
    std::unordered_set<std::string> small_caves;
    for (const auto &cave : ranges::istream_view<std::string>(input)) {
        auto f = ranges::views::split(cave, '-') | ranges::view::transform([](auto &&rng) {
            return std::string(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
        });
        if (is_small_case(*ranges::begin(f)) && *ranges::begin(f) != "start" && *ranges::begin(f) != "end")
            small_caves.insert(*ranges::begin(f));
        if (is_small_case(*ranges::next(ranges::begin(f))) && *ranges::next(ranges::begin(f)) != "start" && *ranges::next(ranges::begin(f)) != "end")
            small_caves.insert(*ranges::next(ranges::begin(f)));
        if (*ranges::begin(f) != "end" && *ranges::next(ranges::begin(f)) != "start")
            cave_connections.insert({ *ranges::begin(f), *ranges::next(ranges::begin(f)) });
        if (*ranges::begin(f) != "start" && *ranges::next(ranges::begin(f)) != "end")
            cave_connections.insert({ *ranges::next(ranges::begin(f)), *ranges::begin(f) });
    }
    auto paths = depth_first_search(cave_connections, "start", small_caves);
    fmt::print("The answer is {}\n", paths.size());
}
