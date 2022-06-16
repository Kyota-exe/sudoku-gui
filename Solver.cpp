#include "Solver.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "State.h"

struct Position { int row, column; };
enum class Command { None, Start, Terminate };

static State worker_state;
static std::mutex mtx;
static std::condition_variable cv;
static std::atomic<Command> cmd;
static std::atomic<int> delay_ms;

bool found_empty(Position& buffer)
{
    for (int row = 0; row < BOARD_ROWS; row++)
    {
        for (int column = 0; column < BOARD_COLUMNS; column++)
        {
            if (worker_state.board[row][column] == 0)
            {
                buffer = {row, column};
                return true;
            }
        }
    }

    return false;
}

bool is_valid(int number, Position position)
{
    // Check row
    for (int column = 0; column < BOARD_COLUMNS; column++)
    {
        if (position.column == column) continue;
        if (worker_state.board[position.row][column] == number)
        {
            return false;
        }
    }

    // Check column
    for (int row = 0; row < BOARD_ROWS; row++)
    {
        if (position.row == row) continue;
        if (worker_state.board[row][position.column] == number)
        {
            return false;
        }
    }

    // Check box
    int box_row = position.row - (position.row % BOX_ROWS);
    int box_column = position.column - (position.column % BOX_COLUMNS);
    for (int row = box_row; row < box_row + BOX_ROWS; row++)
    {
        for (int column = box_column; column < box_column + BOX_COLUMNS; column++)
        {
            if ((row != position.row && column != position.column) && worker_state.board[row][column] == number)
            {
                return false;
            }
        }
    }

    return true;
}

void do_delay()
{
    auto duration = std::chrono::milliseconds(delay_ms);
    std::this_thread::sleep_for(duration);
}

bool solve()
{
    Position empty_slot_pos {};
    if (!found_empty(empty_slot_pos))
    {
        return true;
    }

    for (int number = 1; number <= 9; number++)
    {
        if (is_valid(number, empty_slot_pos))
        {
            do_delay();
            worker_state.board[empty_slot_pos.row][empty_slot_pos.column] = number;
            State::swap(worker_state);
            if (solve()) return true;
            do_delay();
            worker_state.board[empty_slot_pos.row][empty_slot_pos.column] = 0;
            State::swap(worker_state);
        }
    }

    return false;
}

void Solver::main()
{
    std::unique_lock<std::mutex> lock(mtx);
    if (cmd == Command::None) cv.wait(lock);

    switch (cmd)
    {
        case Command::Start:
            if (!solve()) throw std::runtime_error("Solution could not be found.");
            break;
        case Command::Terminate:
            return;
        default:
            throw std::runtime_error("Invalid command sent to worker thread.");
    }
}

void Solver::set_delay(int ms)
{
    delay_ms = ms;
}

int Solver::get_delay()
{
    return delay_ms;
}

// Must be called from main thread
void Solver::start()
{
    cmd = Command::Start;
    cv.notify_all();
}
