#include "game.h"
#include "init_sdl.h"
#include "load_media.h"
#include <SDL2/SDL_scancode.h>

void game_render_color(struct Game *g);
bool game_toggle_music(struct Game *g);
bool game_events(struct Game *g);
void game_update(struct Game *g);
void game_draw(const struct Game *g);

void game_free(struct Game *g) {
    if (g) {
        Mix_HaltMusic();
        Mix_HaltChannel(-1);

        if (g->music) {
            Mix_FreeMusic(g->music);
            g->music = NULL;
        }
        if (g->sdl_sound) {
            Mix_FreeChunk(g->sdl_sound);
            g->sdl_sound = NULL;
        }
        if (g->c_sound) {
            Mix_FreeChunk(g->c_sound);
            g->c_sound = NULL;
        }

        if (g->player) {
            player_free(g->player);
            g->player = NULL;
        }

        if (g->text) {
            text_free(g->text);
            g->text = NULL;
        }

        if (g->fps_text) {
            text_free(g->fps_text);
            g->fps_text = NULL;
        }

        if (g->x_text) {
            text_free(g->x_text);
            g->x_text = NULL;
        }

        if (g->y_text) {
            text_free(g->y_text);
            g->y_text = NULL;
        }

        if (g->fps) {
            fps_free(g->fps);
            g->fps = NULL;
        }

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

        Mix_CloseAudio();

        Mix_Quit();
        TTF_Quit();
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

    // g->text =
    //     text_new(g->renderer, TEXT_FONT, TEXT_SIZE, TEXT_STR, WHITE_COLOR);
    g->text = text_bubble_new(g->renderer, TEXT_FONT, TEXT_SIZE, TEXT_STR,
                              BUBBLE_RADIUS, BLUE_COLOR, WHITE_COLOR);
    if (g->text == NULL) {
        game_free(g);
        return NULL;
    }

    g->fps_text = text_bubble_new(g->renderer, TEXT_FONT, TEXT_SIZE, "FPS: 0",
                                  BUBBLE_RADIUS, BLACK_COLOR, WHITE_COLOR);
    if (g->fps_text == NULL) {
        game_free(g);
        return NULL;
    }
    text_set_anchor(g->fps_text, TOP);
    text_set_pos(g->fps_text, WINDOW_WIDTH / 2.0f, 10);

    g->x_text = text_bubble_new(g->renderer, TEXT_FONT, TEXT_SIZE, "X: 0",
                                BUBBLE_RADIUS, BLACK_COLOR, WHITE_COLOR);
    if (g->x_text == NULL) {
        game_free(g);
        return NULL;
    }
    text_set_anchor(g->x_text, TOP_LEFT);
    text_set_pos(g->x_text, 10, 10);

    g->y_text = text_bubble_new(g->renderer, TEXT_FONT, TEXT_SIZE, "Y: 0",
                                BUBBLE_RADIUS, BLACK_COLOR, WHITE_COLOR);
    if (g->y_text == NULL) {
        game_free(g);
        return NULL;
    }
    text_set_anchor(g->y_text, TOP_RIGHT);
    text_set_pos(g->y_text, WINDOW_WIDTH - 10, 10);

    g->player = player_new(g->renderer);
    if (g->player == NULL) {
        game_free(g);
        return NULL;
    }

    g->fps = fps_new();
    if (g->fps == NULL) {
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

    if (g->enable_sound) {
        Mix_PlayChannel(-1, g->c_sound, 0);
    }
}

bool game_toggle_music(struct Game *g) {
    if (Mix_PlayingMusic()) {
        if (Mix_PausedMusic()) {
            Mix_ResumeMusic();
        } else {
            Mix_PauseMusic();
        }
    } else {
        if (Mix_PlayMusic(g->music, -1)) {
            fprintf(stderr, "Error playing Music: %s\n", SDL_GetError());
            return false;
        }
    }

    return true;
}

bool game_events(struct Game *g) {
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
            case SDL_SCANCODE_M:
                if (!game_toggle_music(g)) {
                    return false;
                }
                break;
            case SDL_SCANCODE_N:
                g->enable_sound = !g->enable_sound;
                break;
            case SDL_SCANCODE_F:
                g->show_fps = !g->show_fps;
                break;
            case SDL_SCANCODE_P:
                g->show_pos = !g->show_pos;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    return true;
}

void game_update(struct Game *g) {
    if (text_update(g->text, g->dt)) {
        if (g->enable_sound) {
            Mix_PlayChannel(-1, g->sdl_sound, 0);
        }
    }
    player_update(g->player, g->dt);

    if (g->show_fps) {
        snprintf(g->str_buf, sizeof(g->str_buf), "FPS: %d", g->fps->fps);
        text_set_str(g->fps_text, g->str_buf);
    }

    if (g->show_pos) {
        snprintf(g->str_buf, sizeof(g->str_buf), "X: %d", (int)g->text->x);
        text_set_str(g->x_text, g->str_buf);
        snprintf(g->str_buf, sizeof(g->str_buf), "Y: %d", (int)g->text->y);
        text_set_str(g->y_text, g->str_buf);
    }
}

void game_draw(const struct Game *g) {
    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->renderer);

    SDL_SetRenderDrawColor(g->renderer, g->bkgd_color.r, g->bkgd_color.g,
                           g->bkgd_color.b, 255);
    SDL_RenderFillRect(g->renderer, NULL);

    SDL_RenderCopy(g->renderer, g->background, NULL, NULL);
    if (g->show_fps) {
        text_draw(g->fps_text);
    }
    if (g->show_pos) {
        text_draw(g->x_text);
        text_draw(g->y_text);
    }
    player_draw(g->player);
    text_draw(g->text);

    SDL_RenderPresent(g->renderer);
}

bool game_run(struct Game *g) {
    while (g->is_running) {
        if (!game_events(g)) {
            return false;
        }

        game_update(g);

        game_draw(g);

        g->dt = fps_update(g->fps);
    }

    return true;
}
