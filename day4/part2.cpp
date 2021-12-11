#include <charconv>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <fstream>
#include <optional>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/common.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/for_each.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <vector>

int main()
{
    std::ifstream input{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day4/day4.txt" };
    auto is_view = ranges::istream_view<std::string>(input);

    auto picked_numbers = ranges::split_view(*(ranges::begin(is_view)), ',')
                          | ranges::view::transform([](auto &&rng) {
                                // https://stackoverflow.com/a/48403210/15697391
                                return std::string_view(&*rng.begin(), static_cast<long unsigned>(ranges::distance(rng)));
                            })
                          | ranges::view::transform([](auto &&sv) {
                                int value=0;
                                std::from_chars(sv.data(),sv.data()+sv.size(),value);
                                return value; });
    fmt::print("Numbers: {}\n", picked_numbers);

    auto board_numbers = is_view | ranges::view::drop(1) | ranges::view::transform([](auto &&v) { return std::stoi(v); }) | ranges::to<std::vector<std::optional<int>>>;
    auto boards = board_numbers | ranges::view::chunk(25);
    auto number_of_boards = ranges::size(boards);
    //TODO: get rid of this vector and use the board directly to check the winning condition
    std::vector<std::array<std::array<std::optional<int>, 5>, 10>> found_rows_and_columns(number_of_boards);

    std::vector<unsigned long> finished_boards;
    bool stop = false;
    for (auto number : picked_numbers) {
        if (stop) break;
        auto enumerated_boards = ranges::views::enumerate(boards);
        for (auto &&[idx, board] : enumerated_boards) {
            if (ranges::find(finished_boards, idx) != ranges::end(finished_boards)) {
                continue;
            }
            auto it = ranges::find(board, number);
            if (it == ranges::end(board)) continue;
            auto board_position = ranges::distance(ranges::begin(board), it);
            board[board_position].reset();
            auto &rows_and_columns = found_rows_and_columns[idx];
            auto row_no = static_cast<long unsigned>(board_position / 5);
            auto column_no = static_cast<long unsigned>(board_position % 5);
            rows_and_columns[row_no][column_no] = number;
            rows_and_columns[5 + column_no][row_no] = number;
            if (ranges::any_of(rows_and_columns, [](auto &&values) { return ranges::all_of(values, [](auto &&v) { return v.has_value(); }); })) {
                finished_boards.push_back(idx);
                if (ranges::size(finished_boards) == number_of_boards) {
                    auto not_marked_values = board | ranges::views::filter([](auto &&v) { return v.has_value(); }) | ranges::views::transform([](auto &&v) { return v.value(); });
                    auto unmarked_points = ranges::accumulate(not_marked_values, 0);
                    fmt::print("The answer is {}\n", unmarked_points * number);
                    stop = true;
                    break;
                }
            }
        }
    }
}
