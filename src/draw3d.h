#ifndef DRAW3D_h
#define DRAW3D_h

#include "draw2d.h"

typedef enum {
    CULL_NONE,
    CULL_BACK,
} CULLING_MODE;

extern CULLING_MODE culling_mode;

void SetCullingMode(int cull_mode);

extern bool rot_mat_defined;

extern float rot_mat_theta;

extern float rot_mat_px;
extern float rot_mat_py;
extern float rot_mat_pz;

extern float rot_mat_nx;
extern float rot_mat_ny;
extern float rot_mat_nz;



void BEGIN_RotMat(float theta, float px, float py, float pz, float nx, float ny, float nz);
void END_RotMat();


#endif