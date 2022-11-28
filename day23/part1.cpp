#include <array>
#include <boost/functional/hash.hpp>
#include <fmt/format.h>
#include <iostream>
#include <optional>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <unordered_map>
#include <utility>

enum class AmphipodTypes {
    A,
    B,
    C,
    D,
};

struct Amphipod
{
    int location = 0;
    AmphipodTypes type = AmphipodTypes::A;
    bool operator==(Amphipod const &other) const
    {
        return location == other.location && type == other.type;
    }
};

std::unordered_map<AmphipodTypes, int> energy_costs{ { AmphipodTypes::A, 1 },
    { AmphipodTypes::B, 10 },
    { AmphipodTypes::C, 100 },
    { AmphipodTypes::D, 1000 } };

using Amphipods = std::array<Amphipod, 8>;

constexpr std::array<int, 7> hall_positions{ 0, 1, 3, 5, 7, 9, 10 };

char type_to_char(AmphipodTypes type)
{
    switch (type) {
    case (AmphipodTypes::A):
        return 'A';
    case (AmphipodTypes::B):
        return 'B';
    case (AmphipodTypes::C):
        return 'C';
    case (AmphipodTypes::D):
        return 'D';
    }
    throw std::runtime_error{ "Unknown Type" };
}
char hall_print_helper(std::optional<AmphipodTypes> type)
{
    if (type.has_value()) {
        return type_to_char(type.value());
    }
    return '.';
}

char room_printer_helper(const Amphipods &amphs, int room_no)
{
    auto pos = ranges::find(amphs, room_no, &Amphipod::location);
    if (pos == ranges::end(amphs)) {
        return '.';
    } else {
        return type_to_char((*pos).type);
    }
}
void print(const Amphipods &amphs)
{
    /*
    #############
    #...........#
    ###B#C#B#D###
      #A#D#C#A#
      #########
    */
    std::array<std::optional<AmphipodTypes>, std::size(hall_positions)> hall_amphs;
    for (std::size_t i = 0; i < std::size(hall_amphs); ++i) {
        auto pos = ranges::find(amphs, hall_positions[i], &Amphipod::location);
        if (pos == ranges::end(amphs)) {
            hall_amphs[i] = {};
        } else {
            hall_amphs[i] = (*pos).type;
        }
    }


    fmt::print("-------------------------------\n");
    fmt::print("#############\n");
    fmt::print("#{}{}.{}.{}.{}.{}{}#\n", hall_print_helper(hall_amphs[0]), hall_print_helper(hall_amphs[1]), hall_print_helper(hall_amphs[2]), hall_print_helper(hall_amphs[3]), hall_print_helper(hall_amphs[4]), hall_print_helper(hall_amphs[5]), hall_print_helper(hall_amphs[6]));
    fmt::print("###{}#{}#{}#{}###\n", room_printer_helper(amphs, 21), room_printer_helper(amphs, 41), room_printer_helper(amphs, 61), room_printer_helper(amphs, 81));
    fmt::print("  #{}#{}#{}#{}#  \n", room_printer_helper(amphs, 20), room_printer_helper(amphs, 40), room_printer_helper(amphs, 60), room_printer_helper(amphs, 80));
    fmt::print("  #########  \n");
    std::cout << "-------------------------------\n";
    std::cout << std::flush;
}
class AmphipodsHash
{
public:
    std::size_t operator()(const Amphipods &amphs) const
    {
        std::size_t seed = 0;
        for (const auto &amp : amphs) {
            boost::hash_combine(seed, amp.type);
            boost::hash_combine(seed, amp.location);
        }


        return seed;
    }
};


class Puzzle
{
public:
    Puzzle(Amphipods amphipods) : amphipods_{ std::move(amphipods) } {}
    void solve()
    {
        next(amphipods_, 0);
        fmt::print("The answer is {}\n", lowest_cost);
    }
    int lowest_cost = 1342123123;

private:
    std::unordered_map<Amphipods, std::vector<std::pair<Amphipods, int>>, AmphipodsHash> cached;
    void next(const Amphipods &amphipods_info, int current_cost)
    {
        std::vector<std::pair<Amphipods, int>> next_amphipods_all;
        if (cached.find(amphipods_info) != cached.end()) {
            next_amphipods_all = cached[amphipods_info];
        } else {
            next_amphipods_all = get_next_layout(amphipods_info);
            cached[amphipods_info] = next_amphipods_all;
        }
        // auto next_amphipods_all = get_next_layout(amphipods_info);
        for (const auto &next_amphs : next_amphipods_all) {
            // print(next_amphs.first);

            int new_cost = current_cost + next_amphs.second;
            if (new_cost >= lowest_cost) continue;
            if (is_solved(next_amphs.first)) {
                if (new_cost < lowest_cost) {

                    lowest_cost = new_cost;
                }

            } else {
                next(next_amphs.first, new_cost);
            }
        }
    }
    bool in_hall(const Amphipod &amph)
    {
        return ranges::find(hall_positions, amph.location) != ranges::end(hall_positions);
    }
    std::vector<std::pair<Amphipods, int>> get_next_layout(const Amphipods &amphs)
    {
        std::vector<std::pair<Amphipods, int>> possibilities{};
        // maybe keep here another vector to have the
        // const auto &[amphs, current_cost] = amphipods;
        for (std::size_t i = 0; i < amphs.size(); ++i) {
            // if cached value exist don't go further steps here
            const auto &amphipod = amphs[i];
            if (!should_move(amphipod, amphs)) continue;
            auto new_amphipod = move_to_room(amphipod, amphs);
            if (new_amphipod.has_value()) {
                // print(amphs);
                auto new_amphs = amphs;
                new_amphs[i] = new_amphipod.value().first;
                possibilities.emplace_back(std::move(new_amphs), new_amphipod.value().second);
            }
            if (in_hall(amphipod)) continue;
            for (const auto &hall_pos : hall_positions) {
                if (hall_pos == amphipod.location) continue;
                if (!is_hall_blocked(amphipod.location, hall_pos, amphs)) {
                    auto new_amphs = amphs;
                    new_amphs[i] = { hall_pos, amphipod.type };
                    possibilities.emplace_back(new_amphs, get_cost(amphipod, hall_pos));
                }
            }
            // we should add the caching here
        }
        return possibilities;
    }

    std::pair<int, int> amphipod_type_to_room_locations(AmphipodTypes room_type)
    {
        switch (room_type) {
        case AmphipodTypes::A:
            return { 20, 21 };
        case AmphipodTypes::B:
            return { 40, 41 };
        case AmphipodTypes::C:
            return { 60, 61 };
        case AmphipodTypes::D:
            return { 80, 81 };
        }
        throw std::runtime_error{ "Unknown type" };
    }

    std::optional<int> get_available_room_location(const Amphipods &all_amphipods, AmphipodTypes room_type)
    {
        auto [first_room, second_room] = amphipod_type_to_room_locations(room_type);
        auto amphipod_at_first_room = ranges::find(all_amphipods, first_room, &Amphipod::location);
        if (amphipod_at_first_room == ranges::end(all_amphipods)) {
            return first_room;
        }
        if ((*amphipod_at_first_room).type == room_type) {
            auto amphipod_at_second_room = ranges::find(all_amphipods, second_room, &Amphipod::location);
            if (amphipod_at_second_room == ranges::end(all_amphipods)) {
                return second_room;
            }
        }
        return {};
    }

    bool is_hall_blocked(int current, int target, const Amphipods &all_amphipods)
    {
        if (current > 10) {
            if (ranges::find(all_amphipods, current + 1, &Amphipod::location) != ranges::end(all_amphipods)) return true;
            current = static_cast<int>(current / 10.0);
        }
        if (target > 10) target = static_cast<int>(target / 10.0);
        if (target > current) {
            // going right
            auto amph_on_the_way = ranges::find_if(all_amphipods, [&](const auto &amph) { return amph.location > current && amph.location <= target; });
            return amph_on_the_way != ranges::end(all_amphipods);
        }
        if (target < current) {
            // going left
            auto amph_on_the_way = ranges::find_if(all_amphipods, [&](const auto &amph) { return amph.location < current && amph.location >= target; });
            return amph_on_the_way != ranges::end(all_amphipods);
        }
        return false;
    }

    int get_cost(Amphipod amphipod, int target)
    {
        auto [current_location, type] = amphipod;
        int additional_steps = 0;
        if (current_location > 10) {
            auto remainder = current_location % 10;
            current_location = static_cast<int>(current_location / 10.0);
            if (remainder == 0) additional_steps += 2;
            if (remainder == 1) additional_steps += 1;
        }
        if (target > 10) {
            auto remainder = target % 10;
            target = static_cast<int>(target / 10.0);
            if (remainder == 0) additional_steps += 2;
            if (remainder == 1) additional_steps += 1;
        }
        return energy_costs[type] * (std::abs(target - current_location) + additional_steps);
    }

    std::optional<std::pair<Amphipod, int>>
      move_to_room(
        const Amphipod &amphipod,
        const Amphipods &all_amphipods)
    {
        auto target_room = get_available_room_location(all_amphipods, amphipod.type);
        if (target_room.has_value() && !is_hall_blocked(amphipod.location, target_room.value(), all_amphipods)) {
            return std::pair{ Amphipod{ target_room.value(), amphipod.type }, get_cost(amphipod, target_room.value()) };
        }
        return std::nullopt;
    }

    bool should_move(const Amphipod &amphipod, const Amphipods &all_amphipods)
    {
        switch (amphipod.type) {
        case AmphipodTypes::A: {
            if (amphipod.location == 20) return false;
            if (amphipod.location == 21) {
                auto first_room = ranges::find_if(
                  all_amphipods,
                  [](const auto &a) { return a.location == 20; });
                if (first_room == ranges::end(all_amphipods)) return true;
                if ((*first_room).type == AmphipodTypes::A) return false;
                return true;
            }
            return true;
        }
        case AmphipodTypes::B: {
            if (amphipod.location == 40) return false;
            if (amphipod.location == 41) {
                auto first_room = ranges::find_if(
                  all_amphipods,
                  [](const auto &a) { return a.location == 40; });
                if (first_room == ranges::end(all_amphipods)) return true;
                if ((*first_room).type == AmphipodTypes::B) return false;
                return true;
            }
            return true;
        }
        case AmphipodTypes::C: {
            if (amphipod.location == 60) return false;
            if (amphipod.location == 61) {
                auto first_room = ranges::find_if(
                  all_amphipods,
                  [](const auto &a) { return a.location == 60; });
                if (first_room == ranges::end(all_amphipods)) return true;
                if ((*first_room).type == AmphipodTypes::C) return false;
                return true;
            }
            return true;
        }
        case AmphipodTypes::D: {
            if (amphipod.location == 80) return false;
            if (amphipod.location == 81) {
                auto first_room = ranges::find_if(
                  all_amphipods,
                  [](const auto &a) { return a.location == 80; });
                if (first_room == ranges::end(all_amphipods)) return true;
                if ((*first_room).type == AmphipodTypes::D) return false;
                return true;
            }
            return true;
        }
        }
        return false;
    }
    bool is_solved(const Amphipods &amphs)
    {
        return ranges::all_of(amphs, [](const auto &amph) {
            switch (amph.type) {
            case AmphipodTypes::A:
                return amph.location == 20 || amph.location == 21;
            case AmphipodTypes::B:
                return amph.location == 40 || amph.location == 41;
            case AmphipodTypes::C:
                return amph.location == 60 || amph.location == 61;
            case AmphipodTypes::D:
                return amph.location == 80 || amph.location == 81;
            default:
                throw std::runtime_error{ "Unexpected Type" };
            }
        });
    }
    Amphipods amphipods_;
};
auto main() -> int
{
    /*
    #############
    #...........#
    ###B#C#B#D###
      #A#D#C#A#
      #########
    */
    // Amphipods initial_layout{
    //     { Amphipod{ 20, AmphipodTypes::A },
    //       Amphipod{ 21, AmphipodTypes::B },
    //       Amphipod{ 40, AmphipodTypes::D },
    //       Amphipod{ 41, AmphipodTypes::C },
    //       Amphipod{ 60, AmphipodTypes::C },
    //       Amphipod{ 61, AmphipodTypes::B },
    //       Amphipod{ 80, AmphipodTypes::A },
    //       Amphipod{ 81, AmphipodTypes::D } }
    // };
    /*
    #############
    #...........#
    ###C#B#A#D###
      #B#C#D#A#
      #########
    */
    Amphipods initial_layout{
        { Amphipod{ 20, AmphipodTypes::B },
          Amphipod{ 21, AmphipodTypes::C },
          Amphipod{ 40, AmphipodTypes::C },
          Amphipod{ 41, AmphipodTypes::B },
          Amphipod{ 60, AmphipodTypes::D },
          Amphipod{ 61, AmphipodTypes::A },
          Amphipod{ 80, AmphipodTypes::A },
          Amphipod{ 81, AmphipodTypes::D } }
    };
    Puzzle puzzle(initial_layout);
    puzzle.solve();
}