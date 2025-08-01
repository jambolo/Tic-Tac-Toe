#pragma once

#include "Components/Player.h"
#include "TicTacToeState/TicTacToeState.h"

namespace GamePlayer
{
class GameTree;
class StaticEvaluator;
class TranspositionTable;
}

class ComputerPlayer : public Player
{
public:
    // Constructor
    explicit ComputerPlayer(TicTacToeState::PlayerId playerId);

    // Gets a move from the computer and applies it to the game state. Overrides Player::move().
    virtual void move(TicTacToeState * pState) override;

private:

    GamePlayer::GameTree * gameTree_;                                    // Game tree for searching responses
    std::shared_ptr<GamePlayer::StaticEvaluator>    staticEvaluator_;    // Static evaluator for the game tree
    std::shared_ptr<GamePlayer::TranspositionTable> transpositionTable_; // Transposition table for the game tree

    std::vector<GamePlayer::GameState *> responseGenerator(GamePlayer::GameState const & state, int depth);
};
