#ifndef QUATERNION_OPERATIONS_H
#define QUATERNION_OPERATIONS_H

#include "draw2d.h"

typedef struct {
    float w;
    float i, j, k;
} Quat;



typedef struct {
    float r00, r01, r02;
    float r10, r11, r12;
    float r20, r21, r22;
} QuatRotMat;


Quat QuatMult(Quat q, Quat p);
Vec3 QuatRotate(Vec3 point, QuatRotMat rot_mat);
Quat QuatAdd(Quat q1, Quat q2);
Quat QuatScale(Quat q, float scale);
float QuatDot(Quat q1, Quat q2);


#endif