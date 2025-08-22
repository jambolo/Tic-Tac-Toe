#pragma once

#include <SDL3/SDL.h>
#include <memory>

class TicTacToeState;

class Window
{
public:
    Window(int width = 600, int height = 600);
    ~Window();

    void render(const TicTacToeState & state);
    void clear();
    SDL_Window * window() const { return window_; }

    // Convert screen coordinates to board position
    std::pair<int, int> screenToBoard(int x, int y) const;

private:
    SDL_Window *   window_;
    SDL_Renderer * renderer_;
    int            width_;
    int            height_;
    int            cellSize_;
    int            boardOffsetX_;
    int            boardOffsetY_;

    void drawGrid();
    void drawX(int row, int col);
    void drawO(int row, int col);
};
