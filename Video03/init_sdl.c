#include "init_sdl.h"

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

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, RENDERER_SCALE_QUALITY);

    g->renderer = SDL_CreateRenderer(g->window, -1, RENDERER_FLAGS);
    if (!g->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_RenderSetLogicalSize(g->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_RenderSetIntegerScale(g->renderer, RENDERER_SCALE_INT);

    SDL_Surface *icon_surf = IMG_Load(WINDOW_ICON);
    if (!icon_surf) {
        fprintf(stderr, "Error loading Surface: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetWindowIcon(g->window, icon_surf);
    SDL_FreeSurface(icon_surf);

    return true;
}
