#include "gtest/gtest.h"

#include "TicTacToe/Board.h"

#include <algorithm>

namespace TicTacToe
{

TEST(Board, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    ASSERT_NO_THROW(Board());
    ASSERT_NO_THROW(
        Board({ {
            Board::Cell::X,       Board::Cell::X, Board::Cell::NEITHER,
            Board::Cell::X,       Board::Cell::X, Board::Cell::NEITHER,
            Board::Cell::NEITHER, Board::Cell::O, Board::Cell::O
        } })
    );
}

TEST(Board, value)
{
    std::array<Board::Cell, 9> boardData = {
        Board::Cell::X,       Board::Cell::NEITHER, Board::Cell::NEITHER, 
        Board::Cell::X,       Board::Cell::NEITHER, Board::Cell::O,       
        Board::Cell::NEITHER, Board::Cell::X,       Board::Cell::O
    };

    // Test each cell three times cycling though each possible value
    for (int i = 0; i < 3; ++i)
    {
        std::for_each(boardData.begin(), boardData.end(), [](Board::Cell& x) {
            x = static_cast<Board::Cell>((static_cast<int>(x) + 1) % 3);
            });

        Board board(boardData);
        EXPECT_EQ(board.value(), boardData);
    }
}

TEST(Board, At_row_column)
{
    std::array<Board::Cell, 9> boardData = {
        Board::Cell::X,       Board::Cell::O,       Board::Cell::NEITHER,
        Board::Cell::X,       Board::Cell::NEITHER, Board::Cell::NEITHER,
        Board::Cell::NEITHER, Board::Cell::O,       Board::Cell::X
    };

    // Test each cell three times cycling though each possible value
    for (int i = 0; i < 3; ++i)
    {
        std::for_each(boardData.begin(), boardData.end(), [](Board::Cell & x) {
            x = static_cast<Board::Cell>((static_cast<int>(x) + 1) % 3);
            });

        Board board(boardData);
        for (int r = 0; r < 3; ++r)
        {
            for (int c = 0; c < 3; ++c)
            {
                EXPECT_EQ(board.at(r, c), boardData[r * 3 + c]);
            }
        }
    }
}

TEST(Board, At_index)
{
    std::array<Board::Cell, 9> boardData = {
        Board::Cell::O,       Board::Cell::X,       Board::Cell::NEITHER,
        Board::Cell::NEITHER, Board::Cell::X,       Board::Cell::NEITHER,
        Board::Cell::O,       Board::Cell::NEITHER, Board::Cell::X
    };

    // Test each cell three times cycling though each possible value
    for (int i = 0; i < 3; ++i)
    {
        std::for_each(boardData.begin(), boardData.end(), [](Board::Cell& x) {
            x = static_cast<Board::Cell>((static_cast<int>(x) + 1) % 3);
            });

        Board board(boardData);
        for (int i = 0; i < 9; ++i)
        {
            EXPECT_EQ(board.at(i), boardData[i]);
        }
    }
}

TEST(Board, Set_row_column)
{
    std::array<Board::Cell, 9> boardData = {
        Board::Cell::NEITHER, Board::Cell::X,       Board::Cell::NEITHER,
        Board::Cell::O,       Board::Cell::X,       Board::Cell::NEITHER,
        Board::Cell::O,       Board::Cell::NEITHER, Board::Cell::X
    };

    // Test each cell three times cycling though each possible value
    Board board;
    for (int i = 0; i < 3; ++i)
    {
        std::for_each(boardData.begin(), boardData.end(), [](Board::Cell& x) {
            x = static_cast<Board::Cell>((static_cast<int>(x) + 1) % 3);
            });

        for (int r = 0; r < 3; ++r)
        {
            for (int c = 0; c < 3; ++c)
            {
                board.set(r, c, boardData[r * 3 + c]);
            }
        }

        EXPECT_EQ(board.value(), boardData);
    }
}

TEST(Board, Set_index)
{
    std::array<Board::Cell, 9> boardData = {
        Board::Cell::O,       Board::Cell::X,       Board::Cell::NEITHER,
        Board::Cell::O,       Board::Cell::NEITHER, Board::Cell::X,
        Board::Cell::NEITHER, Board::Cell::X,       Board::Cell::NEITHER
    };

    // Test each cell three times cycling though each possible value
    Board board;
    for (int i = 0; i < 3; ++i)
    {
        std::for_each(boardData.begin(), boardData.end(), [](Board::Cell& x) {
            x = static_cast<Board::Cell>((static_cast<int>(x) + 1) % 3);
            });

        for (int i = 0; i < 9; ++i)
        {
            board.set(i, boardData[i]);
        }

        EXPECT_EQ(board.value(), boardData);
    }
}

TEST(Board, ToIndex)
{
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            EXPECT_EQ(Board::toIndex(r, c), r * 3 + c);
        }
    }
}

TEST(Board, ToPosition)
{
    for (int i = 0; i < 9; ++i)
    {
        auto [r, c] = Board::toPosition(i);
        EXPECT_EQ(r, i / 3);
        EXPECT_EQ(c, i % 3);
    }
}

} // namespace TicTacToe
