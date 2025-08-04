#pragma once

#include "Components/Board.h"
#include "GamePlayer/GameState.h"
#include "ZHash.h"

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

// A tic-tac-toe game state.

class TicTacToeState : public GamePlayer::GameState
{
public:
    using PlayerId = GamePlayer::GameState::PlayerId;
    struct Move
    {
        Board::Cell cell;
        int         row;
        int         column;
    };

    // Default constructor - creates empty board with Alice to move
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
    virtual PlayerId whoseTurn() const override { return currentPlayer_; }

    // Returns true if the game is over (win or draw)
    bool isDone() const { return done_; }

    // Returns true if the game is a draw
    bool isDraw() const { return done_ && winner_ == Board::Cell::NEITHER; }

    // Returns the winner
    Board::Cell winner() const { return winner_; }

    // Returns the board
    Board const & board() const { return board_; }

    // Returns the last move made by the current player
    Move const & lastMove() const { return lastMove_; }

    // Converts PlayerId to Board::Cell
    static Board::Cell toCell(PlayerId player) { return (player == PlayerId::ALICE) ? Board::Cell::X : Board::Cell::O; }

    // Converts Cell to PlayerId
    static std::optional<PlayerId> toPlayerId(Board::Cell cell)
    {
        return (cell == Board::Cell::NEITHER) ? std::nullopt :
               (cell == Board::Cell::X)       ? std::optional<PlayerId>(PlayerId::ALICE) :
                                                std::optional<PlayerId>(PlayerId::BOB);
    }

private:
    Board       board_;         // Board stored in row-major order
    PlayerId    currentPlayer_; // Current player to move
    bool        done_;          // Indicates if the game is done
    Board::Cell winner_;        // Cell value of the winner (NEITHER means still playing or done with a draw)
    ZHash       zhash_;         // Zobrist hash for the board state
    Move        lastMove_;      // The last move made by the current player

    void checkIfDone(); // Determine the game status after a move
};
