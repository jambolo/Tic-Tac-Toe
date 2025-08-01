#include "Board.h"

#include <cassert>

Board::Cell Board::at(int index) const
{
    assert(index >= 0 && index < 9);
    return board_[index];
}

void Board::set(int index, Board::Cell value)
{
    assert(index >= 0 && index < 9);
    board_[index] = value;
}

int Board::toIndex(int row, int column)
{
    assert(row >= 0 && row < 3 && column >= 0 && column < 3);
    return row * 3 + column;
}

std::pair<int, int> Board::toPosition(int index)
{
    assert(index >= 0 && index < 9);
    return std::make_pair(index / 3, index % 3);
}
