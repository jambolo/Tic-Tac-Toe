#include "gtest/gtest.h"

#include "TicTacToe/TicTacToeEvaluator.h"
#include "TicTacToe/TicTacToeState.h"

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

        // A winning state for the first player (X) should have a value of firstPlayerWins()
        {
            Board boardWinningX = Board({ {
                Board::Cell::X, Board::Cell::X, Board::Cell::X,
                Board::Cell::NEITHER, Board::Cell::O, Board::Cell::NEITHER,
                Board::Cell::O, Board::Cell::NEITHER, Board::Cell::NEITHER
            } });
            TicTacToeState stateWinningX(boardWinningX, GamePlayer::GameState::PlayerId::SECOND);
            EXPECT_EQ(evaluator.evaluate(stateWinningX), evaluator.firstPlayerWins());
        }

        // A winning state for the second player (O) should have a value of secondPlayerWins()
        {
            Board boardWinningO = Board({ {
                Board::Cell::O, Board::Cell::O, Board::Cell::O,
                Board::Cell::NEITHER, Board::Cell::X, Board::Cell::NEITHER,
                Board::Cell::X, Board::Cell::NEITHER, Board::Cell::X
            } });
            TicTacToeState stateWinningO(boardWinningO, GamePlayer::GameState::PlayerId::FIRST);
            EXPECT_EQ(evaluator.evaluate(stateWinningO), evaluator.secondPlayerWins());
        }

        // The score of a board with only single Xs (though it's not valid) should be 0.
        {
            Board board1X = Board({ {
                Board::Cell::X, Board::Cell::NEITHER, Board::Cell::NEITHER,
                Board::Cell::NEITHER, Board::Cell::NEITHER, Board::Cell::X,
                Board::Cell::NEITHER, Board::Cell::X, Board::Cell::NEITHER
            } });
            TicTacToeState state1X(board1X, GamePlayer::GameState::PlayerId::SECOND);
            EXPECT_EQ(evaluator.evaluate(state1X), 0.0f);
        }

        // The score of a board with only single Os (though it's not valid) should be 0.
        {
            Board board1O = Board({ {
                Board::Cell::O, Board::Cell::NEITHER, Board::Cell::NEITHER,
                Board::Cell::NEITHER, Board::Cell::NEITHER, Board::Cell::O,
                Board::Cell::NEITHER, Board::Cell::O, Board::Cell::NEITHER
            } });
            TicTacToeState state1O(board1O, GamePlayer::GameState::PlayerId::FIRST);
            EXPECT_EQ(evaluator.evaluate(state1O), 0.0f);
        }

        {
            // The score of a board with only single Xs and Os should be 0.
            Board board1XO = Board({ {
                Board::Cell::O, Board::Cell::X, Board::Cell::NEITHER,
                Board::Cell::X, Board::Cell::NEITHER, Board::Cell::NEITHER,
                Board::Cell::NEITHER, Board::Cell::O, Board::Cell::X
            } });
            TicTacToeState state1XO(board1XO, GamePlayer::GameState::PlayerId::SECOND);
            EXPECT_EQ(evaluator.evaluate(state1XO), 0.0f);
        }

        // The score of a board with only 2 Xs should be the number of 2 Xs.
        {
            Board board2X = Board({ {
                Board::Cell::X, Board::Cell::X, Board::Cell::NEITHER,
                Board::Cell::X, Board::Cell::X, Board::Cell::NEITHER,
                Board::Cell::NEITHER, Board::Cell::NEITHER, Board::Cell::O
            } });
            TicTacToeState state2X(board2X, GamePlayer::GameState::PlayerId::SECOND);
            EXPECT_EQ(evaluator.evaluate(state2X), 4.0f);
        }

        // The score of a board with only 2 Os should be the negative of the number of 2 Os.
        {
            Board board2O = Board({ {
                Board::Cell::X, Board::Cell::NEITHER, Board::Cell::NEITHER,
                Board::Cell::NEITHER, Board::Cell::O, Board::Cell::O,
                Board::Cell::NEITHER, Board::Cell::O, Board::Cell::O
            } });
            TicTacToeState state2O(board2O, GamePlayer::GameState::PlayerId::FIRST);
            EXPECT_EQ(evaluator.evaluate(state2O), -4.0f);
        }

        // The score of a board with both 2 Xs and 2 Os should be the difference of the number of 2Xs and 2Os
        {
            Board board2XO = Board({ {
                Board::Cell::X, Board::Cell::X, Board::Cell::NEITHER,
                Board::Cell::X, Board::Cell::X, Board::Cell::NEITHER,
                Board::Cell::NEITHER, Board::Cell::O, Board::Cell::O
            } });
            TicTacToeState state2XO(board2XO, GamePlayer::GameState::PlayerId::SECOND);
            EXPECT_EQ(evaluator.evaluate(state2XO), 2.0f);
        }
    }

    TEST(TicTacToeEvaluator, FirstPlayerWins)
    {
        // The first player should have a large positive score (>= 100 * 100) for winning
        EXPECT_GE(TicTacToeEvaluator().firstPlayerWins(), 10000.0f);
    }

    TEST(TicTacToeEvaluator, SecondPlayerWins)
    {
        // The second player (O) should have a large negative score (<= -100 * 100) for winning
        EXPECT_LE(TicTacToeEvaluator().secondPlayerWins(), -10000.0f);
    }

} // namespace TicTacToe
