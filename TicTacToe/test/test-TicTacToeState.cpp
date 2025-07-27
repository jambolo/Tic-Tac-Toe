#include "gtest/gtest.h"

#include "TicTacToe/TicTacToeState.h"
#include "TicTacToe/Board.h"
#include "TicTacToe/ZHash.h"

#include <algorithm>

namespace TicTacToe
{
    // Useful boards
    Board board0;
    Board boardNotDone({ { // X to move
        Board::Cell::X,       Board::Cell::O,       Board::Cell::O,
        Board::Cell::NEITHER, Board::Cell::X,       Board::Cell::NEITHER,
        Board::Cell::X,       Board::Cell::NEITHER, Board::Cell::O
    } });

    Board boardXWins({ { // O to move
        Board::Cell::X,       Board::Cell::O,       Board::Cell::O,
        Board::Cell::X,       Board::Cell::X,       Board::Cell::NEITHER,
        Board::Cell::X,       Board::Cell::NEITHER, Board::Cell::O
    } });
    Board boardOWins({ { // X to move
        Board::Cell::O,       Board::Cell::X,       Board::Cell::X,
        Board::Cell::O,       Board::Cell::NEITHER, Board::Cell::NEITHER,
        Board::Cell::O,       Board::Cell::NEITHER, Board::Cell::X
    } });
    Board boardDraw({ { // O to move
        Board::Cell::O, Board::Cell::O, Board::Cell::X,
        Board::Cell::X, Board::Cell::X, Board::Cell::O,
        Board::Cell::O, Board::Cell::X, Board::Cell::X
    } });
    Board boardDrawNext({ { // X to move
        Board::Cell::O, Board::Cell::O, Board::Cell::X,
        Board::Cell::X, Board::Cell::X, Board::Cell::O,
        Board::Cell::O, Board::Cell::X, Board::Cell::NEITHER
    } });


    void ValidateState(TicTacToeState const& state,
                       Board const& board,
                       GamePlayer::GameState::PlayerId who,
                       bool done,
                       bool draw,
                       Board::Cell winner)
    {
        // Check that the board is correct
        EXPECT_EQ(state.board().value(), board.value());

        // Check that the current player is set to FIRST
        EXPECT_EQ(state.whoseTurn(), who);

        // Check that the game is not done
        EXPECT_EQ(state.isDone(), done);

        // Check that the game is not a draw
        EXPECT_EQ(state.isDraw(), draw);

        // Check that the winner is NEITHER
        EXPECT_EQ(state.winner(), winner);
    }

TEST(TicTacToeState, Constuctor_default)
{
    ASSERT_NO_THROW(TicTacToeState());

    TicTacToeState state;
    ValidateState(state, Board(), GamePlayer::GameState::PlayerId::FIRST, false, false, Board::Cell::NEITHER);

    // Check that the fingerprint is EMPTY
    EXPECT_EQ(state.fingerprint(), ZHash().value());
}


TEST(TicTacToeState, Constructor_board_currentPlayer)
{
    ASSERT_NO_THROW(TicTacToeState(boardDraw, GamePlayer::GameState::PlayerId::SECOND));

    {
        ASSERT_NO_THROW(TicTacToeState(board0, GamePlayer::GameState::PlayerId::FIRST));
        TicTacToeState state(board0, GamePlayer::GameState::PlayerId::FIRST);
        ValidateState(state, board0, GamePlayer::GameState::PlayerId::FIRST, false, false, Board::Cell::NEITHER);
    }
    {
        ASSERT_NO_THROW(TicTacToeState(boardNotDone, GamePlayer::GameState::PlayerId::FIRST));
        TicTacToeState state(boardNotDone, GamePlayer::GameState::PlayerId::FIRST);
        ValidateState(state, boardNotDone, GamePlayer::GameState::PlayerId::FIRST, false, false, Board::Cell::NEITHER);
    }
    {
        ASSERT_NO_THROW(TicTacToeState(boardXWins, GamePlayer::GameState::PlayerId::SECOND));
        TicTacToeState state(boardXWins, GamePlayer::GameState::PlayerId::SECOND);
        ValidateState(state, boardXWins, GamePlayer::GameState::PlayerId::SECOND, true, false, Board::Cell::X);
    }
    {
        ASSERT_NO_THROW(TicTacToeState(boardOWins, GamePlayer::GameState::PlayerId::FIRST));
        TicTacToeState state(boardOWins, GamePlayer::GameState::PlayerId::FIRST);
        ValidateState(state, boardOWins, GamePlayer::GameState::PlayerId::FIRST, true, false, Board::Cell::O);
    }
    {
        ASSERT_NO_THROW(TicTacToeState(boardDraw, GamePlayer::GameState::PlayerId::SECOND));
        TicTacToeState state(boardDraw, GamePlayer::GameState::PlayerId::SECOND);
        ValidateState(state, boardDraw, GamePlayer::GameState::PlayerId::SECOND, true, true, Board::Cell::NEITHER);
    }
}

TEST(TicTacToeState, Fingerprint)
{
    // Test that the fingerprint is calculated correctly for various states
    TicTacToeState state;
    ZHash expected = ZHash();
    EXPECT_EQ(state.fingerprint(), expected.value());

    // X makes a move marking (1, 1)
    state.move(1, 1);
    expected.move(Board::Cell::X, Board::toIndex(1, 1)); // Simulate the move
    expected.turn(); // Change turn to O
    EXPECT_EQ(state.fingerprint(), expected.value());

    // O makes a move marking (0, 1)
    state.move(0, 1);
    expected.move(Board::Cell::O, Board::toIndex(0, 1)); // Simulate the move
    expected.turn(); // Change turn to X
    EXPECT_EQ(state.fingerprint(), expected.value());

    // X makes a move marking (0, 0)
    state.move(0, 0);
    expected.move(Board::Cell::X, Board::toIndex(0, 0)); // Simulate the move
    expected.turn(); // Change turn to O
    EXPECT_EQ(state.fingerprint(), expected.value());

    // O makes a move marking (2, 2)
    state.move(2, 2);
    expected.move(Board::Cell::O, Board::toIndex(2, 2)); // Simulate the move
    expected.turn(); // Change turn to X
    EXPECT_EQ(state.fingerprint(), expected.value());

    // X makes a move marking (2, 0)
    state.move(2, 0);
    expected.move(Board::Cell::X, Board::toIndex(2, 0)); // Simulate the move
    expected.turn(); // Change turn to O
    EXPECT_EQ(state.fingerprint(), expected.value());

    // O makes a move marking (1, 0)
    state.move(1, 0);
    expected.move(Board::Cell::O, Board::toIndex(1, 0)); // Simulate the move
    expected.turn(); // Change turn to O
    EXPECT_EQ(state.fingerprint(), expected.value());

    // X makes a move marking (0, 2), winning the game
    state.move(0, 2);
    expected.move(Board::Cell::X, Board::toIndex(0, 2)); // Simulate the move
    expected.turn(); // Change turn to O
    expected.done(Board::Cell::X); // Mark the game as done with X as the winner
    EXPECT_EQ(state.fingerprint(), expected.value());
}

TEST(TicTacToeState, WhoseTurn)
{
    {
        TicTacToeState state;
        EXPECT_EQ(state.whoseTurn(), GamePlayer::GameState::PlayerId::FIRST);

        // X makes a move marking (1, 1)
        state.move(1, 1);
        EXPECT_EQ(state.whoseTurn(), GamePlayer::GameState::PlayerId::SECOND);
    }
    {
        TicTacToeState state(Board(), GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_EQ(state.whoseTurn(), GamePlayer::GameState::PlayerId::SECOND);

        // O makes a move marking (1, 1)
        state.move(1, 1);
        EXPECT_EQ(state.whoseTurn(), GamePlayer::GameState::PlayerId::FIRST);
    }
}

TEST(TicTacToeState, Move)
{
    {
        Board board;
        TicTacToeState state;
        ValidateState(state, board, GamePlayer::GameState::PlayerId::FIRST, false, false, Board::Cell::NEITHER);

        // X makes a move marking (1, 1)
        state.move(1, 1);
        board.set(Board::toIndex(1, 1), Board::Cell::X);
        ValidateState(state, board, GamePlayer::GameState::PlayerId::SECOND, false, false, Board::Cell::NEITHER);

        // O makes a move marking (0, 1)
        state.move(0, 1);
        board.set(Board::toIndex(0, 1), Board::Cell::O);
        ValidateState(state, board, GamePlayer::GameState::PlayerId::FIRST, false, false, Board::Cell::NEITHER);

        // X makes a move marking (0, 0)
        state.move(0, 0);
        board.set(Board::toIndex(0, 0), Board::Cell::X);
        ValidateState(state, board, GamePlayer::GameState::PlayerId::SECOND, false, false, Board::Cell::NEITHER);

        // O makes a move marking (2, 2)
        state.move(2, 2);
        board.set(Board::toIndex(2, 2), Board::Cell::O);
        ValidateState(state, board, GamePlayer::GameState::PlayerId::FIRST, false, false, Board::Cell::NEITHER);

        // X makes a move marking (2, 0)
        state.move(2, 0);
        board.set(Board::toIndex(2, 0), Board::Cell::X);
        ValidateState(state, board, GamePlayer::GameState::PlayerId::SECOND, false, false, Board::Cell::NEITHER);

        // O makes a move marking (1, 0)
        state.move(1, 0);
        board.set(Board::toIndex(1, 0), Board::Cell::O);
        ValidateState(state, board, GamePlayer::GameState::PlayerId::FIRST, false, false, Board::Cell::NEITHER);

        // X makes a move marking (0, 2), winning the game
        state.move(0, 2);
        board.set(Board::toIndex(0, 2), Board::Cell::X);
    }
    {
        TicTacToeState state(boardDrawNext, GamePlayer::GameState::PlayerId::FIRST);
        ValidateState(state, boardDrawNext, GamePlayer::GameState::PlayerId::FIRST, false, false, Board::Cell::NEITHER);

        // X makes a move marking (2, 2) and its a draw
        state.move(2, 2);
        boardDraw.set(Board::toIndex(2, 2), Board::Cell::X);
        ValidateState(state, boardDraw, GamePlayer::GameState::PlayerId::SECOND, true, true, Board::Cell::NEITHER);
    }
}

TEST(TicTacToeState, IsDone)
{
    {
        TicTacToeState state(board0, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_FALSE(state.isDone());
    }
    {
        TicTacToeState state(boardNotDone, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_FALSE(state.isDone());
    }
    {
        TicTacToeState state(boardXWins, GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_TRUE(state.isDone());
    }
    {
        TicTacToeState state(boardOWins, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_TRUE(state.isDone());
    }
    {
        TicTacToeState state(boardDraw, GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_TRUE(state.isDone());
    }
}

TEST(TicTacToeState, IsDraw)
{
    {
        TicTacToeState state(board0, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_FALSE(state.isDraw());
    }
    {
        TicTacToeState state(boardNotDone, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_FALSE(state.isDraw());
    }
    {
        TicTacToeState state(boardXWins, GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_FALSE(state.isDraw());
    }
    {
        TicTacToeState state(boardOWins, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_FALSE(state.isDraw());
    }
    {
        TicTacToeState state(boardDraw, GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_TRUE(state.isDraw());
    }
}

TEST(TicTacToeState, Winner)
{
    {
        TicTacToeState state(board0, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_EQ(state.winner(), Board::Cell::NEITHER);
    }
    {
        TicTacToeState state(boardNotDone, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_EQ(state.winner(), Board::Cell::NEITHER);
    }
    {
        TicTacToeState state(boardXWins, GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_EQ(state.winner(), Board::Cell::X);
    }
    {
        TicTacToeState state(boardOWins, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_EQ(state.winner(), Board::Cell::O);
    }
    {
        TicTacToeState state(boardDraw, GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_EQ(state.winner(), Board::Cell::NEITHER);
    }
}

TEST(TicTacToeState, Board)
{
    {
        TicTacToeState state(board0, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_EQ(state.board().value(), board0.value());
    }
    {
        TicTacToeState state(boardNotDone, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_EQ(state.board().value(), boardNotDone.value());
    }
    {
        TicTacToeState state(boardXWins, GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_EQ(state.board().value(), boardXWins.value());
    }
    {
        TicTacToeState state(boardOWins, GamePlayer::GameState::PlayerId::FIRST);
        EXPECT_EQ(state.board().value(), boardOWins.value());
    }
    {
        TicTacToeState state(boardDraw, GamePlayer::GameState::PlayerId::SECOND);
        EXPECT_EQ(state.board().value(), boardDraw.value());
    }
}

TEST(TicTacToeState, ToCell)
{
    EXPECT_EQ(TicTacToeState::toCell(GamePlayer::GameState::PlayerId::FIRST), Board::Cell::X);
    EXPECT_EQ(TicTacToeState::toCell(GamePlayer::GameState::PlayerId::SECOND), Board::Cell::O);
}

TEST(TicTacToeState, ToPlayerId)
{
    EXPECT_TRUE(TicTacToeState::toPlayerId(Board::Cell::X).has_value());
    EXPECT_TRUE(TicTacToeState::toPlayerId(Board::Cell::O).has_value());
    EXPECT_FALSE(TicTacToeState::toPlayerId(Board::Cell::NEITHER).has_value());
    EXPECT_EQ(TicTacToeState::toPlayerId(Board::Cell::X).value(), GamePlayer::GameState::PlayerId::FIRST);
    EXPECT_EQ(TicTacToeState::toPlayerId(Board::Cell::O).value(), GamePlayer::GameState::PlayerId::SECOND);
}

} // namespace TicTacToe