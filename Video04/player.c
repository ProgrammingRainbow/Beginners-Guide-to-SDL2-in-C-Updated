#include "player.h"

void player_free(struct Player *p) {
    if (p) {
        if (p->image) {
            SDL_DestroyTexture(p->image);
            p->image = NULL;
        }

        p->renderer = NULL;
        p->keystate = NULL;

        free(p);
        p = NULL;

        printf("Free Player.\n");
    }
}

struct Player *player_new(SDL_Renderer *renderer) {
    struct Player *p = calloc(1, sizeof(struct Player));
    if (p == NULL) {
        fprintf(stderr, "Error Calloc of New Player.\n");
        return NULL;
    }

    p->renderer = renderer;

    p->image = IMG_LoadTexture(p->renderer, "images/C-logo.png");
    if (!p->image) {
        fprintf(stderr, "Error loading Texture: %s\n", SDL_GetError());
        player_free(p);
        p = NULL;
        return NULL;
    }

    if (SDL_QueryTexture(p->image, NULL, NULL, &p->rect.w, &p->rect.h)) {
        fprintf(stderr, "Error querying Texture Size: %s\n", SDL_GetError());
        player_free(p);
        p = NULL;
        return NULL;
    }

    p->keystate = SDL_GetKeyboardState(NULL);

    return p;
}

void player_update(struct Player *p) {
    int x_dir = 0;
    int y_dir = 0;

    if (p->keystate[SDL_SCANCODE_LEFT] || p->keystate[SDL_SCANCODE_A]) {
        x_dir = -1;
    }
    if (p->keystate[SDL_SCANCODE_RIGHT] || p->keystate[SDL_SCANCODE_D]) {
        x_dir = 1;
    }
    if (p->keystate[SDL_SCANCODE_UP] || p->keystate[SDL_SCANCODE_W]) {
        y_dir = -1;
    }
    if (p->keystate[SDL_SCANCODE_DOWN] || p->keystate[SDL_SCANCODE_S]) {
        y_dir = 1;
    }

    if (x_dir && y_dir) {
        p->rect.x += x_dir * (int)(PLAYER_VEL * NORMALIZED_45);
        p->rect.y += y_dir * (int)(PLAYER_VEL * NORMALIZED_45);
    } else {
        p->rect.x += x_dir * PLAYER_VEL;
        p->rect.y += y_dir * PLAYER_VEL;
    }
}

void player_draw(const struct Player *p) {
    SDL_RenderCopy(p->renderer, p->image, NULL, &p->rect);
}
