#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_FLAGS SDL_INIT_VIDEO
#define IMG_FLAGS IMG_INIT_PNG
#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#define RENDERER_SCALE_QUALITY "2"
#define RENDERER_SCALE_INT SDL_FALSE

#define WINDOW_TITLE "Random Colors"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_ICON "images/C-logo.png"

#endif
