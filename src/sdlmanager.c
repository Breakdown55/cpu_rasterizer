#include <SDL3/SDL.h> 
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#include "draw2d.h"
#include "config.h"
#include "stb_image.h"

static SDL_Window *win = NULL;
static SDL_Renderer *ren = NULL;
static SDL_Texture *tex = NULL;

void CreateWindow(int screen_width, int screen_height, char *window_name) {

    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow(window_name, screen_width, screen_height, 0);
    ren = SDL_CreateRenderer(win, NULL);
    tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888,
                            SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);

}


void PushToFrameBuffer(uint32_t *pixels, int screen_width) {
    
    SDL_UpdateTexture(tex, NULL, pixels, screen_width * sizeof(uint32_t));  // send to gpu
    SDL_RenderTexture(ren, tex, NULL, NULL);   // write to back buffer
    SDL_RenderPresent(ren); // flip back buffer to frame buffer
}


void ClearBuffer(uint32_t *pixels, RGBA color) {
    uint32_t pixel_color = mapSDLcolor(color);

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        pixels[i] = pixel_color;
    }

    memset(Zbuffer, 0, WIDTH * HEIGHT * sizeof(float));
}

// wait for poll event to be a close event
bool NoneExitingPollEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT)
            return false;
    }
    return true;
}



static uint64_t last_time = 0;

// SDL_GetPerformanceCounter can have ns accuracy
float GetDTs() {
    static uint64_t last_time = 0;

    uint64_t current_time = SDL_GetPerformanceCounter();

    if (last_time == 0) {
        last_time = current_time;
        return 0.0f;
    }

    float dt =
        (float)(current_time - last_time) /
        (float)SDL_GetPerformanceFrequency();

    last_time = current_time;

    return dt;
}


float GetDTms() {
    static uint64_t last_time = 0;

    uint64_t current_time = SDL_GetPerformanceCounter();

    if (last_time == 0) {
        last_time = current_time;
        return 0.0f;
    }

    float dt =
        (float)(current_time - last_time) * 1000.0f /
        (float)SDL_GetPerformanceFrequency();

    last_time = current_time;

    return dt;
}