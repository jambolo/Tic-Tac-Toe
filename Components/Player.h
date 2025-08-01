#pragma once

#include "TicTacToeState/TicTacToeState.h"

// Abstract base class for players in a tic-tac-toe game.
class Player
{
public:
    explicit Player(TicTacToeState::PlayerId playerId)
        : playerId_(playerId)
    {
    }

    virtual ~Player() = default;

    // Make a move on the given game state. This method must be overridden.
    virtual void move(TicTacToeState * pState) = 0;

    // Get the player's ID.
    TicTacToeState::PlayerId playerId() const { return playerId_; }

protected:
    TicTacToeState::PlayerId playerId_;    // The player's ID
};
