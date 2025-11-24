#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Board = uint64_t;

constexpr Board RANK_ATTACK_MASK = 0xFF;
constexpr Board FILE_ATTACK_MASK = 0x0101010101010101;

constexpr Board DL_ATTACK_MASK = 0x8040201008040201;
constexpr Board DR_ATTACK_MASK = 0x0102040810204080;

constexpr Board DL_ATTACK_MASKS[15] = {
    DL_ATTACK_MASK << 56, DL_ATTACK_MASK << 48, DL_ATTACK_MASK << 40,
    DL_ATTACK_MASK << 32, DL_ATTACK_MASK << 24, DL_ATTACK_MASK << 16,
    DL_ATTACK_MASK << 8,  DL_ATTACK_MASK,       DL_ATTACK_MASK >> 8,
    DL_ATTACK_MASK >> 16, DL_ATTACK_MASK >> 24, DL_ATTACK_MASK >> 32,
    DL_ATTACK_MASK >> 40, DL_ATTACK_MASK >> 48, DL_ATTACK_MASK >> 56,
};

constexpr Board DR_ATTACK_MASKS[15] = {
    DR_ATTACK_MASK << 56, DR_ATTACK_MASK << 48, DR_ATTACK_MASK << 40,
    DR_ATTACK_MASK << 32, DR_ATTACK_MASK << 24, DR_ATTACK_MASK << 16,
    DR_ATTACK_MASK << 8,  DR_ATTACK_MASK,       DR_ATTACK_MASK >> 8,
    DR_ATTACK_MASK >> 16, DR_ATTACK_MASK >> 24, DR_ATTACK_MASK >> 32,
    DR_ATTACK_MASK >> 40, DR_ATTACK_MASK >> 48, DR_ATTACK_MASK >> 56,
};

constexpr Board gen_attack_mask(uint8_t pos) {
    assert(pos < 64);

    uint8_t rank = pos >> 3;
    uint8_t file = pos & 7;

    Board rank_board = RANK_ATTACK_MASK << (rank << 3);
    Board file_board = FILE_ATTACK_MASK << file;
    Board left_diagonal_board = DL_ATTACK_MASKS[7 - rank + file];
    Board right_diagonal_board = DR_ATTACK_MASKS[14 - (rank + file)];

    return rank_board | file_board | left_diagonal_board | right_diagonal_board;
}

template <size_t N = 64, size_t... I>
constexpr std::array<Board, N> gen_attack_masks_impl(
    std::index_sequence<I...>) {
    return {static_cast<Board>(gen_attack_mask(I))...};
}

constexpr std::array<Board, 64> gen_attack_masks() {
    return gen_attack_masks_impl(std::make_index_sequence<64>{});
}

constexpr std::array<Board, 64> ATTACK_MASKS = gen_attack_masks();

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

Board gen_attack_mask_global(Board board) {
    Board mask{0};

    for (uint8_t rank_idx = 0; rank_idx < 8; ++rank_idx) {
        Board rank = (board >> (rank_idx << 3)) & 0xFF;

        if (rank) {
            for (uint8_t file_idx = 0; file_idx < 8; ++file_idx) {
                if (rank & ((Board)1 << file_idx)) {
                    mask |= gen_attack_mask((rank_idx << 3) + file_idx);
                }
            }
        }
    }

    return mask;
}

int main() {
    auto time_start = std::chrono::high_resolution_clock::now();

    std::vector<Board> current_boards;
    std::vector<Board> next_boards;

    current_boards.reserve(5000);
    next_boards.reserve(5000);

    current_boards.push_back(0);

    for (uint8_t rank = 0; rank < 8; ++rank) {
        next_boards.clear();

        for (const Board board : current_boards) {
            Board attack_mask = gen_attack_mask_global(board);

            for (uint8_t file = 0; file < 8; ++file) {
                uint8_t pos = (rank * 8) + file;
                Board bit = (Board)1 << pos;

                if (!((bool)(bit & attack_mask))) {
                    next_boards.push_back(board | bit);
                }
            }
        }

        std::swap(current_boards, next_boards);
    }

    auto time_end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                      time_end - time_start)
                      .count();

    for (const Board board : current_boards) {
        std::cout << board_format(board);
    }

    std::cout << "Execution time (ns) -> " << time << "\n";
    std::cout << "Number of possible boards -> " << current_boards.size()
              << "\n";
    return 0;
}
