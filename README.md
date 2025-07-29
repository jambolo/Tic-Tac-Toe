# Tic-Tac-Toe
Play a game of Tic-Tac-Toe against a computer opponent.

## Command Syntax
`tictactoe [--first|-f|--second|-s] [--help|-h]`

### Options
- `--first` or `-f`: Play as the first player (X) (*default*).
- `--second` or `-s`: Play as the second player (O).
- `--help` or `-h`: Show the help message.

## Building
### Build Environment
The project uses CMake.
- There is no installation functionality.
- Tests are built if BUILD_TESTING is enabled.
- CMake 3.21 or higher
- C++17 compatible compiler

### Dependencies
- nlohmann_json - for reporting information about the AI's state
- CLI11 - for command line argument parsing
- GTest - for unit testing
- GamePlayer - my generic two-player perfect information game player (https://github.com/jambolo/GamePlayer)
