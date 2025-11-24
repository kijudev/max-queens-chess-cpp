#include <bitset>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

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
    Board board{};
    board.set(14);

    std::cout << board_format(board);
    std::cout << board_format(gen_attack_mask_idx(14));

    return 0;
}
