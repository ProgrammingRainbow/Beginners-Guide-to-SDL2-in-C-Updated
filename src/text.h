#ifndef TEXT_H
#define TEXT_H

#include "main.h"

#define BUFFER_SIZE 512

struct Text {
        SDL_Renderer *renderer;
        TTF_Font *font;
        SDL_Texture *image;
        SDL_Rect rect;
        float x;
        float y;
        float x_vel;
        float y_vel;
        int font_size;
        char text_str[BUFFER_SIZE];
        enum RectAnchor anchor;
        int radius;
        SDL_Color inner_color;
        SDL_Color outer_color;
        bool bubble;
};

void text_free(struct Text *t);
struct Text *text_new(SDL_Renderer *renderer, const char *font_file,
                      int font_size, const char *text_str, SDL_Color color);
struct Text *text_bubble_new(SDL_Renderer *renderer, const char *font_file,
                             int font_size, const char *text_str, int radius,
                             SDL_Color inner_color, SDL_Color outer_color);
void text_set_pos(struct Text *t, float x, float y);
bool text_set_color(struct Text *t, SDL_Color inner_color);
bool text_set_colors(struct Text *t, SDL_Color inner_color,
                     SDL_Color outer_color);
bool text_set_bubble(struct Text *t, int radius, SDL_Color outer_color);
bool text_unset_bubble(struct Text *t);
void text_set_anchor(struct Text *t, enum RectAnchor anchor);
bool text_set_str(struct Text *t, const char *new_str);
bool text_update(struct Text *t, float dt);
void text_draw(const struct Text *t);

#endif
