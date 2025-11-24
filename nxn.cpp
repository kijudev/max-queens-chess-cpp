#include <bitset>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

using BoardSize = size_t;
constexpr BoardSize BOARD_SIZE = 10;
using Board = std::bitset<BOARD_SIZE * BOARD_SIZE>;

constexpr Board gen_r_mask_idx(BoardSize idx) {
    Board board{};
    BoardSize offset = idx % BOARD_SIZE;

    for (BoardSize i = 0; i < BOARD_SIZE; ++i) {
        board.set(i + offset);
    }

    return board;
}

constexpr Board gen_f_mask_idx(BoardSize idx) {
    Board board{};

    for (BoardSize i = 0; i < BOARD_SIZE; ++i) {
        board.set((i * BOARD_SIZE) + idx);
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
    board.set(4);

    std::cout << board_format(board);
    std::cout << board_format(gen_r_mask_idx(4));
    std::cout << board_format(gen_f_mask_idx(4));

    return 0;
}
