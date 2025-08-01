#include "ZHash.h"

#include "Components/Board.h"
#include "GamePlayer/GameState.h"

#include <cassert>
#include <random>

ZHash::ZValueTable const ZHash::zValueTable_;

ZHash::ZHash(Board const & board, GamePlayer::GameState::PlayerId currentPlayer, bool over, Board::Cell winner)
    : value_(ZHash::EMPTY)
{
    // Initialize the hash with the board state
    for (int i = 0; i < 9; ++i)
    {
        move(board.at(i), i);
    }

    // Add the current player
    if (currentPlayer != GamePlayer::GameState::PlayerId::FIRST)
        turn();

    // If the game is over, add the done status and winner
    if (over)
    {
        done(winner);
    }
}

ZHash & ZHash::move(Board::Cell cell, int index)
{
    value_ ^= zValueTable_.cellValue(cell, index);
    return *this;
}

ZHash & ZHash::turn()
{
    value_ ^= zValueTable_.turnValue();
    return *this;
}

ZHash & ZHash::done(Board::Cell winner)
{
    value_ ^= zValueTable_.winnerValue(winner);
    return *this;
}

ZHash::ZValueTable::ZValueTable()
{
    std::mt19937_64 rng;
    static_assert(sizeof (std::mt19937_64::result_type) == 8, "The random number generator must generate 64 bits.");
    // Note: We don't seed the generator. This means that the same values are generated each time the program is run.

    // Generate cell values
    for (int i = 0; i < 9; ++i)
    {
        cellValues_[i][static_cast<int>(Board::Cell::NEITHER)] = 0;     // Empty cells don't contribute to the hash
        cellValues_[i][static_cast<int>(Board::Cell::X)]       = rng();
        cellValues_[i][static_cast<int>(Board::Cell::O)]       = rng();
    }

    // Generate turn value
    turnValue_ = rng();

    // Generate winner values
    winnerValues_[static_cast<int>(Board::Cell::NEITHER)] = rng();
    winnerValues_[static_cast<int>(Board::Cell::X)]       = rng();
    winnerValues_[static_cast<int>(Board::Cell::O)]       = rng();
}
