#include "text.h"
#include "bubble.h"

bool text_regen_image(struct Text *t);
void text_update_pos(struct Text *t);

void text_free(struct Text *t) {
    if (t) {
        if (t->font) {
            TTF_CloseFont(t->font);
            t->font = NULL;
        }

        if (t->image) {
            SDL_DestroyTexture(t->image);
            t->image = NULL;
        }

        t->renderer = NULL;

        free(t);
        t = NULL;

        printf("Free Text.\n");
    }
}

bool text_regen_image(struct Text *t) {
    if (t->image) {
        SDL_DestroyTexture(t->image);
        t->image = NULL;
    }

    SDL_Surface *text_surf =
        TTF_RenderText_Blended(t->font, t->text_str, t->inner_color);
    if (text_surf == NULL) {
        fprintf(stderr, "Error rendering text to Surface: %s\n",
                SDL_GetError());
        return false;
    }

    SDL_Surface *surf = NULL;

    if (t->bubble) {
        SDL_Surface *outer_surf =
            TTF_RenderText_Blended(t->font, t->text_str, t->outer_color);
        if (outer_surf == NULL) {
            fprintf(stderr, "Error rendering text to outer Surface: %s\n",
                    SDL_GetError());
            return false;
        }

        surf = bubble_two_surfaces(t->radius, text_surf, outer_surf);
        SDL_FreeSurface(text_surf);
        text_surf = NULL;
        if (surf == NULL) {
            return false;
        }
    } else {
        surf = text_surf;
    }

    t->rect.w = surf->w;
    t->rect.h = surf->h;

    t->image = SDL_CreateTextureFromSurface(t->renderer, surf);
    SDL_FreeSurface(surf);
    surf = NULL;
    if (!t->image) {
        fprintf(stderr, "Error creating Texture from Surface: %s\n",
                SDL_GetError());
        text_free(t);
        return false;
    }

    return true;
}

struct Text *text_new(SDL_Renderer *renderer, const char *font_file,
                      int font_size, const char *text_str, SDL_Color color) {
    struct Text *t = calloc(1, sizeof(struct Text));
    if (t == NULL) {
        fprintf(stderr, "Error Calloc of New Text.\n");
        return NULL;
    }

    t->renderer = renderer;
    t->font_size = font_size;
    t->inner_color = color;
    t->x_vel = TEXT_VEL;
    t->y_vel = TEXT_VEL;
    t->bubble = false;

    strncpy(t->text_str, text_str, BUFFER_SIZE - 1);
    t->text_str[BUFFER_SIZE - 1] = '\0';

    t->font = TTF_OpenFont(font_file, font_size);
    if (t->font == NULL) {
        fprintf(stderr, "Error Opening Font: %s\n", SDL_GetError());
        text_free(t);
        return NULL;
    }

    if (!text_regen_image(t)) {
        text_free(t);
        return NULL;
    }

    return t;
}

struct Text *text_bubble_new(SDL_Renderer *renderer, const char *font_file,
                             int font_size, const char *text_str, int radius,
                             SDL_Color inner_color, SDL_Color outer_color) {
    struct Text *t = calloc(1, sizeof(struct Text));
    if (t == NULL) {
        fprintf(stderr, "Error Calloc of New Text.\n");
        return NULL;
    }

    t->renderer = renderer;
    t->font_size = font_size;
    t->radius = radius;
    t->inner_color = inner_color;
    t->outer_color = outer_color;
    t->x_vel = TEXT_VEL;
    t->y_vel = TEXT_VEL;
    t->bubble = true;

    strncpy(t->text_str, text_str, BUFFER_SIZE - 1);
    t->text_str[BUFFER_SIZE - 1] = '\0';

    t->font = TTF_OpenFont(font_file, font_size);
    if (t->font == NULL) {
        fprintf(stderr, "Error Opening Font: %s\n", SDL_GetError());
        text_free(t);
        return NULL;
    }

    if (!text_regen_image(t)) {
        text_free(t);
        return NULL;
    }

    return t;
}

void text_update_pos(struct Text *t) {
    switch (t->anchor) {
    case TOP_RIGHT:
    case RIGHT:
    case BOTTOM_RIGHT:
        t->rect.x = (int)t->x - t->rect.w;
        break;
    case TOP:
    case BOTTOM:
    case CENTER:
        t->rect.x = (int)(t->x - (float)t->rect.w / 2);
        break;
    default:
        t->rect.x = (int)t->x;
        break;
    }

    switch (t->anchor) {
    case BOTTOM_LEFT:
    case BOTTOM:
    case BOTTOM_RIGHT:
        t->rect.y = (int)t->y - t->rect.h;
        break;
    case LEFT:
    case RIGHT:
    case CENTER:
        t->rect.y = (int)(t->y - (float)t->rect.h / 2);
        break;
    default:
        t->rect.y = (int)t->y;
        break;
    }
}

void text_set_pos(struct Text *t, float x, float y) {
    t->x = x;
    t->y = y;

    text_update_pos(t);
}

bool text_set_color(struct Text *t, SDL_Color inner_color) {
    t->inner_color = inner_color;

    if (!text_regen_image(t)) {
        return false;
    }

    return true;
}

bool text_set_colors(struct Text *t, SDL_Color inner_color,
                     SDL_Color outer_color) {
    t->inner_color = inner_color;
    t->outer_color = outer_color;

    if (!text_regen_image(t)) {
        return false;
    }

    return true;
}

bool text_set_bubble(struct Text *t, int radius, SDL_Color outer_color) {
    t->radius = radius;
    t->outer_color = outer_color;
    t->bubble = true;

    if (!text_regen_image(t)) {
        return false;
    }

    text_update_pos(t);

    return true;
}

bool text_unset_bubble(struct Text *t) {
    t->bubble = false;

    if (!text_regen_image(t)) {
        return false;
    }

    text_update_pos(t);

    return true;
}

void text_set_anchor(struct Text *t, enum RectAnchor anchor) {
    t->anchor = anchor;
}

bool text_set_str(struct Text *t, const char *new_str) {
    if (!strcmp(t->text_str, new_str)) {
        return true;
    }

    strncpy(t->text_str, new_str, BUFFER_SIZE - 1);
    t->text_str[BUFFER_SIZE - 1] = '\0';

    if (!text_regen_image(t)) {
        return false;
    }

    text_update_pos(t);

    return true;
}

bool text_update(struct Text *t, float dt) {
    bool collide = false;
    t->x += t->x_vel * dt;
    t->y += t->y_vel * dt;

    text_update_pos(t);

    if (t->rect.x + t->rect.w > WINDOW_WIDTH) {
        t->x_vel = -TEXT_VEL;
        collide = true;
    } else if (t->rect.x < 0) {
        t->x_vel = TEXT_VEL;
        collide = true;
    }
    if (t->rect.y + t->rect.h > WINDOW_HEIGHT) {
        t->y_vel = -TEXT_VEL;
        collide = true;
    } else if (t->rect.y < 0) {
        t->y_vel = TEXT_VEL;
        collide = true;
    }

    return collide;
}

void text_draw(const struct Text *t) {
    SDL_RenderCopy(t->renderer, t->image, NULL, &t->rect);
}
