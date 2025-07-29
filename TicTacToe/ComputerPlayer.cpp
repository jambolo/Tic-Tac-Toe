#include "ComputerPlayer.h"

#include "Board.h"
#include "TicTacToeEvaluator.h"
#include "TicTacToeState.h"

#include "GamePlayer/GameTree.h"
#include "GamePlayer/StaticEvaluator.h"
#include "GamePlayer/TranspositionTable.h"

#include <cassert>
#include <ctime>
#include <functional>
#include <vector>

ComputerPlayer::ComputerPlayer(GamePlayer::GameState::PlayerId playerId)
    : Player(playerId)
    , gameTree_(nullptr)
    , staticEvaluator_(nullptr)
    , transpositionTable_(nullptr)
{
    staticEvaluator_    = std::make_shared<TicTacToeEvaluator>();
    transpositionTable_ = std::make_shared<GamePlayer::TranspositionTable>(362880, 9);
    gameTree_           = new GamePlayer::GameTree(transpositionTable_,
                                                   staticEvaluator_,
                                                   std::bind(&ComputerPlayer::responseGenerator, this, std::placeholders::_1,
                                                             std::placeholders::_2),
                                                   MAX_DEPTH);
}

void ComputerPlayer::move(TicTacToeState * pState)
{
    // Let's be safe and check if the state is valid
    if (pState == nullptr || pState->isDone())
    {
        return;
    }

    // Find the best response to the current state
    auto pCopy = std::make_shared<TicTacToeState>(*pState);
    gameTree_->findBestResponse(std::static_pointer_cast<GamePlayer::GameState>(pCopy));
    auto pResponse = std::dynamic_pointer_cast<TicTacToeState>(pCopy->response_);
    assert(pResponse);
    *pState = *pResponse;
}

std::vector<GamePlayer::GameState *> ComputerPlayer::responseGenerator(GamePlayer::GameState const & state, int depth)
{
    std::vector<GamePlayer::GameState *> responses;
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
