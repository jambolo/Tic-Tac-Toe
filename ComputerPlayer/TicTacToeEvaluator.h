#pragma once

#include "GamePlayer/StaticEvaluator.h"

namespace GamePlayer
{
class GameState;
}

// A static evaluation function for tic-tac-toe.
class TicTacToeEvaluator : public GamePlayer::StaticEvaluator
{
public:
    // Constructor.
    TicTacToeEvaluator() = default;

    // Destructor.
    virtual ~TicTacToeEvaluator() = default;

    // Returns a value for the given tic-tac-toe state. Overrides StaticEvaluator::evaluate().
    virtual float evaluate(GamePlayer::GameState const & state) const override;

    // Returns the value of a winning state for Alice. Overrides StaticEvaluator::aliceWinsValue().
    virtual float aliceWinsValue() const override { return WIN_VALUE; }

    // Returns the value of a winning state for Bob. Overrides StaticEvaluator::bobWinsValue().
    virtual float bobWinsValue() const override { return -WIN_VALUE; }

private:
    // Value constants for evaluation
    static float constexpr WIN_VALUE         = 10000.0f;
    static float constexpr CENTER_BONUS      = 5.0f;
    static float constexpr CORNER_BONUS      = 1.0f;
    static float constexpr TWO_IN_LINE_BONUS = 100.0f;
};
