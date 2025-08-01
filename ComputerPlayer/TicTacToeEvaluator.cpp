#include "TicTacToeEvaluator.h"

#include "TicTacToeState/TicTacToeState.h"

#include <array>
#include <cassert>
#include <vector>

typedef std::array<int, 3> Line;

static Line allLines[] {
    { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 }, // Rows
    { 0, 3, 6 }, { 1, 4, 7 }, { 2, 5, 8 }, // Columns
    { 0, 4, 8 }, { 2, 4, 6 }               // Diagonals
};

float TicTacToeEvaluator::evaluate(GamePlayer::GameState const & state) const
{
    // Check if the state is a TicTacToeState
    auto const &  tttState = dynamic_cast<TicTacToeState const &>(state);

    Board const & board    = tttState.board();

    // If there are 3 Xs in a row, return the X win value
    for (auto const & line : allLines)
    {
        if (board.at(line[0]) == Board::Cell::X &&
            board.at(line[1]) == Board::Cell::X &&
            board.at(line[2]) == Board::Cell::X)
        {
            return X_WIN_VALUE;
        }
        if (board.at(line[0]) == Board::Cell::O &&
            board.at(line[1]) == Board::Cell::O &&
            board.at(line[2]) == Board::Cell::O)
        {
            return O_WIN_VALUE;
        }
    }

    // If the game is a draw, return 0
    if (tttState.isDraw())
    {
        return 0.0f;
    }

    // Evaluate the score based on the current board state

    float score = 0.0f;

    // Check rows, columns, and diagonals for two Xs or Os and one NEITHER in a line
    for (auto const & line : allLines)
    {
        int xCount = 0, oCount = 0, emptyCount = 0;
        for (int idx : line)
        {
            switch (board.at(idx))
            {
            case Board::Cell::X: ++xCount; break;
            case Board::Cell::O: ++oCount; break;
            case Board::Cell::NEITHER: ++emptyCount; break;
            }
        }
        if (xCount == 2 && emptyCount == 1)
        {
            score += TWO_IN_LINE_BONUS;
        }
        else if (oCount == 2 && emptyCount == 1)
        {
            score -= TWO_IN_LINE_BONUS;
        }
    }

    // Check for center bonus
    if (board.at(4) == Board::Cell::X)
    {
        score += CENTER_BONUS;
    }
    else if (board.at(4) == Board::Cell::O)
    {
        score -= CENTER_BONUS;
    }

    // Check for corner bonuses
    for (int corner : { 0, 2, 6, 8 })
    {
        if (board.at(corner) == Board::Cell::X)
        {
            score += CORNER_BONUS;
        }
        else if (board.at(corner) == Board::Cell::O)
        {
            score -= CORNER_BONUS;
        }
    }

    return score;
}
