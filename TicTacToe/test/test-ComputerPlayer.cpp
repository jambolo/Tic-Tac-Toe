#include "gtest/gtest.h"

#include "TicTacToe/ComputerPlayer.h"

#include "TicTacToe/TicTacToeState.h"
#include "TicTacToe/Board.h"
#include <algorithm>
#include <numeric>

namespace TicTacToe
{
TEST(ComputerPlayer, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    ASSERT_NO_THROW(ComputerPlayer{GamePlayer::GameState::PlayerId::FIRST});
    ASSERT_NO_THROW(ComputerPlayer{GamePlayer::GameState::PlayerId::SECOND});
}

TEST(ComputerPlayer, Move)
{
    TicTacToeState state;
    ComputerPlayer computerX(GamePlayer::GameState::PlayerId::FIRST);
    auto board0 = state.board().value();
    computerX.move(&state);

    // Check that a move was made by the computer by verifying the board has exactly one NEITHER to X and nothing else changed
    auto board1 = state.board().value();

    // Verify that there is exactly one difference between the two boards
    int differencesX = std::inner_product(board0.begin(), board0.end(), board1.begin(), 0, std::plus<>(), std::not_equal_to<>());
        
    ASSERT_EQ(differencesX, 1);
        
    // Verify the difference is NEITHER -> X
    if (differencesX == 1)
    {
        auto [i0, i1] = std::mismatch(board0.begin(), board0.end(), board1.begin());
        ASSERT_TRUE((*i0 == Board::Cell::NEITHER) && (*i1 == Board::Cell::X));
    }

    ComputerPlayer computerO(GamePlayer::GameState::PlayerId::SECOND);
    auto board2 = state.board().value();

    computerO.move(&state);

    // Check that a move was made by the computer by verifying the board has exactly one NEITHER to O and nothing else changed
    auto board3 = state.board().value();

    // Verify that there is exactly one difference between the two boards
    int differencesO = std::inner_product(board2.begin(), board2.end(), board3.begin(), 0, std::plus<>(), std::not_equal_to<>());

    ASSERT_EQ(differencesO, 1);

    // Verify the difference is NEITHER -> O
    if (differencesO == 1)
    {
        auto [i2, i3] = std::mismatch(board2.begin(), board2.end(), board3.begin());
        ASSERT_TRUE((*i2 == Board::Cell::NEITHER) && (*i3 == Board::Cell::O));
    }
}

} // namespace TicTacToe
