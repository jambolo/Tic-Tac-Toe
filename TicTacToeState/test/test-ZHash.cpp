#include "gtest/gtest.h"

#include "TicTacToeState/TicTacToeState.h"
#include "TicTacToeState/ZHash.h"

namespace TicTacToe
{
TEST(ZHash, Z)
{
    EXPECT_EQ(sizeof (ZHash::Z), 8);
}

TEST(ZHash, EMPTY)
{
    EXPECT_EQ(ZHash::EMPTY, 0);
}

TEST(ZHash, UNDEFINED)
{
    EXPECT_EQ(ZHash::UNDEFINED, ~0);
}

TEST(ZHash, Constructor_z)
{
    ASSERT_NO_THROW(ZHash());
    ASSERT_NO_THROW(ZHash{ ZHash::EMPTY });
    ASSERT_NO_THROW(ZHash{ ZHash::UNDEFINED });
    ASSERT_NO_THROW(ZHash(1));
    EXPECT_EQ(ZHash().value(), ZHash::EMPTY);
    EXPECT_EQ(ZHash(ZHash::EMPTY).value(), ZHash::EMPTY);
    EXPECT_EQ(ZHash(ZHash::UNDEFINED).value(), ZHash::UNDEFINED);
    EXPECT_EQ(ZHash(1).value(), 1);
}

TEST(ZHash, Constructor_board_currentPlayer_done_winner)
{
    Board board0;
    ASSERT_NO_THROW(ZHash(board0, TicTacToeState::PlayerId::FIRST, false, Board::Cell::NEITHER));

    ZHash z0(board0, TicTacToeState::PlayerId::FIRST, false, Board::Cell::NEITHER);
    EXPECT_EQ(z0.value(), ZHash::EMPTY);

    ZHash z1(board0, TicTacToeState::PlayerId::SECOND, false, Board::Cell::NEITHER);
    EXPECT_NE(z1.value(), z0.value());

    ZHash z2(board0, TicTacToeState::PlayerId::FIRST, false, Board::Cell::X);
    EXPECT_EQ(z2.value(), z0.value()); // winner should be ignored in this case

    ZHash z3(board0, TicTacToeState::PlayerId::FIRST, false, Board::Cell::O);
    EXPECT_EQ(z3.value(), z0.value()); // winner should be ignored in this case

    ZHash z5(board0, TicTacToeState::PlayerId::FIRST, true, Board::Cell::NEITHER);
    EXPECT_NE(z5.value(), z0.value());

    ZHash z6(board0, TicTacToeState::PlayerId::FIRST, true, Board::Cell::X);
    EXPECT_NE(z6.value(), z0.value());
    EXPECT_NE(z6.value(), z5.value());

    ZHash z7(board0, TicTacToeState::PlayerId::FIRST, true, Board::Cell::O);
    EXPECT_NE(z7.value(), z0.value());
    EXPECT_NE(z7.value(), z6.value());
    EXPECT_NE(z7.value(), z5.value());
}

TEST(ZHash, Value)
{
    // I'll think of a way to test this later. ZHash::value() is used everywhere, so it will be tested indirectly.
}

TEST(ZHash, Move)
{
    // Since values are random, theyh cannot be compared to predetermined values.

    // Setting a cell to NEITHER should not change the value
    {
        ZHash z(0xB1D28E7F56A3F9C4);
        ZHash z0 = z;
        for (int i = 0; i < 9; ++i)
        {
            z.move(Board::Cell::NEITHER, i);
            EXPECT_EQ(z.value(), z0.value());
        }
    }
    // Marking a cell as X or O should change the value. Marking it twice reverts the value.
    {
        ZHash z(0x28E7F56A3F9C4B1D);
        ZHash z0 = z;
        for (int i = 0; i < 9; ++i)
        {
            ZHash zN = z;
            z.move(Board::Cell::X, i);
            EXPECT_NE(z.value(), zN.value());
            ZHash zX = z;
            z.move(Board::Cell::O, i);
            EXPECT_NE(z.value(), zX.value());
        }
        for (int i = 0; i < 9; ++i)
        {
            ZHash zN = z;
            z.move(Board::Cell::X, i);
            EXPECT_NE(z.value(), zN.value());
            ZHash zX = z;
            z.move(Board::Cell::O, i);
            EXPECT_NE(z.value(), zX.value());
        }
        EXPECT_EQ(z.value(), z0.value()); // after marking all cells twice, the value should return to the original
    }
}

TEST(ZHash, Turn)
{
    ZHash z(0xA3F9C4B1D28E7F56);
    ZHash z0 = z;
    ZHash z1 = z.turn();
    ZHash z2 = z.turn();
    EXPECT_NE(z0.value(), z1.value());
    EXPECT_NE(z1.value(), z2.value());
    EXPECT_EQ(z0.value(), z2.value()); // two turns should return to the original state
}

TEST(ZHash, Done)
{
    EXPECT_EQ(ZHash().done(Board::Cell::NEITHER), ZHash(Board(), TicTacToeState::PlayerId::FIRST, true, Board::Cell::NEITHER));
    EXPECT_EQ(ZHash().done(Board::Cell::X), ZHash(Board(), TicTacToeState::PlayerId::FIRST, true, Board::Cell::X));
    EXPECT_EQ(ZHash().done(Board::Cell::O), ZHash(Board(), TicTacToeState::PlayerId::FIRST, true, Board::Cell::O));
}

TEST(ZHash, IsUndefined)
{
    EXPECT_TRUE(ZHash(ZHash::UNDEFINED).isUndefined());
    EXPECT_FALSE(ZHash(ZHash::EMPTY).isUndefined());
    EXPECT_FALSE(ZHash(1).isUndefined());
}
} // namespace TicTacToe
