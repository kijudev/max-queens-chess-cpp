#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using Board = uint64_t;

constexpr Board RANK_MASK = 0xFF;
constexpr Board FILE_MASK = 0x0101010101010101;

constexpr Board DIAGONAL_LEFT_MASK = 0x8040201008040201;
constexpr Board DIAGONAL_RIGHT_MASK = 0x0102040810204080;

const Board DIAGONAL_LEFT_MASKS[15] = {
    DIAGONAL_LEFT_MASK << 56, DIAGONAL_LEFT_MASK << 48,
    DIAGONAL_LEFT_MASK << 40, DIAGONAL_LEFT_MASK << 32,
    DIAGONAL_LEFT_MASK << 24, DIAGONAL_LEFT_MASK << 16,
    DIAGONAL_LEFT_MASK << 8,  DIAGONAL_LEFT_MASK,
    DIAGONAL_LEFT_MASK >> 8,  DIAGONAL_LEFT_MASK >> 16,
    DIAGONAL_LEFT_MASK >> 24, DIAGONAL_LEFT_MASK >> 32,
    DIAGONAL_LEFT_MASK >> 40, DIAGONAL_LEFT_MASK >> 48,
    DIAGONAL_LEFT_MASK >> 56,
};

const Board DIAGONAL_RIGHT_MASKS[15] = {
    DIAGONAL_RIGHT_MASK << 56, DIAGONAL_RIGHT_MASK << 48,
    DIAGONAL_RIGHT_MASK << 40, DIAGONAL_RIGHT_MASK << 32,
    DIAGONAL_RIGHT_MASK << 24, DIAGONAL_RIGHT_MASK << 16,
    DIAGONAL_RIGHT_MASK << 8,  DIAGONAL_RIGHT_MASK,
    DIAGONAL_RIGHT_MASK >> 8,  DIAGONAL_RIGHT_MASK >> 16,
    DIAGONAL_RIGHT_MASK >> 24, DIAGONAL_RIGHT_MASK >> 32,
    DIAGONAL_RIGHT_MASK >> 40, DIAGONAL_RIGHT_MASK >> 48,
    DIAGONAL_RIGHT_MASK >> 56,
};

struct GameState {
    Board attack;
    Board placement;
};

std::string board_format(Board board) {
    std::string fmt = "   0 1 2 3 4 5 6 7\n   ----------------";

    for (uint8_t i = 0; i < 8; ++i) {
        fmt += "\n" + std::to_string(i) + "|";

        for (uint8_t j = 0; j < 8; ++j) {
            fmt += " ";
            fmt += (board >> ((8 * i) + j)) & 1 ? "#" : ".";
        }
    }

    return fmt + "\n";
}

Board board_calc_attacks(uint8_t pos) {
    assert(pos < 64);

    uint8_t rank = pos >> 3;
    uint8_t file = pos & 7;

    Board rank_board = RANK_MASK << (rank << 3);
    Board file_board = FILE_MASK << file;
    Board left_diagonal_board = DIAGONAL_LEFT_MASKS[7 - rank + file];
    Board right_diagonal_board = DIAGONAL_RIGHT_MASKS[14 - (rank + file)];

    return rank_board | file_board | left_diagonal_board | right_diagonal_board;
}

int main() {
    auto time_start = std::chrono::high_resolution_clock::now();

    std::vector<GameState> current_states;
    std::vector<GameState> next_states;

    current_states.reserve(5000);
    next_states.reserve(5000);

    current_states.push_back({0, 0});

    for (uint8_t rank = 0; rank < 8; ++rank) {
        next_states.clear();

        for (const GameState state : current_states) {
            for (uint8_t file = 0; file < 8; ++file) {
                uint8_t pos = (rank * 8) + file;
                Board bit = (Board)1 << pos;

                if (!(state.attack & bit)) {
                    next_states.push_back(
                        {state.attack | board_calc_attacks(pos),
                         state.placement | bit});
                }
            }
        }

        std::swap(current_states, next_states);
    }

    auto time_end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                      time_end - time_start)
                      .count();

    for (const GameState state : current_states) {
        std::cout << board_format(state.placement);
    }

    std::cout << "Execution time (ns) -> " << time << "\n";
    std::cout << "Number of possible boards -> " << current_states.size()
              << "\n";
    return 0;
}
