#ifndef SDLMANAGER_H
#define SDLMANAGER_H


#include "draw2d.h"

void CreateWindow(int screen_width, int screen_height, char *window_name);
void PushToFrameBuffer(uint32_t *pixels, int screen_width);
bool NoneExitingPollEvents();
void ClearBuffer(uint32_t *pixels, RGBA color);
float GetDTs();
float GetDTms();

#endif