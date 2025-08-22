#include "Window.h"

#include "TicTacToeState/TicTacToeState.h"

#define _USE_MATH_DEFINES 1
#include <iostream>
#include <math.h>

Window::Window(int width, int height)
    : window_(nullptr)
    , renderer_(nullptr)
    , width_(width)
    , height_(height)
    , cellSize_(std::min(width, height) / 3 - 20)
    , boardOffsetX_((width - cellSize_ * 3) / 2)
    , boardOffsetY_((height - cellSize_ * 3) / 2)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
    }

    window_ = SDL_CreateWindow("Tic-Tac-Toe", width_, height_, 0);
    if (!window_)
    {
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_)
    {
        SDL_DestroyWindow(window_);
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
    }
}

Window::~Window()
{
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

void Window::clear()
{
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255); // White background
    SDL_RenderClear(renderer_);
}

void Window::render(TicTacToeState const & state)
{
    clear();
    drawGrid();

    Board const & board = state.board();
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            Board::Cell cell = board.at(row, col);
            if (cell == Board::Cell::X)
            {
                drawX(row, col);
            }
            else if (cell == Board::Cell::O)
            {
                drawO(row, col);
            }
        }
    }
    SDL_RenderPresent(renderer_);
}

void Window::drawGrid()
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255); // Black lines

    // Vertical lines
    for (int i = 1; i < 3; ++i)
    {
        int x = boardOffsetX_ + i * cellSize_;
        SDL_RenderLine(renderer_, x, boardOffsetY_, x, boardOffsetY_ + 3 * cellSize_);
    }

    // Horizontal lines
    for (int i = 1; i < 3; ++i)
    {
        int y = boardOffsetY_ + i * cellSize_;
        SDL_RenderLine(renderer_, boardOffsetX_, y, boardOffsetX_ + 3 * cellSize_, y);
    }

    // Border
    SDL_FRect border = {static_cast<float>(boardOffsetX_),
                        static_cast<float>(boardOffsetY_),
                        static_cast<float>(3 * cellSize_),
                        static_cast<float>(3 * cellSize_)};
    SDL_RenderRect(renderer_, &border);
}

void Window::drawX(int row, int col)
{
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255); // Red X

    int x    = boardOffsetX_ + col * cellSize_ + 10;
    int y    = boardOffsetY_ + row * cellSize_ + 10;
    int size = cellSize_ - 20;

    // Draw X as two diagonal lines
    SDL_RenderLine(renderer_, x, y, x + size, y + size);
    SDL_RenderLine(renderer_, x + size, y, x, y + size);
}

void Window::drawO(int row, int col)
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255); // Blue O

    int centerX = boardOffsetX_ + col * cellSize_ + cellSize_ / 2;
    int centerY = boardOffsetY_ + row * cellSize_ + cellSize_ / 2;
    int radius  = cellSize_ / 2 - 15;

    // Draw circle (simplified - draw as octagon)
    int const points = 32;
    for (int i = 0; i < points; ++i)
    {
        float angle1 = (2.0f * M_PI * i) / points;
        float angle2 = (2.0f * M_PI * (i + 1)) / points;

        int x1 = centerX + static_cast<int>(radius * cos(angle1));
        int y1 = centerY + static_cast<int>(radius * sin(angle1));
        int x2 = centerX + static_cast<int>(radius * cos(angle2));
        int y2 = centerY + static_cast<int>(radius * sin(angle2));

        SDL_RenderLine(renderer_, x1, y1, x2, y2);
    }
}

std::pair<int, int> Window::screenToBoard(int x, int y) const
{
    int col = (x - boardOffsetX_) / cellSize_;
    int row = (y - boardOffsetY_) / cellSize_;

    // Clamp to valid range
    col = std::max(0, std::min(2, col));
    row = std::max(0, std::min(2, row));

    return {row, col};
}
