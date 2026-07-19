#include "quaternion_operations.h"
#include "draw2d.h"
#include <math.h>

#define PI 3.14159265359


Quat QuatMult(Quat q, Quat p) {
    // p' = q * p * q⁻¹
    // q * p
    float w_qp = q.w*p.w - q.i*p.i - q.j*p.j - q.k*p.k;
    float x_qp = q.w*p.i + q.i*p.w + q.j*p.k - q.k*p.j;
    float y_qp = q.w*p.j - q.i*p.k + q.j*p.w + q.k*p.i;
    float z_qp = q.w*p.k + q.i*p.j - q.j*p.i + q.k*p.w;

    Quat resultant = {
        .w = w_qp,
        .i = x_qp,
        .j = y_qp,
        .k = z_qp
    };

    return resultant;
}


Vec3 QuatRotate(Vec3 point, QuatRotMat rot_mat) {

    Vec3 rotated_point = {
        .x = rot_mat.r00*point.x + rot_mat.r01*point.y + rot_mat.r02*point.z,
        .y = rot_mat.r10*point.x + rot_mat.r11*point.y + rot_mat.r12*point.z,
        .z = rot_mat.r20*point.x + rot_mat.r21*point.y + rot_mat.r22*point.z
    };

    return rotated_point;

}

Quat QuatAdd(Quat q1, Quat q2) {
    Quat resultant_quat = {
        .w = q1.w + q2.w,
        .i = q1.i + q2.i,
        .j = q1.j + q2.j,
        .k = q1.k + q2.k
    };

    return resultant_quat;
}

Quat QuatScale(Quat q, float scale) {
    Quat scaled_quat = {
        .w = q.w * scale,
        .i = q.i * scale,
        .j = q.j * scale,
        .k = q.k * scale
    };

    return scaled_quat;
}


float QuatDot(Quat q1, Quat q2) {
    return q1.w * q2.w + q1.i * q2.i + q1.j * q2.j + q1.k * q2.k;
}