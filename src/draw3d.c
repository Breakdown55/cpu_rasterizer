#include <stdbool.h>


#include "draw2d.h"
#include "quaternion_operations.h"
#include "draw3d.h"
#include "vector_operations.h"



float rot_mat_theta = 0.0f;
float rot_mat_nx = 0.0f;
float rot_mat_ny = 0.0f;
float rot_mat_nz = 0.0f;
float rot_mat_px = 0.0f; 
float rot_mat_py = 0.0f;
float rot_mat_pz = 0.0f;


bool rot_mat_defined = false;

CULLING_MODE culling_mode = CULL_NONE;

void SetCullingMode(int cull_mode) {
    culling_mode = cull_mode;
}

/// @brief Creates a rotation matrix applicable to every 3D drawing until END_RotMat() is called
void BEGIN_RotMat(float theta, float px, float py, float pz, float nx, float ny, float nz) {
    rot_mat_defined = true;

    rot_mat_theta = theta;

    rot_mat_px = px;
    rot_mat_py = py;
    rot_mat_pz = pz;

    Vec3 normalized_rot_mat_normal = NormalizeVec3f(nx, ny, nz);
    rot_mat_nx = normalized_rot_mat_normal.x;
    rot_mat_ny = normalized_rot_mat_normal.y;
    rot_mat_nz = normalized_rot_mat_normal.z;

    
}

void END_RotMat() {
    rot_mat_defined = false;
}




RGBA blue = {0, 0, 255, 255};
