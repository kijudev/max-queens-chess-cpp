#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

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

    for (BoardSize pos = 0; pos < BOARD_SIZE * BOARD_SIZE; ++pos) {
        fmt += board.test(pos) ? "# " : ". ";

        if (pos % BOARD_SIZE == BOARD_SIZE - 1) {
            fmt += "\n";
        }
    }

    return fmt + "\n";
}

int main() {
    auto time_start = std::chrono::high_resolution_clock::now();

    std::vector<Board> current_boards;
    std::vector<Board> next_boards;

    current_boards.reserve(BOARD_SIZE * BOARD_SIZE * BOARD_SIZE);
    next_boards.reserve(BOARD_SIZE * BOARD_SIZE * BOARD_SIZE);

    for (BoardSize pos = 0; pos < BOARD_SIZE; ++pos) {
        current_boards.push_back({});
        current_boards[current_boards.size() - 1].set(pos);
    }

    for (BoardSize rank = 1; rank < BOARD_SIZE; ++rank) {
        next_boards.clear();

        for (const auto& board : current_boards) {
            for (BoardSize file = 0; file < BOARD_SIZE; ++file) {
                BoardSize pos = (rank * BOARD_SIZE) + file;
                Board result_mask = ATTACK_MASKS[pos] & board;

                if (!result_mask.any()) {
                    next_boards.push_back(board);
                    next_boards[next_boards.size() - 1].set(pos);
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
