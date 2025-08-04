#include "gtest/gtest.h"

#include "ComputerPlayer/TicTacToeEvaluator.h"
#include "TicTacToeState/TicTacToeState.h"

// These values are copied from the TicTacToeEvaluator.h file
static float constexpr CENTER_BONUS      = 5.0f;
static float constexpr CORNER_BONUS      = 1.0f;
static float constexpr TWO_IN_LINE_BONUS = 100.0f;

namespace TicTacToe
{
TEST(TicTacToeEvaluator, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    ASSERT_NO_THROW(TicTacToeEvaluator());
}

TEST(TicTacToeEvaluator, Evaluate)
{
    TicTacToeEvaluator evaluator;

    // The starting state should have a score of 0
    {
        TicTacToeState initialState;
        EXPECT_EQ(evaluator.evaluate(initialState), 0.0f);
    }

    // A winning state for Alice (X) should have a value of aliceWinsValue() regardless of the values of the other marks
    {
        Board boardWinningX = Board({{
                                        Board::Cell::X, Board::Cell::X, Board::Cell::X,
                                        Board::Cell::NEITHER, Board::Cell::O, Board::Cell::NEITHER,
                                        Board::Cell::O, Board::Cell::NEITHER, Board::Cell::NEITHER
                                    }});
        TicTacToeState stateWinningX(boardWinningX, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(evaluator.evaluate(stateWinningX), evaluator.aliceWinsValue());
    }

    // A winning state for Bob (O) should have a value of bobWinsValue() regardless of the values of the other
    // marks
    {
        Board boardWinningO = Board({{
                                        Board::Cell::O, Board::Cell::O, Board::Cell::O,
                                        Board::Cell::NEITHER, Board::Cell::X, Board::Cell::NEITHER,
                                        Board::Cell::X, Board::Cell::NEITHER, Board::Cell::X
                                    }});
        TicTacToeState stateWinningO(boardWinningO, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(evaluator.evaluate(stateWinningO), evaluator.bobWinsValue());
    }

    // The score of a board with only single Xs not in the center or corner should be 0.
    {
        Board board1X = Board({{
                                  Board::Cell::NEITHER, Board::Cell::NEITHER, Board::Cell::NEITHER,
                                  Board::Cell::NEITHER, Board::Cell::NEITHER, Board::Cell::X,
                                  Board::Cell::NEITHER, Board::Cell::X,       Board::Cell::NEITHER
                              }});
        TicTacToeState state1X(board1X, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(evaluator.evaluate(state1X), 0.0f);
    }

    // The score of a board with only single Os not in the center or corner should be 0.
    {
        Board board1O = Board({{
                                  Board::Cell::NEITHER, Board::Cell::O, Board::Cell::NEITHER,
                                  Board::Cell::O, Board::Cell::NEITHER, Board::Cell::NEITHER,
                                  Board::Cell::NEITHER, Board::Cell::NEITHER, Board::Cell::NEITHER
                              }});
        TicTacToeState state1O(board1O, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(evaluator.evaluate(state1O), 0.0f);
    }

    {
        // The score of a board with only single Xs and Os not in the center or corner should be 0.
        Board board1XO = Board({{
                                   Board::Cell::NEITHER, Board::Cell::X, Board::Cell::NEITHER,
                                   Board::Cell::X, Board::Cell::NEITHER, Board::Cell::NEITHER,
                                   Board::Cell::NEITHER, Board::Cell::O, Board::Cell::NEITHER
                               }});
        TicTacToeState state1XO(board1XO, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(evaluator.evaluate(state1XO), 0.0f);
    }

    {
        Board board2X = Board({{
                                  Board::Cell::X, Board::Cell::X, Board::Cell::NEITHER,
                                  Board::Cell::X, Board::Cell::X, Board::Cell::O,
                                  Board::Cell::NEITHER, Board::Cell::NEITHER, Board::Cell::NEITHER
                              }});
        TicTacToeState state2X(board2X, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(evaluator.evaluate(state2X), 4 * TWO_IN_LINE_BONUS + 1 * CENTER_BONUS + 1 * CORNER_BONUS);
    }

    {
        Board board2O = Board({{
                                  Board::Cell::NEITHER, Board::Cell::X, Board::Cell::NEITHER,
                                  Board::Cell::NEITHER, Board::Cell::O, Board::Cell::O,
                                  Board::Cell::NEITHER, Board::Cell::O, Board::Cell::O
                              }});
        TicTacToeState state2O(board2O, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(evaluator.evaluate(state2O), -4 * TWO_IN_LINE_BONUS - 1 * CENTER_BONUS - 1 * CORNER_BONUS);
    }

    // The score of a board with both 2 Xs and 2 Os should be the difference of the number of 2Xs and 2Os
    {
        Board board2XO = Board({{
                                   Board::Cell::X, Board::Cell::X, Board::Cell::NEITHER,
                                   Board::Cell::X, Board::Cell::X, Board::Cell::NEITHER,
                                   Board::Cell::NEITHER, Board::Cell::O, Board::Cell::O
                               }});
        TicTacToeState state2XO(board2XO, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(evaluator.evaluate(state2XO), (3 - 1) * TWO_IN_LINE_BONUS + 1 * CENTER_BONUS + (1 - 1) * CORNER_BONUS);
    }
}

TEST(TicTacToeEvaluator, AliceWins)
{
    // Alice should have a large positive score (>= 100 * 100) for winning
    EXPECT_GE(TicTacToeEvaluator().aliceWinsValue(), 10000.0f);
}

TEST(TicTacToeEvaluator, BobWins)
{
    // The bob (O) should have a large negative score (<= -100 * 100) for winning
    EXPECT_LE(TicTacToeEvaluator().bobWinsValue(), -10000.0f);
}
} // namespace TicTacToe
