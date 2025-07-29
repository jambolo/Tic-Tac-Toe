#pragma once

#include "Player.h"

class HumanPlayer : public Player
{
public:
    // Constructor
    explicit HumanPlayer(GamePlayer::GameState::PlayerId playerId);

    // Gets a move from the human and applies it to the game state. Overrides Player::move().
    virtual void move(TicTacToeState * pState) override;

private:
    // Validate if a move is legal
    bool isValidMove(TicTacToeState const & state, int row, int col) const;

    // Clear the input stream after invalid input
    void clearInputStream() const;
};
