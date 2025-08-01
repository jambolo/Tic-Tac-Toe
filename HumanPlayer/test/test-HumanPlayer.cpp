#include "gtest/gtest.h"

#include "HumanPlayer/HumanPlayer.h"

namespace TicTacToe
{
TEST(HumanPlayer, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    ASSERT_NO_THROW(HumanPlayer{ TicTacToeState::PlayerId::FIRST });
    ASSERT_NO_THROW(HumanPlayer{ TicTacToeState::PlayerId::SECOND });
}

TEST(HumanPlayer, Move)
{
    // I'm not motivated to implement this test right now.
}
} // namespace TicTacToe
