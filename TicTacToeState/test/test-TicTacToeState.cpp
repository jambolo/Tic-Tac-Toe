#include "gtest/gtest.h"

#include "Components/Board.h"
#include "TicTacToeState/TicTacToeState.h"
#include "TicTacToeState/ZHash.h"

#include <algorithm>

namespace TicTacToe
{
// Useful boards
Board board0;
Board boardNotDone({{      // X to move
                       Board::Cell::X,       Board::Cell::O,       Board::Cell::O,
                       Board::Cell::NEITHER, Board::Cell::X,       Board::Cell::NEITHER,
                       Board::Cell::X,       Board::Cell::NEITHER, Board::Cell::O
                   }});

Board boardXWins({{      // O to move
                     Board::Cell::X,       Board::Cell::O,       Board::Cell::O,
                     Board::Cell::X,       Board::Cell::X,       Board::Cell::NEITHER,
                     Board::Cell::X,       Board::Cell::NEITHER, Board::Cell::O
                 }});
Board boardOWins({{      // X to move
                     Board::Cell::O,       Board::Cell::X,       Board::Cell::X,
                     Board::Cell::O,       Board::Cell::NEITHER, Board::Cell::NEITHER,
                     Board::Cell::O,       Board::Cell::NEITHER, Board::Cell::X
                 }});
Board boardDraw({{      // O to move
                    Board::Cell::O, Board::Cell::O, Board::Cell::X,
                    Board::Cell::X, Board::Cell::X, Board::Cell::O,
                    Board::Cell::O, Board::Cell::X, Board::Cell::X
                }});
Board boardDrawNext({{      // X to move
                        Board::Cell::O, Board::Cell::O, Board::Cell::X,
                        Board::Cell::X, Board::Cell::X, Board::Cell::O,
                        Board::Cell::O, Board::Cell::X, Board::Cell::NEITHER
                    }});

void ValidateState(TicTacToeState const &   state,
                   Board const &            board,
                   TicTacToeState::PlayerId who,
                   bool                     done,
                   bool                     draw,
                   Board::Cell              winner)
{
    // Check that the board is correct
    EXPECT_EQ(state.board().value(), board.value());

    // Check that the current player is set to ALICE
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
    ValidateState(state, Board(), TicTacToeState::PlayerId::ALICE, false, false, Board::Cell::NEITHER);

    // Check that the fingerprint is EMPTY
    EXPECT_EQ(state.fingerprint(), ZHash().value());
}

TEST(TicTacToeState, Constructor_board_currentPlayer)
{
    ASSERT_NO_THROW(TicTacToeState(boardDraw, TicTacToeState::PlayerId::BOB));

    {
        ASSERT_NO_THROW(TicTacToeState(board0, TicTacToeState::PlayerId::ALICE));
        TicTacToeState state(board0, TicTacToeState::PlayerId::ALICE);
        ValidateState(state, board0, TicTacToeState::PlayerId::ALICE, false, false, Board::Cell::NEITHER);
    }
    {
        ASSERT_NO_THROW(TicTacToeState(boardNotDone, TicTacToeState::PlayerId::ALICE));
        TicTacToeState state(boardNotDone, TicTacToeState::PlayerId::ALICE);
        ValidateState(state, boardNotDone, TicTacToeState::PlayerId::ALICE, false, false, Board::Cell::NEITHER);
    }
    {
        ASSERT_NO_THROW(TicTacToeState(boardXWins, TicTacToeState::PlayerId::BOB));
        TicTacToeState state(boardXWins, TicTacToeState::PlayerId::BOB);
        ValidateState(state, boardXWins, TicTacToeState::PlayerId::BOB, true, false, Board::Cell::X);
    }
    {
        ASSERT_NO_THROW(TicTacToeState(boardOWins, TicTacToeState::PlayerId::ALICE));
        TicTacToeState state(boardOWins, TicTacToeState::PlayerId::ALICE);
        ValidateState(state, boardOWins, TicTacToeState::PlayerId::ALICE, true, false, Board::Cell::O);
    }
    {
        ASSERT_NO_THROW(TicTacToeState(boardDraw, TicTacToeState::PlayerId::BOB));
        TicTacToeState state(boardDraw, TicTacToeState::PlayerId::BOB);
        ValidateState(state, boardDraw, TicTacToeState::PlayerId::BOB, true, true, Board::Cell::NEITHER);
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
    expected.turn();                                     // Change turn to O
    EXPECT_EQ(state.fingerprint(), expected.value());

    // O makes a move marking (0, 1)
    state.move(0, 1);
    expected.move(Board::Cell::O, Board::toIndex(0, 1)); // Simulate the move
    expected.turn();                                     // Change turn to X
    EXPECT_EQ(state.fingerprint(), expected.value());

    // X makes a move marking (0, 0)
    state.move(0, 0);
    expected.move(Board::Cell::X, Board::toIndex(0, 0)); // Simulate the move
    expected.turn();                                     // Change turn to O
    EXPECT_EQ(state.fingerprint(), expected.value());

    // O makes a move marking (2, 2)
    state.move(2, 2);
    expected.move(Board::Cell::O, Board::toIndex(2, 2)); // Simulate the move
    expected.turn();                                     // Change turn to X
    EXPECT_EQ(state.fingerprint(), expected.value());

    // X makes a move marking (2, 0)
    state.move(2, 0);
    expected.move(Board::Cell::X, Board::toIndex(2, 0)); // Simulate the move
    expected.turn();                                     // Change turn to O
    EXPECT_EQ(state.fingerprint(), expected.value());

    // O makes a move marking (1, 0)
    state.move(1, 0);
    expected.move(Board::Cell::O, Board::toIndex(1, 0)); // Simulate the move
    expected.turn();                                     // Change turn to O
    EXPECT_EQ(state.fingerprint(), expected.value());

    // X makes a move marking (0, 2), winning the game
    state.move(0, 2);
    expected.move(Board::Cell::X, Board::toIndex(0, 2)); // Simulate the move
    expected.turn();                                     // Change turn to O
    expected.done(Board::Cell::X);                       // Mark the game as done with X as the winner
    EXPECT_EQ(state.fingerprint(), expected.value());
}

TEST(TicTacToeState, WhoseTurn)
{
    {
        TicTacToeState state;
        EXPECT_EQ(state.whoseTurn(), TicTacToeState::PlayerId::ALICE);

        // X makes a move marking (1, 1)
        state.move(1, 1);
        EXPECT_EQ(state.whoseTurn(), TicTacToeState::PlayerId::BOB);
    }
    {
        TicTacToeState state(Board(), TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(state.whoseTurn(), TicTacToeState::PlayerId::BOB);

        // O makes a move marking (1, 1)
        state.move(1, 1);
        EXPECT_EQ(state.whoseTurn(), TicTacToeState::PlayerId::ALICE);
    }
}

TEST(TicTacToeState, Move)
{
    {
        Board board;
        TicTacToeState state;
        ValidateState(state, board, TicTacToeState::PlayerId::ALICE, false, false, Board::Cell::NEITHER);

        // X makes a move marking (1, 1)
        state.move(1, 1);
        board.set(Board::toIndex(1, 1), Board::Cell::X);
        ValidateState(state, board, TicTacToeState::PlayerId::BOB, false, false, Board::Cell::NEITHER);

        // O makes a move marking (0, 1)
        state.move(0, 1);
        board.set(Board::toIndex(0, 1), Board::Cell::O);
        ValidateState(state, board, TicTacToeState::PlayerId::ALICE, false, false, Board::Cell::NEITHER);

        // X makes a move marking (0, 0)
        state.move(0, 0);
        board.set(Board::toIndex(0, 0), Board::Cell::X);
        ValidateState(state, board, TicTacToeState::PlayerId::BOB, false, false, Board::Cell::NEITHER);

        // O makes a move marking (2, 2)
        state.move(2, 2);
        board.set(Board::toIndex(2, 2), Board::Cell::O);
        ValidateState(state, board, TicTacToeState::PlayerId::ALICE, false, false, Board::Cell::NEITHER);

        // X makes a move marking (2, 0)
        state.move(2, 0);
        board.set(Board::toIndex(2, 0), Board::Cell::X);
        ValidateState(state, board, TicTacToeState::PlayerId::BOB, false, false, Board::Cell::NEITHER);

        // O makes a move marking (1, 0)
        state.move(1, 0);
        board.set(Board::toIndex(1, 0), Board::Cell::O);
        ValidateState(state, board, TicTacToeState::PlayerId::ALICE, false, false, Board::Cell::NEITHER);

        // X makes a move marking (0, 2), winning the game
        state.move(0, 2);
        board.set(Board::toIndex(0, 2), Board::Cell::X);
    }
    {
        TicTacToeState state(boardDrawNext, TicTacToeState::PlayerId::ALICE);
        ValidateState(state, boardDrawNext, TicTacToeState::PlayerId::ALICE, false, false, Board::Cell::NEITHER);

        // X makes a move marking (2, 2) and its a draw
        state.move(2, 2);
        boardDraw.set(Board::toIndex(2, 2), Board::Cell::X);
        ValidateState(state, boardDraw, TicTacToeState::PlayerId::BOB, true, true, Board::Cell::NEITHER);
    }
}

TEST(TicTacToeState, IsDone)
{
    {
        TicTacToeState state(board0, TicTacToeState::PlayerId::ALICE);
        EXPECT_FALSE(state.isDone());
    }
    {
        TicTacToeState state(boardNotDone, TicTacToeState::PlayerId::ALICE);
        EXPECT_FALSE(state.isDone());
    }
    {
        TicTacToeState state(boardXWins, TicTacToeState::PlayerId::BOB);
        EXPECT_TRUE(state.isDone());
    }
    {
        TicTacToeState state(boardOWins, TicTacToeState::PlayerId::ALICE);
        EXPECT_TRUE(state.isDone());
    }
    {
        TicTacToeState state(boardDraw, TicTacToeState::PlayerId::BOB);
        EXPECT_TRUE(state.isDone());
    }
}

TEST(TicTacToeState, IsDraw)
{
    {
        TicTacToeState state(board0, TicTacToeState::PlayerId::ALICE);
        EXPECT_FALSE(state.isDraw());
    }
    {
        TicTacToeState state(boardNotDone, TicTacToeState::PlayerId::ALICE);
        EXPECT_FALSE(state.isDraw());
    }
    {
        TicTacToeState state(boardXWins, TicTacToeState::PlayerId::BOB);
        EXPECT_FALSE(state.isDraw());
    }
    {
        TicTacToeState state(boardOWins, TicTacToeState::PlayerId::ALICE);
        EXPECT_FALSE(state.isDraw());
    }
    {
        TicTacToeState state(boardDraw, TicTacToeState::PlayerId::BOB);
        EXPECT_TRUE(state.isDraw());
    }
}

TEST(TicTacToeState, Winner)
{
    {
        TicTacToeState state(board0, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(state.winner(), Board::Cell::NEITHER);
    }
    {
        TicTacToeState state(boardNotDone, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(state.winner(), Board::Cell::NEITHER);
    }
    {
        TicTacToeState state(boardXWins, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(state.winner(), Board::Cell::X);
    }
    {
        TicTacToeState state(boardOWins, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(state.winner(), Board::Cell::O);
    }
    {
        TicTacToeState state(boardDraw, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(state.winner(), Board::Cell::NEITHER);
    }
}

TEST(TicTacToeState, Board)
{
    {
        TicTacToeState state(board0, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(state.board().value(), board0.value());
    }
    {
        TicTacToeState state(boardNotDone, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(state.board().value(), boardNotDone.value());
    }
    {
        TicTacToeState state(boardXWins, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(state.board().value(), boardXWins.value());
    }
    {
        TicTacToeState state(boardOWins, TicTacToeState::PlayerId::ALICE);
        EXPECT_EQ(state.board().value(), boardOWins.value());
    }
    {
        TicTacToeState state(boardDraw, TicTacToeState::PlayerId::BOB);
        EXPECT_EQ(state.board().value(), boardDraw.value());
    }
}

TEST(TicTacToeState, ToCell)
{
    EXPECT_EQ(TicTacToeState::toCell(TicTacToeState::PlayerId::ALICE), Board::Cell::X);
    EXPECT_EQ(TicTacToeState::toCell(TicTacToeState::PlayerId::BOB), Board::Cell::O);
}

TEST(TicTacToeState, ToPlayerId)
{
    EXPECT_TRUE(TicTacToeState::toPlayerId(Board::Cell::X).has_value());
    EXPECT_TRUE(TicTacToeState::toPlayerId(Board::Cell::O).has_value());
    EXPECT_FALSE(TicTacToeState::toPlayerId(Board::Cell::NEITHER).has_value());
    EXPECT_EQ(TicTacToeState::toPlayerId(Board::Cell::X).value(), TicTacToeState::PlayerId::ALICE);
    EXPECT_EQ(TicTacToeState::toPlayerId(Board::Cell::O).value(), TicTacToeState::PlayerId::BOB);
}
} // namespace TicTacToe
