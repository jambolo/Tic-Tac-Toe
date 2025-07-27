#pragma once

#include "Board.h"
#include "ZHash.h"

#include "GamePlayer/GameState.h"

#include <array>
#include <cstdint>
#include <optional>
#include <memory>
#include <utility>
#include <vector>

// A tic-tac-toe game state.

class TicTacToeState : public GamePlayer::GameState
{
public:
    // Default constructor - creates empty board with first player to move
    TicTacToeState();

    // Constructor with specific board state and current player. The board is assued to be valid.
    TicTacToeState(Board const & board, PlayerId currentPlayer);

    // Destructor
    virtual ~TicTacToeState() = default;

    // Make a move for the current player at the specified position. The cell must be empty.
    void move(int row, int column);

    // Returns a fingerprint for this state. Overrides GameState::fingerprint().
    virtual uint64_t fingerprint() const override;

    // Returns the player whose turn it is. Overrides GameState::whoseTurn().
    virtual PlayerId whoseTurn() const override { return currentPlayer_; };

    // Check if the game is over (win or draw)
    bool isDone() const { return done_; }

    // Check if the game is a draw
    bool isDraw() const { return done_ && winner_ == Board::Cell::NEITHER; }

    // Returns the winner
    Board::Cell winner() const { return winner_; }

    // Get the board as a const reference
    Board const & board() const { return board_; }

    // Convert PlayerId to Board::Cell
    static Board::Cell toCell(PlayerId player) { return (player == PlayerId::FIRST) ? Board::Cell::X : Board::Cell::O; }

    // Convert Cell to PlayerId
    static std::optional<PlayerId> toPlayerId(Board::Cell cell)
    {
        return (cell == Board::Cell::NEITHER) ? std::optional<PlayerId>() :
            (cell == Board::Cell::X) ? std::optional<PlayerId>(PlayerId::FIRST) :
            std::optional<PlayerId>(PlayerId::SECOND);
    }

private:
    Board board_;               // Board stored in row-major order
    PlayerId currentPlayer_;    // Current player to move
    bool done_;                 // Indicates if the game is done
    Board::Cell winner_;        // Cell value of the winner (NEITHER means still playing or done with a draw)
    ZHash zhash_;               // Zobrist hash for the board state

    void checkIfDone(); // Determine the game status after a move
};
