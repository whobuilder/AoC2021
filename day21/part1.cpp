#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/stride.hpp>
#include <range/v3/view/transform.hpp>
int main()
{
    auto deterministic_die = ranges::views::cycle(ranges::views::iota(1, 101));
    auto grouped = deterministic_die | ranges::views::chunk(3) | ranges::views::transform([](const auto &rolls) { return ranges::accumulate(rolls, 0ULL); });
    auto player1_rolls = grouped | ranges::views::stride(2);
    auto player2_rolls = grouped | ranges::views::drop(1) | ranges::views::stride(2);

    long i = 0;
    std::size_t player1_score = 0;
    std::size_t player2_score = 0;
    std::size_t player1_pos = 4;
    std::size_t player2_pos = 2;
    bool player1_won = false;
    std::size_t rolled_die_count = 0;

    while (true) {
        player1_pos = ((player1_pos + player1_rolls[i] - 1) % 10) + 1;
        rolled_die_count += 3;
        player1_score += player1_pos;
        if (player1_score >= 1000) {
            player1_won = true;
            break;
        }
        player2_pos = ((player2_pos + player2_rolls[i] - 1) % 10) + 1;
        rolled_die_count += 3;
        player2_score += player2_pos;
        if (player2_score >= 1000) break;
        ++i;
    }
    fmt::print("Player 1 score: {}\n", player1_score);
    fmt::print("Player 2 score: {}\n", player2_score);
    fmt::print("Dice rolled {} times\n", rolled_die_count);
    if (player1_won) {
        fmt::print("The answer is {}\n", player2_score * rolled_die_count);
    } else {
        fmt::print("The answer is {}\n", player1_score * rolled_die_count);
    }
}
