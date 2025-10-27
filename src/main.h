#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_FLAGS SDL_INIT_EVERYTHING
#define IMG_FLAGS IMG_INIT_PNG
#define MIX_FLAGS MIX_INIT_OGG
#define CHUNK_SIZE 1024
#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#define RENDERER_SCALE_QUALITY "2"
#define RENDERER_SCALE_INT SDL_FALSE

#define WINDOW_TITLE "Music & Sound"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_ICON "images/C-logo.png"

#define WHITE_COLOR (SDL_Color){255, 255, 255, 255}
#define BLACK_COLOR (SDL_Color){0, 0, 0, 255}
#define BLUE_COLOR (SDL_Color){137, 180, 250, 255}
#define BUBBLE_RADIUS 15

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TEXT_FONT "fonts/freesansbold.ttf"
#define TEXT_SIZE 100
#define TEXT_STR "SDL"
#define TEXT_VEL 180

#define PLAYER_VEL 300

#define FPS_TARGET 100000
#define FPS_MAX_DURATION 50

enum RectAnchor {
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM,
    BOTTOM_RIGHT,
    LEFT,
    RIGHT,
    CENTER
};

#endif
