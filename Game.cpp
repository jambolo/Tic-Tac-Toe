#include "Game.h"

#include "ComputerPlayer/ComputerPlayer.h"
#include "TicTacToeState/TicTacToeState.h"

#include <iostream>

Game::Game(bool humanGoesFirst)
    : window_()
    , state_()
    , currentPhase_(Phase::WAITING_FOR_HUMAN)
    , needsRender_(true)
    , computerMoveStartTime_(0)
    , humanId_(humanGoesFirst ? TicTacToeState::PlayerId::ALICE : TicTacToeState::PlayerId::BOB)
    , computerId_(humanGoesFirst ? TicTacToeState::PlayerId::BOB : TicTacToeState::PlayerId::ALICE)
{

    computer_ = std::make_unique<ComputerPlayer>(computerId_);

    // Set initial phase based on who goes first
    if (state_.whoseTurn() == computerId_)
    {
        transition(Phase::WAITING_FOR_COMPUTER);
    }
}

SDL_AppResult Game::handleEvent(SDL_Event * event)
{
    if (event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE))
    {
        transition(Phase::QUIT);
        return SDL_APP_SUCCESS;
    }

    switch (currentPhase_)
    {
    case Phase::WAITING_FOR_HUMAN:
        if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT)
        {
            handleMouseClick(static_cast<int>(event->button.x), static_cast<int>(event->button.y));
        }
        break;

    case Phase::GAME_OVER:
    case Phase::WAITING_FOR_COMPUTER:
    case Phase::QUIT:
        // No user input handled in these phases
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult Game::iterate()
{
    if (currentPhase_ == Phase::QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    update();

    if (needsRender_)
    {
        window_.render(state_);
        needsRender_ = false;
    }
    else
    {
        // Wait 1/60th of a second to avoid busy-waiting
        SDL_Delay(1000 / 60);
    }

    return SDL_APP_CONTINUE;
}

void Game::handleMouseClick(int x, int y)
{
    if (state_.whoseTurn() != humanId_)
    {
        return;
    }

    auto [row, col] = window_.screenToBoard(x, y);

    // Check if the cell is empty and the move is valid
    if (state_.board().at(row, col) == Board::Cell::NEITHER)
    {
        state_.move(row, col);
        needsRender_ = true;

        if (state_.isDone())
        {
            transition(Phase::GAME_OVER);
        }
        else
        {
            transition(Phase::WAITING_FOR_COMPUTER);
        }
    }
}

void Game::update()
{
    switch (currentPhase_)
    {
    case Phase::WAITING_FOR_COMPUTER:
        // Check if enough time has passed for computer to "think"
        if (SDL_GetTicks() - computerMoveStartTime_ >= COMPUTER_THINK_TIME_MS)
        {
            if (state_.whoseTurn() == computer_->playerId())
            {
                computer_->move(&state_);
                needsRender_ = true;

                if (state_.isDone())
                {
                    transition(Phase::GAME_OVER);
                }
                else
                {
                    transition(Phase::WAITING_FOR_HUMAN);
                }
            }
        }
        break;

    case Phase::GAME_OVER:
    {
        auto         winner   = TicTacToeState::toPlayerId(state_.winner()).value_or(TicTacToeState::PlayerId::ALICE);
        bool         humanWon = (winner == humanId_);
        char const * message  = state_.isDraw() ? "It's a Draw!" : humanWon ? "You Win!" : "Computer Wins!";
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", message, window_.window());

        // Reset game
        state_ = TicTacToeState();
        transition(state_.whoseTurn() == humanId_ ? Phase::WAITING_FOR_HUMAN : Phase::WAITING_FOR_COMPUTER);
        break;
    }

    case Phase::WAITING_FOR_HUMAN:
    case Phase::QUIT:
        // No updates needed
        break;
    }
}

void Game::transition(Phase newPhase)
{
    if (newPhase == currentPhase_)
    {
        return;
    }

    currentPhase_ = newPhase;
    needsRender_  = true;

    // Handle phase-specific setup
    switch (newPhase)
    {
    case Phase::WAITING_FOR_COMPUTER:
        computerMoveStartTime_ = SDL_GetTicks();
        break;

    case Phase::GAME_OVER:
        break;

    case Phase::QUIT:
        break;

    case Phase::WAITING_FOR_HUMAN:
        break;
    }
}
