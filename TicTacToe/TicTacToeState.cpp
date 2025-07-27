#include "TicTacToeState.h"

#include "Board.h"
#include "ZHash.h"

#include <array>
#include <algorithm>
#include <cassert>

typedef std::array<int, 3> Line;
typedef std::vector<Line> LineList;
typedef std::array<LineList, 9> LineListArray;

 // Lines for each cell in the tic-tac-toe board}
static LineListArray linesFrom{{
   LineList{{{0, 1, 2}, {0, 4, 8}, {0, 3, 6}}},             // 0
   LineList{{{0, 1, 2}, {1, 4, 7}}},                        // 1
   LineList{{{0, 1, 2}, {2, 4, 6}, {2, 5, 8}}},             // 2
   LineList{{{3, 4, 5}, {0, 3, 6}}},                        // 3
   LineList{{{3, 4, 5}, {0, 4, 8}, {1, 4, 7}, {2, 4, 6}}},  // 4
   LineList{{{3, 4, 5}, {2, 5, 8}}},                        // 5
   LineList{{{6, 7, 8}, {0, 3, 6}, {2, 4, 6}}},             // 6
   LineList{{{6, 7, 8}, {1, 4, 7}}},                        // 7
   LineList{{{6, 7, 8}, {0, 4, 8}, {2, 5, 8}}},             // 8
}};

TicTacToeState::TicTacToeState()
    : board_()
    , currentPlayer_(PlayerId::FIRST)
    , done_(false)
    , winner_(Board::Cell::NEITHER)
{
}

TicTacToeState::TicTacToeState(Board const & board, PlayerId currentPlayer)
    : board_(board)
    , currentPlayer_(currentPlayer)
    , done_(false)
    , winner_(Board::Cell::NEITHER)
    , zhash_(board, currentPlayer)
{
    // Initialize done_ and winner_ based on the board state, and update the hash accordingly
    checkIfDone();
}

uint64_t TicTacToeState::fingerprint() const
{
    return zhash_.value();
}

void TicTacToeState::move(int row, int column)
{
    // Sanity check - the cell should be empty
    assert(board_.at(row, column) == Board::Cell::NEITHER);

    // The current status is assumed to be not done with no winner
    assert(!done_);
    assert(winner_ == Board::Cell::NEITHER);

    // Set the cell for the current player
    Board::Cell xo = toCell(currentPlayer_);
    board_.set(row, column, xo);
    zhash_.move(xo, Board::toIndex(row, column));

    // Check for win or draw
    checkIfDone();

    // Switch to the next player
    currentPlayer_ = (currentPlayer_ == PlayerId::FIRST) ? PlayerId::SECOND : PlayerId::FIRST;
    zhash_.turn();
}

void TicTacToeState::checkIfDone()
{
    if (done_)
    {
        // If the game is already done, no need to check again
        return;
    }

    assert(winner_ == Board::Cell::NEITHER);

    // Check for a win
    for (int i = 0; i < 9; ++i)
    {
        auto const & lines = linesFrom[i];
        for (auto const & line : lines)
        {
            if (board_.at(line[0]) != Board::Cell::NEITHER &&
                board_.at(line[0]) == board_.at(line[1]) &&
                board_.at(line[1]) == board_.at(line[2]))
            {
                done_ = true;
                winner_ = board_.at(line[0]);
                zhash_.done(winner_);
                return;
            }
        }
    }

    // Check for a draw. If no cells are NEITHER, then it's a draw.
    for (int i = 0; i < 9; ++i)
    {
        if (board_.at(i) == Board::Cell::NEITHER)
        {
            return; // There is at least one empty cell, so the game is not done
        }
    }
    // If we reach here, it means all cells are filled and no winner was found, so it's a draw.
    // Set the game as done and leave the winner as NEITHER.
    done_ = true;
    zhash_.done(winner_);
}
