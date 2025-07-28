#pragma once

#include "GamePlayer/StaticEvaluator.h"

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

    // Returns the value of a winning state for the first player. Overrides StaticEvaluator::firstPlayerWins().
    virtual float firstPlayerWins() const override { return X_WIN_VALUE; }

    // Returns the value of a winning state for the second player. Overrides StaticEvaluator::secondPlayerWins().
    virtual float secondPlayerWins() const override { return O_WIN_VALUE; }

private:
    // Value constants for evaluation
    static float constexpr X_WIN_VALUE = 10000.0f;
    static float constexpr O_WIN_VALUE = -10000.0f;
    static float constexpr CENTER_BONUS = 5.0f;
    static float constexpr CORNER_BONUS = 1.0f;
    static float constexpr TWO_IN_LINE_BONUS = 100.0f;
};
