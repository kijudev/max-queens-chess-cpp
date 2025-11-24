#include <array>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

using BoardSize = size_t;
constexpr BoardSize BOARD_SIZE = 10;
using Board = std::bitset<BOARD_SIZE * BOARD_SIZE>;

constexpr Board gen_attack_mask_idx(BoardSize idx) {
    Board board{};
    BoardSize rank = idx / BOARD_SIZE;
    BoardSize file = idx % BOARD_SIZE;

    // Rank mask, file mask
    for (BoardSize i = 0; i < BOARD_SIZE; ++i) {
        board.set(i + (rank * BOARD_SIZE));
        board.set((i * BOARD_SIZE) + file);
    }

    // Diagonal left mask
    for (BoardSize i = 0; (rank + i) < BOARD_SIZE && (file + i) < BOARD_SIZE;
         ++i) {
        board.set(((rank + i) * BOARD_SIZE) + (file + i));
    }

    for (BoardSize i = 0; (rank - i + 1) > 0 && (file - i + 1) > 0; ++i) {
        board.set(((rank - i) * BOARD_SIZE) + (file - i));
    }

    // Diagonal right mask
    for (BoardSize i = 0; (rank - i + 1) > 0 && (file + i) < BOARD_SIZE; ++i) {
        board.set(((rank - i) * BOARD_SIZE) + (file + i));
    }

    for (BoardSize i = 0; (rank + i) < BOARD_SIZE && (file - i + 1) > 0; ++i) {
        board.set(((rank + i) * BOARD_SIZE) + (file - i));
    }

    return board;
}

template <BoardSize... I>
constexpr std::array<Board, BOARD_SIZE * BOARD_SIZE> gen_attack_masks_impl(
    std::index_sequence<I...>) {
    return {gen_attack_mask_idx(I)...};
}

constexpr std::array<Board, BOARD_SIZE * BOARD_SIZE> gen_attack_masks() {
    return gen_attack_masks_impl(
        std::make_index_sequence<BOARD_SIZE * BOARD_SIZE>{});
}

constexpr std::array<Board, BOARD_SIZE * BOARD_SIZE> ATTACK_MASKS =
    gen_attack_masks();

std::string board_format(Board board) {
    std::string fmt{};

    for (BoardSize i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        fmt += board.test(i) ? "# " : ". ";

        if (i % BOARD_SIZE == BOARD_SIZE - 1) {
            fmt += "\n";
        }
    }

    return fmt + "\n";
}

int main() {
    for (BoardSize i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
        std::cout << board_format(ATTACK_MASKS[i]);
    }

    return 0;
}
