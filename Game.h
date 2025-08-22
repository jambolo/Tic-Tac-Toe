#pragma once

#include "Window.h"

#include "TicTacToeState/TicTacToeState.h"

#include <SDL3/SDL.h>

#include <functional>
#include <memory>

class ComputerPlayer;

class Game
{
public:
    Game(bool humanGoesFirst);
    ~Game() = default;

    // SDL3 main callbacks
    SDL_AppResult handleEvent(SDL_Event * event);
    SDL_AppResult iterate();

private:
    enum class Phase
    {
        WAITING_FOR_HUMAN,
        WAITING_FOR_COMPUTER,
        GAME_OVER,
        QUIT
    };

    Window                          window_;
    TicTacToeState                  state_;
    std::unique_ptr<ComputerPlayer> computer_;
    Phase                           currentPhase_;
    bool                            needsRender_;
    Uint64                          computerMoveStartTime_; // Timer for computer moves
    TicTacToeState::PlayerId        humanId_;
    TicTacToeState::PlayerId        computerId_;

    static constexpr Uint64 COMPUTER_THINK_TIME_MS = 500;

    void handleMouseClick(int x, int y);
    void update();
    void transition(Phase newPhase);
};
