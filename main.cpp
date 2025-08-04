#include "TicTacToeState/TicTacToeState.h"
#include "HumanPlayer/HumanPlayer.h"
#include "ComputerPlayer/ComputerPlayer.h"

#include <CLI/CLI.hpp>

#include <cassert>
#include <iostream>

using namespace GamePlayer;

static void displayBoard(TicTacToeState const & state);

int main(int argc, char * argv[])
{
    bool humanGoesFirst = true;     // Human player goes first by default

    {
        CLI::App cli;
        bool     first  = false;
        bool     second = false;

        auto *   order  = cli.add_option_group("Order of play", "Choose who goes first");
        order->add_flag("--first, -f", first, "You go first. (default)");
        order->add_flag("--second, -s", second, "The computer goes first.");
        order->require_option(0, 1);

        CLI11_PARSE(cli, argc, argv);

        if (first)
        {
            humanGoesFirst = true;
        }
        else if (second)
        {
            humanGoesFirst = false;
        }
    }

    TicTacToeState state;
    HumanPlayer    human(humanGoesFirst ? GameState::PlayerId::ALICE : GameState::PlayerId::BOB);
    ComputerPlayer computer(humanGoesFirst ? GameState::PlayerId::BOB : GameState::PlayerId::ALICE);

    while (!state.isDone())
    {
        displayBoard(state);

        if (state.whoseTurn() == computer.playerId())
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
        TicTacToeState::PlayerId winner = TicTacToeState::toPlayerId(state.winner()).value();
        if (winner == human.playerId())
            std::cout << "You win!" << std::endl;
        else
            std::cout << "The computer wins!" << std::endl;
    }

    return 0;
}

static void displayBoard(TicTacToeState const & state)
{
    std::cout << "   0   1   2\n";
    for (int row = 0; row < 3; ++row)
    {
        std::cout << row << " ";
        for (int col = 0; col < 3; ++col)
        {
            Board::Cell cell     = state.board().at(row, col);
            char        cellChar = '.';

            switch (cell)
            {
            case Board::Cell::X:
                cellChar = 'X';
                break;
            case Board::Cell::O:
                cellChar = 'O';
                break;
            }

            std::cout << " " << cellChar << " ";
            if (col < 2)
                std::cout << "|";
        }
        std::cout << "\n";
        if (row < 2)
            std::cout << "  ---|---|---\n";
    }
    std::cout << "\n";
}
