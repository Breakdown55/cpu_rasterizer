#include <math.h>

#include "draw2d.h"
#include "vector_operations.h"
#include "vector_operations.h"

Vec3 NormalizeVec3(Vec3 v) {
    float magnitude = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

    float inv_mag = 1 / magnitude;

    Vec3 normalized_vector = {v.x * inv_mag, v.y * inv_mag, v.z * inv_mag};

    return normalized_vector;

}

Vec3 NormalizeVec3f(float x, float y, float z) {
    float magnitude = sqrtf((x * x) + (y * y) + (z * z));

    float inv_mag = 1 / magnitude;

    Vec3 normalized_vector = {x * inv_mag, y * inv_mag, z * inv_mag};

    return normalized_vector;

}

float DotProdVec3_Vec3(Vec3 v1, Vec3 v2) {
    Vec3 nv1 = NormalizeVec3(v1);
    Vec3 nv2 = NormalizeVec3(v2);
    
    float dot = (nv1.x * nv2.x) + (nv1.y * nv2.y) + (nv1.z * nv2.z);

    return dot;
}

float DotProdVec3_f(Vec3 v1, float x2, float y2, float z2) {
    Vec3 nv1 = NormalizeVec3(v1);
    
    Vec3 v2 = {x2, y2, z2};

    Vec3 nv2 = NormalizeVec3(v2);
    
    float dot = (nv1.x * nv2.x) + (nv1.y * nv2.y) + (nv1.z * nv2.z);

    return dot;
}