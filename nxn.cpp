#include <bitset>
#include <cassert>
#include <cstdint>
#include <string>

using BoardSize = uint16_t;
constexpr BoardSize BOARD_SIZE = 8;
using Board = std::bitset<BOARD_SIZE>;

constexpr Board calc_rank_mask() {
    Board board{};

    for (BoardSize i = 0; i < BOARD_SIZE; ++i) {
        board.set(i);
    }

    return board;
}

constexpr Board calc_file_mask() {
    Board board{};

    for (BoardSize i = 0; i < BOARD_SIZE; ++i) {
        board |= (Board)1 << (i * BOARD_SIZE);
    }

    return board;
}

constexpr Board calc_diagonal_left_mask() {
    Board board{};

    for (BoardSize i = 0; i < BOARD_SIZE; ++i) {
        board |= (Board)1 << (i * BOARD_SIZE);
    }

    return board;
}

constexpr Board calc_diagonal_right_mask() {
    Board board{};

    for (BoardSize i = 0; i < BOARD_SIZE; ++i) {
        board |= (Board)1 << (i * BOARD_SIZE);
    }

    return board;
}

constexpr Board RANK_MASK = calc_rank_mask();
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

int main() { return 0; }
