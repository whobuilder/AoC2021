#include <algorithm>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/unique.hpp>
#include <utility>
#include <vector>

struct game_state
{
    int player1_pos;
    int player1_score;
    int player2_pos;
    int player2_score;
    bool game_finished;
    std::size_t number_of_universes;
};

struct winners_number
{
    std::size_t player1;
    std::size_t player2;
};

int get_position(int current, int steps)
{
    return ((current + steps - 1) % 10) + 1;
}

game_state advance(game_state current_state, std::pair<int, std::size_t> roll, bool player1_playing)
{
    auto &[steps, freq] = roll;
    if (player1_playing) {
        current_state.player1_pos = get_position(current_state.player1_pos, steps);
        current_state.player1_score += current_state.player1_pos;
    } else {
        current_state.player2_pos = get_position(current_state.player2_pos, steps);
        current_state.player2_score += current_state.player2_pos;
    }
    current_state.number_of_universes *= freq;
    return current_state;
}

std::vector<game_state> roll_dice(const std::vector<game_state> &universes, bool player1_rolling)
{
    std::array<std::pair<int, std::size_t>, 7> rolls{
        std::pair<int, std::size_t>{ 3, 1 },
        std::pair<int, std::size_t>{ 4, 3 },
        std::pair<int, std::size_t>{ 5, 6 },
        std::pair<int, std::size_t>{ 6, 7 },
        std::pair<int, std::size_t>{ 7, 6 },
        std::pair<int, std::size_t>{ 8, 3 },
        std::pair<int, std::size_t>{ 9, 1 }
    };
    std::vector<game_state> universes_next;
    for (const auto &s : universes) {
        for (auto r : rolls) {
            universes_next.push_back(advance(s, r, player1_rolling));
        }
    }
    return universes_next;
}
void check_scores(std::vector<game_state> &universes, winners_number &board)
{
    int winning_score = 21;
    for (auto &s : universes) {
        if (s.player1_score >= winning_score) {
            s.game_finished = true;
            board.player1 += s.number_of_universes;
        } else if (s.player2_score >= winning_score) {
            s.game_finished = true;
            board.player2 += s.number_of_universes;
        }
    }
    auto start = std::remove_if(universes.begin(), universes.end(), [](const auto &s) { return s.game_finished; });

    universes.erase(start, universes.end());
}


int main()
{
    auto s = game_state{ 4, 0, 2, 0, false, 1 };
    std::vector<game_state> universes;
    winners_number score_board{ 0, 0 };
    universes.push_back(s);
    bool player1_is_playing = true;
    while (!universes.empty()) {
        universes = roll_dice(universes, player1_is_playing);
        check_scores(universes, score_board);
        player1_is_playing = !player1_is_playing;
    }

    fmt::print("player1 wins {}\n", score_board.player1);
    fmt::print("player2 wins {}\n", score_board.player2);
    fmt::print("The answer is {}\n", std::max(score_board.player1, score_board.player2));
}
