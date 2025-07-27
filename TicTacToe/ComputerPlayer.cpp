#include "ComputerPlayer.h"

#include "Board.h"
#include "TicTacToeEvaluator.h"
#include "TicTacToeState.h"

#include "GamePlayer/GameTree.h"
#include "GamePlayer/StaticEvaluator.h"
#include "GamePlayer/TranspositionTable.h"

#include <cassert>
#include <vector>
#include <functional>

ComputerPlayer::ComputerPlayer(GamePlayer::GameState::PlayerId playerId)
    : Player(playerId)
    , gameTree_(nullptr)
    , staticEvaluator_(nullptr)
    , transpositionTable_(nullptr)
{
    staticEvaluator_ = std::make_shared<TicTacToeEvaluator>();
    transpositionTable_ = std::make_shared<GamePlayer::TranspositionTable>(362880, 9);
    gameTree_ = new GamePlayer::GameTree(transpositionTable_,
                                         staticEvaluator_,
                                         std::bind(&ComputerPlayer::responseGenerator, this, std::placeholders::_1, std::placeholders::_2),
                                         MAX_DEPTH);
}

void ComputerPlayer::move(TicTacToeState * pState)
{
    // Let's be safe and check if the state is valid
    if (pState == nullptr || pState->isDone())
        return;

    // Find all the empty cells in the board
    std::vector<int> emptyCells;
    for (int i = 0; i < 9; ++i)
    {
        if (pState->board().at(i) == Board::Cell::NEITHER)
        {
            emptyCells.push_back(i);
        }
    }

    assert(emptyCells.size() > 0);

    // Choose a random empty cell to mark
        int randomIndex = rand() % emptyCells.size();
    auto [r, c] = Board::toPosition(emptyCells[randomIndex]);
    pState->move(r, c);
}

std::vector<GamePlayer::GameState*> ComputerPlayer::responseGenerator(GamePlayer::GameState const & state, int depth)
{
    std::vector<GamePlayer::GameState*> responses;
    TicTacToeState const * pTTTState = dynamic_cast<TicTacToeState const *>(&state);
    for (int i = 0; i < 9; ++i)
    {
        if (pTTTState->board().at(i) == Board::Cell::NEITHER)
        {
            TicTacToeState * pResponse = new TicTacToeState(*pTTTState);
            auto [r, c] = Board::toPosition(i);
            pResponse->move(r, c);
            responses.push_back(pResponse);
        }
    }
    return responses;
}
