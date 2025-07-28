#include "TicTacToe/TicTacToeState.h"
#include "TicTacToe/HumanPlayer.h"
#include "TicTacToe/ComputerPlayer.h"

#include <cassert>
#include <iostream>

using namespace GamePlayer;

static void displayBoard(TicTacToeState const & state);

int main(int argc, char* argv[])
{
    TicTacToeState state;
    ComputerPlayer computer(GameState::PlayerId::FIRST);
    HumanPlayer human(GameState::PlayerId::SECOND);

    while (!state.isDone())
    {
        displayBoard(state);

        if (state.whoseTurn() == TicTacToeState::PlayerId::FIRST)
        {
            // Computer player's turn
            std::cout << "Computer's turn...\n";
            computer.move(&state);
        }
        else
        {
            // Human player's turn
            std::cout << "Your turn...\n";
            human.move(&state);
        }
    }

    displayBoard(state);

    if (state.isDraw())
    {
        std::cout << "The game is a draw!" << std::endl;
    }
    else
    {
        Board::Cell winner = state.winner();
        if (winner == Board::Cell::X)
            std::cout << "Player X wins!" << std::endl;
        else if (winner == Board::Cell::O)
            std::cout << "Player O wins!" << std::endl;
        else
            assert(false && "The game is not a draw, but there is no winner.");
    }

    return 0;
}


static void displayBoard(TicTacToeState const& state)
{
    std::cout << "   0   1   2\n";
    for (int row = 0; row < 3; ++row)
    {
        std::cout << row << " ";
        for (int col = 0; col < 3; ++col)
        {
            Board::Cell cell = state.board().at(row, col);
            char cellChar;

            switch (cell)
            {
            case Board::Cell::NEITHER:
                cellChar = '.';
                break;
            case Board::Cell::X:
                cellChar = 'X';
                break;
            case Board::Cell::O:
                cellChar = 'O';
                break;
            }

            std::cout << " " << cellChar << " ";
            if (col < 2) std::cout << "|";
        }
        std::cout << "\n";
        if (row < 2) std::cout << "  ---|---|---\n";
    }
    std::cout << "\n";
}
