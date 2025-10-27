#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SDL_FLAGS SDL_INIT_VIDEO
#define IMG_FLAGS IMG_INIT_PNG
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED

#define WINDOW_TITLE "Background and Icon"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_ICON "images/C-logo.png"

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *background;
        SDL_Event event;
        bool is_running;
};

void game_free(struct Game *g);
bool game_init_sdl(struct Game *g);
bool game_load_media(struct Game *g);
struct Game *game_new(void);
void game_events(struct Game *g);
void game_draw(struct Game *g);
void game_run(struct Game *g);

void game_free(struct Game *g) {
    if (g) {
        if (g->background) {
            SDL_DestroyTexture(g->background);
            g->background = NULL;
        }

        if (g->renderer) {
            SDL_DestroyRenderer(g->renderer);
            g->renderer = NULL;
        }

        if (g->window) {
            SDL_DestroyWindow(g->window);
            g->window = NULL;
        }

        SDL_Quit();

        free(g);
        g = NULL;

        puts("All Clean!");
    }
}

bool game_init_sdl(struct Game *g) {
    if (SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL2: %s\n", SDL_GetError());
        return false;
    }

    if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS) {
        fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
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

    SDL_Surface *icon_surf = IMG_Load(WINDOW_ICON);
    if (!icon_surf) {
        fprintf(stderr, "Error loading Surface: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetWindowIcon(g->window, icon_surf);
    SDL_FreeSurface(icon_surf);

    return true;
}

bool game_load_media(struct Game *g) {
    g->background = IMG_LoadTexture(g->renderer, "images/background.png");
    if (!g->background) {
        fprintf(stderr, "Error loading Texture: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

struct Game *game_new(void) {
    struct Game *g = calloc(1, sizeof(struct Game));
    if (g == NULL) {
        fprintf(stderr, "Error Calloc of New Game.\n");
        return NULL;
    }

    if (!game_init_sdl(g)) {
        game_free(g);
        return NULL;
    }

    if (!game_load_media(g)) {
        game_free(g);
        return NULL;
    }

    g->is_running = true;

    return g;
}

void game_events(struct Game *g) {
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
}

void game_draw(struct Game *g) {
    SDL_RenderClear(g->renderer);

    SDL_RenderCopy(g->renderer, g->background, NULL, NULL);

    SDL_RenderPresent(g->renderer);
}

void game_run(struct Game *g) {
    while (g->is_running) {
        game_events(g);

        game_draw(g);

        SDL_Delay(16);
    }
}

int main(void) {
    bool exit_status = EXIT_FAILURE;

    struct Game *game = game_new();
    if (game) {
        game_run(game);

        exit_status = EXIT_SUCCESS;
    }

    game_free(game);
    game = NULL;

    return exit_status;
}
