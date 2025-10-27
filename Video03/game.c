#include "game.h"
#include "init_sdl.h"
#include "load_media.h"

void game_render_color(struct Game *g);
void game_events(struct Game *g);
void game_draw(struct Game *g);

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

        printf("All Clean!\n");
    }
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

    srand((unsigned)time(NULL));

    return g;
}

void game_render_color(struct Game *g) {
    g->bkgd_color.r = (Uint8)rand();
    g->bkgd_color.g = (Uint8)rand();
    g->bkgd_color.b = (Uint8)rand();
    g->bkgd_color.a = 255;
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
            case SDL_SCANCODE_SPACE:
                game_render_color(g);
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
    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->renderer);

    SDL_SetRenderDrawColor(g->renderer, g->bkgd_color.r, g->bkgd_color.g,
                           g->bkgd_color.b, 255);
    SDL_RenderFillRect(g->renderer, NULL);

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
