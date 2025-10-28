#ifndef GAME_H
#define GAME_H

#include "player.h"

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *background;
        struct Player *player;
        SDL_Event event;
        bool is_running;
        SDL_Color bkgd_color;
};

void game_free(struct Game *g);
struct Game *game_new(void);
void game_run(struct Game *g);

#endif
