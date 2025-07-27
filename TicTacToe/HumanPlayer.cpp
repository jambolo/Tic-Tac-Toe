#include "HumanPlayer.h"
#include "HumanPlayer.h"
#include "HumanPlayer.h"

#include "Board.h"
#include "TicTacToeState.h"

#include <iostream>
#include <limits>
#include <string>

HumanPlayer::HumanPlayer(GamePlayer::GameState::PlayerId playerId)
    : Player(playerId)
{
}

void HumanPlayer::move(TicTacToeState * pState)
{
    // Display the current board state
    std::cout << "\nCurrent board state:\n";
    displayBoard(*pState);
    
    // Determine the player's symbol
    char playerSymbol = (TicTacToeState::toCell(playerId_) == Board::Cell::X) ? 'X' : 'O';
    std::cout << "\nPlayer " << playerSymbol << ", it's your turn!\n";
    
    // Get valid move from user
    int row, col;
    while (true)
    {
        std::cout << "Enter your move (row col), where row and col are 0-2: ";
        
        // Read input
        if (!(std::cin >> row >> col))
        {
            // Handle invalid input (non-numeric)
            std::cout << "Invalid input! Please enter two numbers.\n";
            clearInputStream();
            continue;
        }
        
        // Validate move
        if (isValidMove(*pState, row, col))
        {
            break;
        }
        else
        {
            std::cout << "Invalid move! ";
            if (row < 0 || row > 2 || col < 0 || col > 2)
            {
                std::cout << "Row and column must be between 0 and 2.\n";
            }
            else
            {
                std::cout << "Cell (" << row << ", " << col << ") is already occupied.\n";
            }
        }
    }
    
    // Create a new state with the move applied
    pState->move(row, col);
    
    std::cout << "You chose position (" << row << ", " << col << ")\n";
}

void HumanPlayer::displayBoard(TicTacToeState const & state) const
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
}

bool HumanPlayer::isValidMove(TicTacToeState const & state, int row, int col) const
{
    // Check if position is within bounds
    if (row < 0 || row > 2 || col < 0 || col > 2)
    {
        return false;
    }
    
    // Check if the cell is empty
    return state.board().at(row, col) == Board::Cell::NEITHER;
}

void HumanPlayer::clearInputStream() const
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}