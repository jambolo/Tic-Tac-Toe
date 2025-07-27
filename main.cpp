#include "TicTacToe/TicTacToeState.h"
#include "TicTacToe/HumanPlayer.h"
#include "TicTacToe/ComputerPlayer.h"

#include <iostream>

using namespace GamePlayer;

int main(int argc, char *argv[])
{
    TicTacToeState state;
    ComputerPlayer computer(GameState::PlayerId::FIRST);
    HumanPlayer human(GameState::PlayerId::SECOND);

    while (!state.isDone())
    {
        if (state.whoseTurn() == TicTacToeState::PlayerId::FIRST)
        {
            // Computer player's turn
            computer.move(&state);
        }
        else
        {
            // Human player's turn
            human.move(&state);
        }

        // Print the current board state
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                Board::Cell cell = state.board().at(i, j);
                if (cell == Board::Cell::NEITHER)
                    std::cout << ". ";
                else if (cell == Board::Cell::X)
                    std::cout << "X ";
                else
                    std::cout << "O ";
            }
            std::cout << std::endl;
        }
    }
    if (state.isDraw())
    {
        std::cout << "The game is a draw!" << std::endl;
    }
    else
    {
        Board::Cell winner = state.board().at(0, 0); // Assuming the winner is determined by the first cell
        if (winner == Board::Cell::X)
            std::cout << "Player X wins!" << std::endl;
        else if (winner == Board::Cell::O)
            std::cout << "Player O wins!" << std::endl;
        else
            std::cout << "No winner!" << std::endl;
    }
}