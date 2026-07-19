#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "draw2d.h"



typedef struct {
    Triangle *tris;
    int num_tris;
} OBJ;


typedef struct TEX {
    uint32_t *ARGB;
    int texture_width, texture_height;
} TEX;


OBJ Load_OBJ(const char *obj_file_path);
void Free_OBJ(OBJ *obj);
void Render_OBJ(const OBJ *obj, TEX *texture_img, float pos_x, float pos_y, float pos_z, float scale_x, float scale_y, float scale_z, uint32_t *pixels);
TEX Load_Tex(const char *texture_path);
void Free_Tex(TEX *Texture);

#endif