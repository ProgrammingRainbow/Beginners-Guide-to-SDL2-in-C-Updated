#ifndef GAME_H
#define GAME_H

#include "main.h"

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *background;
        SDL_Event event;
        bool is_running;
};

void game_free(struct Game *g);
struct Game *game_new(void);
void game_run(struct Game *g);

#endif
