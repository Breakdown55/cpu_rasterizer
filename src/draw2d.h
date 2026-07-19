#ifndef DRAW2D_H
#define DRAW2D_H

#include <stdint.h>
#include <stdio.h>


typedef struct TEX TEX;

extern uint32_t pixels[];
extern float Zbuffer[];

#define PI 3.14159265359

typedef struct {
    float x, y, z;    
} Vec3;


typedef struct {
    float x, y;    
} Vec2;

typedef struct {
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
} Vertex;

typedef struct {
    Vertex v1, v2, v3;
} Triangle;


typedef struct {
    uint8_t R, G, B, A;
} RGBA;


void DrawCornerRectangle(int x, int y, int width, int height, RGBA color, uint32_t *pixels);
void DrawCenterRectangle(int center_x, int center_y, int width, int height, RGBA color, uint32_t *pixels);
uint32_t mapSDLcolor(RGBA color);
void DrawPixel(int x, int y, RGBA color, uint32_t *pixels);
Vec3 v(float x, float y, float z);
void DrawCircle(int center_x, int center_y, int radius, RGBA color, uint32_t *pixels);
void DrawVLine(int x1, int y1, int length, RGBA color, uint32_t *pixels);
void DrawHLine(int x1, int y1, int length, RGBA color, uint32_t *pixels);
void DrawLine(int x1, int y1, int x2, int y2, int thickness, RGBA color, uint32_t *pixels);
uint32_t UV_to_ARGB(float w1, float w2, float w3, Vec3 v1_normal, Vec3 v2_normal, Vec3 v3_normal, float depth, float reciprocal_Z1, float reciprocal_Z2, float reciprocal_Z3, float edge1, float edge2, float edge3, Vec2 v1_uv, Vec2 v2_uv, Vec2 v3_uv, TEX *texture);
void RasterizeTriangle2d(Triangle tri, TEX *texture, uint32_t *pixels);
void ClearZBuffer();

#endif