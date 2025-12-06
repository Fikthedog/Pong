# Pong Game

A simple implementation of the classic Pong game in C using SDL2.

## Requirements

- SDL2 library installed (via Homebrew on macOS)
- Clang compiler

## Installation

### macOS (using Homebrew)

```bash
brew install sdl2
```

## Compilation

Use one of the commands below to compile on macOS (Homebrew):

```bash
# Direct clang compile (explicit include/lib paths)
clang -g main.c -o Pong -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2

# Or use sdl2-config to populate flags automatically
clang -g main.c -o Pong $(sdl2-config --cflags --libs)
```

Note: the source includes SDL as `#include <SDL2/SDL.h>` when using the Homebrew include path above.

## Running the Game

```bash
./Pong
```

## Controls

- **Left Paddle:**
  - W - Move up
  - S - Move down

- **Right Paddle:**
  - Up Arrow - Move up
  - Down Arrow - Move down

## Gameplay

- The game starts with the ball at the center
- Players control paddles to hit the ball back and forth
- A point is scored when the ball passes the opponent's paddle
- The ball speed and direction change based on paddle contact
- First to reach the highest score wins!

## Features

- Real-time paddle control
- Ball physics and collision detection
- Score tracking with digital display
- 60 FPS game loop
- Smooth paddle-to-ball interaction

## Game Objects

- **Ball**: 10x10 pixel square moving across the screen
- **Paddles**: 15x100 pixel rectangles on each side
- **Center Line**: Visual divider in the middle of the screen

## Code Structure

- `init_game()` - Initialize game state
- `handle_input()` - Process keyboard input
- `update()` - Update game logic and collisions
- `render()` - Draw all game elements
- `draw_digit()` - Render score digits
- `main()` - Main game loop
