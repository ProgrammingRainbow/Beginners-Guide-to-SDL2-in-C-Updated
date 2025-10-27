#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SDL_FLAGS SDL_INIT_VIDEO
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED

#define WINDOW_TITLE "Open Window"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Event event;
        bool is_running;
};

void game_free(struct Game *g);
bool game_init_sdl(struct Game *g);
void game_run(struct Game *g);

void game_free(struct Game *g) {
    if (g) {
        if (g->renderer) {
            SDL_DestroyRenderer(g->renderer);
            g->renderer = NULL;
        }

        if (g->window) {
            SDL_DestroyWindow(g->window);
            g->window = NULL;
        }

        SDL_Quit();

        puts("All Clean!");
    }
}

bool game_init_sdl(struct Game *g) {
    if (SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL2: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                 WINDOW_HEIGHT, WINDOW_FLAGS);
    if (!g->window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, -1, RENDERER_FLAGS);
    if (!g->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void game_run(struct Game *g) {
    g->is_running = true;

    while (g->is_running) {
        while (SDL_PollEvent(&g->event)) {
            switch (g->event.type) {
            case SDL_QUIT:
                g->is_running = false;
                break;
            case SDL_KEYDOWN:
                switch (g->event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    g->is_running = false;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        SDL_RenderClear(g->renderer);

        SDL_RenderPresent(g->renderer);

        SDL_Delay(16);
    }
}

int main(void) {
    bool exit_status = EXIT_FAILURE;

    struct Game game = {0};

    if (game_init_sdl(&game)) {
        game_run(&game);

        exit_status = EXIT_SUCCESS;
    }

    game_free(&game);

    return exit_status;
}
