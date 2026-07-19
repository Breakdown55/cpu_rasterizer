#ifndef VECTOR_OPERATIONS_H
#define VECTOR_OPERATIONS_H

#include "draw2d.h"


Vec3 NormalizeVec3(Vec3 v);
Vec3 NormalizeVec3f(float x, float y, float z);
float DotProdVec3(Vec3 v1, Vec3 v2);
float DotProdVec3_f(Vec3 v1, float x2, float y2, float z2);


#endif