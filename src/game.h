#ifndef GAME_H
#define GAME_H

#include "fps.h"
#include "player.h"
#include "text.h"

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *background;
        struct Text *text;
        struct Text *fps_text;
        struct Text *x_text;
        struct Text *y_text;
        struct Player *player;
        struct Fps *fps;
        Mix_Chunk *c_sound;
        Mix_Chunk *sdl_sound;
        Mix_Music *music;
        SDL_Color bkgd_color;
        char str_buf[BUFFER_SIZE];
        SDL_Event event;
        bool is_running;
        bool show_fps;
        bool show_pos;
        bool enable_sound;
        float dt;
};

struct Game *game_new(void);
void game_free(struct Game *g);
bool game_run(struct Game *g);

#endif
