#include "player.h"
#include "bubble.h"
#include "main.h"

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

    // p->image = IMG_LoadTexture(p->renderer, "images/C-logo.png");
    // if (!p->image) {
    //     fprintf(stderr, "Error loading Texture: %s\n", SDL_GetError());
    //     player_free(p);
    //     return NULL;
    // }

    p->image = bubble_texture(renderer, "images/C-logo.png", BUBBLE_RADIUS,
                              WHITE_COLOR);
    if (!p->image) {
        player_free(p);
        return NULL;
    }

    if (SDL_QueryTexture(p->image, NULL, NULL, &p->rect.w, &p->rect.h)) {
        fprintf(stderr, "Error querying Texture Size: %s\n", SDL_GetError());
        player_free(p);
        return NULL;
    }

    p->keystate = SDL_GetKeyboardState(NULL);

    // if (!SDL_SetTextureScaleMode(p->image, SDL_SCALEMODE_NEAREST)) {
    //     fprintf(stderr, "Error setting texture scale mode: %s\n",
    //             SDL_GetError());
    //     player_free(p);
    //     return NULL;
    // }

    return p;
}

void player_update(struct Player *p, float dt) {
    if (p->keystate[SDL_SCANCODE_LEFT] || p->keystate[SDL_SCANCODE_A]) {
        p->x -= PLAYER_VEL * dt;
        p->rect.x = (int)p->x;
    }
    if (p->keystate[SDL_SCANCODE_RIGHT] || p->keystate[SDL_SCANCODE_D]) {
        p->x += PLAYER_VEL * dt;
        p->rect.x = (int)p->x;
    }
    if (p->keystate[SDL_SCANCODE_UP] || p->keystate[SDL_SCANCODE_W]) {
        p->y -= PLAYER_VEL * dt;
        p->rect.y = (int)p->y;
    }
    if (p->keystate[SDL_SCANCODE_DOWN] || p->keystate[SDL_SCANCODE_S]) {
        p->y += PLAYER_VEL * dt;
        p->rect.y = (int)p->y;
    }
}

void player_draw(const struct Player *p) {
    SDL_RenderCopy(p->renderer, p->image, NULL, &p->rect);
}
