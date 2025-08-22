#include "Game.h"
#include "Window.h"

#include "ComputerPlayer/ComputerPlayer.h"
#include "TicTacToeState/TicTacToeState.h"

#include <CLI/CLI.hpp>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cassert>
#include <iostream>

using namespace GamePlayer;

// Global game instance for SDL3 main callbacks
static std::unique_ptr<Game> g_game;

// Command line option to determine who goes first
static bool g_humanGoesFirst = true;

// Function to parse command line arguments
static int parseCommandLine(int argc, char * argv[])
{
    CLI::App cli;
    bool     first  = false;
    bool     second = false;

    auto * order = cli.add_option_group("Order of play", "Choose who goes first");
    order->add_flag("--first, -f", first, "You go first. (default)");
    order->add_flag("--second, -s", second, "The computer goes first.");
    order->require_option(0, 1);

    CLI11_PARSE(cli, argc, argv);

    if (first)
    {
        g_humanGoesFirst = true;
    }
    else if (second)
    {
        g_humanGoesFirst = false;
    }

    return 0;
}

// SDL3 main callback functions
static SDL_AppResult SDL_AppInit(void ** appstate, int argc, char * argv[])
{
    parseCommandLine(argc, argv);

    try
    {
        g_game = std::make_unique<Game>(g_humanGoesFirst);
    }
    catch (const std::exception & e)
    {
        std::cerr << "Failed to initialize game: " << e.what() << std::endl;
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult SDL_AppIterate(void * appstate)
{
    if (!g_game)
    {
        return SDL_APP_FAILURE;
    }

    return g_game->iterate();
}

static SDL_AppResult SDL_AppEvent(void * appstate, SDL_Event * event)
{
    if (!g_game)
    {
        return SDL_APP_FAILURE;
    }

    return g_game->handleEvent(event);
}

static void SDL_AppQuit(void * appstate, SDL_AppResult result)
{
    g_game.reset();
}
