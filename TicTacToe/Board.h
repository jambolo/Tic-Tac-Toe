#pragma once

#include <cstdint>
#include <array>
#include <utility>


class Board
{
public:
    // Cell values on the board
    enum class Cell : uint8_t
    {
        NEITHER = 0,
        X = 1,
        O = 2
    };

    Board(std::array<Cell, 9> const& board = {})
        : board_(board)
    {
    }

    std::array<Cell, 9> value() const { return board_; }

    // Returns the cell value at the specified position
    Cell at(int row, int column) const { return board_[toIndex(row, column)]; }

    // Returns the cell value at the specified index
    Cell at(int index) const;

    // Set the cell value at the specified position
    void set(int row, int column, Cell value) { board_[toIndex(row, column)] = value; }

    // Set the cell value at the specified position
    void set(int index, Cell value);

    // Convert row/column to index
    static int toIndex(int row, int column);

    // Convert index to row/column
    static std::pair<int, int> toPosition(int index);

private:
    std::array<Cell, 9> board_; // 3x3 board stored in row-major order
};


