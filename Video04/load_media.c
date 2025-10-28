#include "load_media.h"

bool game_load_media(struct Game *g) {
    g->background = IMG_LoadTexture(g->renderer, "images/background.png");
    if (g->background == NULL) {
        fprintf(stderr, "Error loading Texture: %s\n", IMG_GetError());
        return false;
    }

    return true;
}
