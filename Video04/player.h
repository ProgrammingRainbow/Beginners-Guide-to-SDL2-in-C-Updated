#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"

struct Player {
        SDL_Renderer *renderer;
        SDL_Texture *image;
        SDL_Rect rect;
        const Uint8 *keystate;
};

void player_free(struct Player *p);
struct Player *player_new(SDL_Renderer *renderer);
void player_update(struct Player *p);
void player_draw(const struct Player *p);

#endif
