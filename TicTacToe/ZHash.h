#pragma once

#include "Board.h"
#include "GamePlayer/GameState.h"
#include <cstdint>

// Zobrist Hashing Calculator.
// 
// Zobrist Hashing is used to encode the state of a chess game in a single number. There is a risk of two unrelated states
// having the same value. In this implementation, the value has 64 bits and the risk of collision can be assumed to be
// very low, though not for a large number of values. The probability of at least one collision with 1 million values
// is 2.710502×10<sup>-8</sup>, but with 1 billion values, it rises to 1 in 40.
// 
// There are 6 components of state. This implementation of Zorbrist hashing incorporates most of them.
//      1. board
//      2. whose turn it is
//      3. The current status of the game (whether it is still playing) and the winner (if any)
//
// An important characteristic of a Zorbrist hash is that it is independent of the order of the changes made to reach the state.
class ZHash
{
public:

    // Type of a hash value
    using Z = std::uint64_t;

    // The value of an empty board
    static Z constexpr EMPTY = 0;

    // A value which represents an "undefined" state
    static Z constexpr UNDEFINED = ~EMPTY;

    // Constructor
    explicit ZHash(Z z = EMPTY)
        : value_(z)
    {
    }

    // Constructor
    ZHash(Board const & board,
          GamePlayer::GameState::PlayerId currentPlayer,
          bool done = false,
          Board::Cell winner = Board::Cell::NEITHER);

    // Returns the current value.
    Z value() const { return value_; }

    // Adds a piece. Returns a reference to itself
    ZHash & move(Board::Cell cell, int index);

    // Changes whose turn. Returns a reference to itself.
    ZHash & turn();

    // Changes from the PLAYING status to the WON or DRAW status.
    ZHash & done(Board::Cell winner);

    // Returns true if the value is undefined (i.e. not a legal Z value)
    bool isUndefined() const { return value_ == UNDEFINED; }

private:

    friend bool operator ==(ZHash const & x, ZHash const & y);
    friend bool operator <(ZHash const& x, ZHash const& y);


    class ZValueTable; // declared below

    Z value_; // The hash value

    static ZValueTable const zValueTable_; // The hash values for each incremental state change
};

// Equality operator
inline bool operator ==(ZHash const & x, ZHash const & y)
{
    return x.value_ == y.value_;
}


// Less than operator
inline bool operator <(ZHash const& x, ZHash const& y)
{
    return x.value_ < y.value_;
}


class ZHash::ZValueTable
{
public:

    ZValueTable();

    Z cellValue(Board::Cell cell, int row, int column) const { return cellValue(cell, Board::toIndex(row, column)); }
    Z cellValue(Board::Cell cell, int index) const { return cellValues_[index][static_cast<int>(cell)]; }
    Z turnValue() const { return turnValue_; }
    Z winnerValue(Board::Cell winner) const { return winnerValues_[static_cast<int>(winner)]; }

private:

    Z cellValues_[9][3];
    Z turnValue_;
    Z winnerValues_[3];
};
