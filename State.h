#pragma once

constexpr int BOARD_COLUMNS = 9;
constexpr int BOARD_ROWS = 9;
constexpr int BOX_COLUMNS = 3;
constexpr int BOX_ROWS = 3;

struct State
{
    int board[BOARD_ROWS][BOARD_COLUMNS] {
        {7, 8, 0, 4, 0, 0, 1, 2, 0},
        {6, 0, 0, 0, 7, 5, 0, 0, 9},
        {0, 0, 0, 6, 0, 1, 0, 7, 8},
        {0, 0, 7, 0, 4, 0, 2, 6, 0},
        {0, 0, 1, 0, 5, 0, 9, 3, 0},
        {9, 0, 4, 0, 6, 0, 0, 0, 5},
        {0, 7, 0, 3, 0, 0, 0, 1, 2},
        {1, 2, 0, 0, 0, 7, 4, 0, 0},
        {0, 4, 9, 2, 0, 6, 0, 0, 7}
    };

    static void swap(State next);
    static State copy_current();
};
