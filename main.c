//Hello World Pong Game in C using SDL2 
//This is a simple implementation of the classic Pong game using the SDL2 library in C.
//Make sure you have SDL2 installed and linked properly in your build environment.
//To compile this code, you can use a command like:
// clang main.c -o Pong -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2
// Then to run the game, use:
// ./Pong


#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Paddle dimensions
#define PADDLE_WIDTH 15
#define PADDLE_HEIGHT 100
#define PADDLE_SPEED 6

// Ball dimensions
#define BALL_SIZE 10
#define BALL_SPEED 4

// Game state
typedef struct {
    float x, y;
    float vx, vy;
} Ball;

typedef struct {
    float x, y;
    float vy;
} Paddle;

typedef struct {
    Ball ball;
    Paddle left_paddle;
    Paddle right_paddle;
    int left_score;
    int right_score;
    int running;
} GameState;

// Initialize game state
void init_game(GameState *state) {
    state->ball.x = SCREEN_WIDTH / 2.0f;
    state->ball.y = SCREEN_HEIGHT / 2.0f;
    state->ball.vx = BALL_SPEED;
    state->ball.vy = BALL_SPEED;
    
    state->left_paddle.x = 10;
    state->left_paddle.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
    state->left_paddle.vy = 0;
    
    state->right_paddle.x = SCREEN_WIDTH - PADDLE_WIDTH - 10;
    state->right_paddle.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
    state->right_paddle.vy = 0;
    
    state->left_score = 0;
    state->right_score = 0;
    state->running = 1;
}

// Reset ball to center
void reset_ball(Ball *ball) {
    ball->x = SCREEN_WIDTH / 2.0f;
    ball->y = SCREEN_HEIGHT / 2.0f;
    ball->vx = (rand() % 2 ? 1 : -1) * BALL_SPEED;
    ball->vy = (rand() % 2 ? 1 : -1) * BALL_SPEED;
}

// Handle input
void handle_input(GameState *state, const Uint8 *keys) {
    state->left_paddle.vy = 0;
    state->right_paddle.vy = 0;
    
    if (keys[SDL_SCANCODE_W]) {
        state->left_paddle.vy = -PADDLE_SPEED;
    }
    if (keys[SDL_SCANCODE_S]) {
        state->left_paddle.vy = PADDLE_SPEED;
    }
    if (keys[SDL_SCANCODE_UP]) {
        state->right_paddle.vy = -PADDLE_SPEED;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        state->right_paddle.vy = PADDLE_SPEED;
    }
}

// Update game logic
void update(GameState *state) {
    // Update paddle positions
    state->left_paddle.y += state->left_paddle.vy;
    state->right_paddle.y += state->right_paddle.vy;
    
    // Clamp paddles to screen bounds
    if (state->left_paddle.y < 0) {
        state->left_paddle.y = 0;
    }
    if (state->left_paddle.y + PADDLE_HEIGHT > SCREEN_HEIGHT) {
        state->left_paddle.y = SCREEN_HEIGHT - PADDLE_HEIGHT;
    }
    if (state->right_paddle.y < 0) {
        state->right_paddle.y = 0;
    }
    if (state->right_paddle.y + PADDLE_HEIGHT > SCREEN_HEIGHT) {
        state->right_paddle.y = SCREEN_HEIGHT - PADDLE_HEIGHT;
    }
    
    // Update ball position
    state->ball.x += state->ball.vx;
    state->ball.y += state->ball.vy;
    
    // Ball collision with top/bottom
    if (state->ball.y - BALL_SIZE / 2.0f < 0) {
        state->ball.y = BALL_SIZE / 2.0f;
        state->ball.vy = -state->ball.vy;
    }
    if (state->ball.y + BALL_SIZE / 2.0f > SCREEN_HEIGHT) {
        state->ball.y = SCREEN_HEIGHT - BALL_SIZE / 2.0f;
        state->ball.vy = -state->ball.vy;
    }
    
    // Ball collision with left paddle
    if (state->ball.x - BALL_SIZE / 2.0f < state->left_paddle.x + PADDLE_WIDTH &&
        state->ball.y > state->left_paddle.y &&
        state->ball.y < state->left_paddle.y + PADDLE_HEIGHT) {
        state->ball.x = state->left_paddle.x + PADDLE_WIDTH + BALL_SIZE / 2.0f;
        state->ball.vx = -state->ball.vx;
        state->ball.vy += state->left_paddle.vy * 0.2f;
    }
    
    // Ball collision with right paddle
    if (state->ball.x + BALL_SIZE / 2.0f > state->right_paddle.x &&
        state->ball.y > state->right_paddle.y &&
        state->ball.y < state->right_paddle.y + PADDLE_HEIGHT) {
        state->ball.x = state->right_paddle.x - BALL_SIZE / 2.0f;
        state->ball.vx = -state->ball.vx;
        state->ball.vy += state->right_paddle.vy * 0.2f;
    }
    
    // Ball out of bounds - left side
    if (state->ball.x < 0) {
        state->right_score++;
        reset_ball(&state->ball);
    }
    
    // Ball out of bounds - right side
    if (state->ball.x > SCREEN_WIDTH) {
        state->left_score++;
        reset_ball(&state->ball);
    }
}

// Draw digit using simple rectangles
void draw_digit(SDL_Renderer *renderer, int digit, int x, int y, int scale) {
    int w = 2 * scale;
    int h = 3 * scale;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // Simple 7-segment-like digit drawing
    switch(digit) {
        case 0:
            SDL_Rect top = {x, y, w, scale};
            SDL_Rect mid = {x, y + h, w, scale};
            SDL_Rect bot = {x, y + 2*h, w, scale};
            SDL_Rect left_top = {x - scale/2, y + scale, scale, h};
            SDL_Rect left_bot = {x - scale/2, y + h + scale, scale, h};
            SDL_Rect right_top = {x + w, y + scale, scale, h};
            SDL_Rect right_bot = {x + w, y + h + scale, scale, h};
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &left_top);
            SDL_RenderFillRect(renderer, &left_bot);
            SDL_RenderFillRect(renderer, &right_top);
            SDL_RenderFillRect(renderer, &right_bot);
            break;
        case 1:
            SDL_Rect r1 = {x + w, y + scale, scale, 2*h + scale};
            SDL_RenderFillRect(renderer, &r1);
            break;
        case 2:
            SDL_Rect t2 = {x, y, w, scale};
            SDL_Rect m2 = {x, y + h, w, scale};
            SDL_Rect b2 = {x, y + 2*h, w, scale};
            SDL_Rect rt2 = {x + w, y + scale, scale, h};
            SDL_Rect lt2 = {x - scale/2, y + h + scale, scale, h};
            SDL_RenderFillRect(renderer, &t2);
            SDL_RenderFillRect(renderer, &m2);
            SDL_RenderFillRect(renderer, &b2);
            SDL_RenderFillRect(renderer, &rt2);
            SDL_RenderFillRect(renderer, &lt2);
            break;
        case 3:
            SDL_Rect t3 = {x, y, w, scale};
            SDL_Rect m3 = {x, y + h, w, scale};
            SDL_Rect b3 = {x, y + 2*h, w, scale};
            SDL_Rect rt3t = {x + w, y + scale, scale, h};
            SDL_Rect rt3b = {x + w, y + h + scale, scale, h};
            SDL_RenderFillRect(renderer, &t3);
            SDL_RenderFillRect(renderer, &m3);
            SDL_RenderFillRect(renderer, &b3);
            SDL_RenderFillRect(renderer, &rt3t);
            SDL_RenderFillRect(renderer, &rt3b);
            break;
        case 4:
            SDL_Rect lt4 = {x - scale/2, y + scale, scale, h};
            SDL_Rect m4 = {x, y + h, w, scale};
            SDL_Rect rt4 = {x + w, y, scale, 2*h + scale};
            SDL_RenderFillRect(renderer, &lt4);
            SDL_RenderFillRect(renderer, &m4);
            SDL_RenderFillRect(renderer, &rt4);
            break;
        case 5:
            SDL_Rect t5 = {x, y, w, scale};
            SDL_Rect m5 = {x, y + h, w, scale};
            SDL_Rect b5 = {x, y + 2*h, w, scale};
            SDL_Rect lt5 = {x - scale/2, y + scale, scale, h};
            SDL_Rect rt5 = {x + w, y + h + scale, scale, h};
            SDL_RenderFillRect(renderer, &t5);
            SDL_RenderFillRect(renderer, &m5);
            SDL_RenderFillRect(renderer, &b5);
            SDL_RenderFillRect(renderer, &lt5);
            SDL_RenderFillRect(renderer, &rt5);
            break;
        case 6:
            SDL_Rect t6 = {x, y, w, scale};
            SDL_Rect m6 = {x, y + h, w, scale};
            SDL_Rect b6 = {x, y + 2*h, w, scale};
            SDL_Rect lt6t = {x - scale/2, y + scale, scale, h};
            SDL_Rect lt6b = {x - scale/2, y + h + scale, scale, h};
            SDL_Rect rt6b = {x + w, y + h + scale, scale, h};
            SDL_RenderFillRect(renderer, &t6);
            SDL_RenderFillRect(renderer, &m6);
            SDL_RenderFillRect(renderer, &b6);
            SDL_RenderFillRect(renderer, &lt6t);
            SDL_RenderFillRect(renderer, &lt6b);
            SDL_RenderFillRect(renderer, &rt6b);
            break;
        case 7:
            SDL_Rect t7 = {x, y, w, scale};
            SDL_Rect rt7 = {x + w, y + scale, scale, 2*h + scale};
            SDL_RenderFillRect(renderer, &t7);
            SDL_RenderFillRect(renderer, &rt7);
            break;
        case 8:
            SDL_Rect t8 = {x, y, w, scale};
            SDL_Rect m8 = {x, y + h, w, scale};
            SDL_Rect b8 = {x, y + 2*h, w, scale};
            SDL_Rect lt8t = {x - scale/2, y + scale, scale, h};
            SDL_Rect lt8b = {x - scale/2, y + h + scale, scale, h};
            SDL_Rect rt8t = {x + w, y + scale, scale, h};
            SDL_Rect rt8b = {x + w, y + h + scale, scale, h};
            SDL_RenderFillRect(renderer, &t8);
            SDL_RenderFillRect(renderer, &m8);
            SDL_RenderFillRect(renderer, &b8);
            SDL_RenderFillRect(renderer, &lt8t);
            SDL_RenderFillRect(renderer, &lt8b);
            SDL_RenderFillRect(renderer, &rt8t);
            SDL_RenderFillRect(renderer, &rt8b);
            break;
        case 9:
            SDL_Rect t9 = {x, y, w, scale};
            SDL_Rect m9 = {x, y + h, w, scale};
            SDL_Rect b9 = {x, y + 2*h, w, scale};
            SDL_Rect lt9 = {x - scale/2, y + scale, scale, h};
            SDL_Rect rt9t = {x + w, y + scale, scale, h};
            SDL_Rect rt9b = {x + w, y + h + scale, scale, h};
            SDL_RenderFillRect(renderer, &t9);
            SDL_RenderFillRect(renderer, &m9);
            SDL_RenderFillRect(renderer, &b9);
            SDL_RenderFillRect(renderer, &lt9);
            SDL_RenderFillRect(renderer, &rt9t);
            SDL_RenderFillRect(renderer, &rt9b);
            break;
    }
}

// Render game
void render(SDL_Renderer *renderer, GameState *state) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Draw center line
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < SCREEN_HEIGHT; y += 10) {
        SDL_Rect line = {SCREEN_WIDTH / 2 - 1, y, 2, 5};
        SDL_RenderFillRect(renderer, &line);
    }
    
    // Draw left paddle
    SDL_Rect left_rect = {
        (int)state->left_paddle.x,
        (int)state->left_paddle.y,
        PADDLE_WIDTH,
        PADDLE_HEIGHT
    };
    SDL_RenderFillRect(renderer, &left_rect);
    
    // Draw right paddle
    SDL_Rect right_rect = {
        (int)state->right_paddle.x,
        (int)state->right_paddle.y,
        PADDLE_WIDTH,
        PADDLE_HEIGHT
    };
    SDL_RenderFillRect(renderer, &right_rect);
    
    // Draw ball
    SDL_Rect ball_rect = {
        (int)(state->ball.x - BALL_SIZE / 2.0f),
        (int)(state->ball.y - BALL_SIZE / 2.0f),
        BALL_SIZE,
        BALL_SIZE
    };
    SDL_RenderFillRect(renderer, &ball_rect);
    
    // Draw scoreboard
    int left_ones = state->left_score % 10;
    int left_tens = state->left_score / 10;
    int right_ones = state->right_score % 10;
    int right_tens = state->right_score / 10;
    
    int scale = 4;
    
    if (left_tens > 0) {
        draw_digit(renderer, left_tens, SCREEN_WIDTH / 4 - 40, 30, scale);
    }
    draw_digit(renderer, left_ones, SCREEN_WIDTH / 4 + 20, 30, scale);
    
    if (right_tens > 0) {
        draw_digit(renderer, right_tens, 3 * SCREEN_WIDTH / 4 - 40, 30, scale);
    }
    draw_digit(renderer, right_ones, 3 * SCREEN_WIDTH / 4 + 20, 30, scale);
    
    // Present renderer
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL2 initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    GameState state;
    init_game(&state);
    
    SDL_Event event;
    int quit = 0;
    
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&state, keys);
        update(&state);
        render(renderer, &state);
        
        SDL_Delay(16); // ~60 FPS
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
