
#include <stdio.h>
#include <stdlib.h>

#include "obj_loader.h"
#include "config.h"
#include "quaternion_operations.h"
#include "draw3d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PI 3.14159265359


OBJ Load_OBJ(const char *obj_file_path) {
    FILE *fptr = fopen(obj_file_path, "r");
    char current_line[128];
    

    Vec3 *positions = malloc(MAX_TRIS * sizeof(Vec3));
    Vec3 *normals = malloc(MAX_TRIS * sizeof(Vec3));
    Vec2 *uvs = malloc(MAX_TRIS * sizeof(Vec2));


    int positionsIndex = 0;
    int normalsIndex = 0;
    int uvsIndex = 0;


    Triangle *triangles = malloc(MAX_TRIS * sizeof(Triangle));
    int triIndex = 0;


    // fgets advances fptr automatically
    while (fgets(current_line, sizeof(current_line), fptr)) {
        char char1 = current_line[0];
        char char2 = current_line[1];
        
        // vertex position
        // v -1.000000 1.000000 -1.000000
        if (char1 == 'v' && char2 == ' ') {
            
            float x, y, z;
            sscanf(current_line, "v %f %f %f", 
                &positions[positionsIndex].x,
                &positions[positionsIndex].y, 
                &positions[positionsIndex].z);


            positionsIndex++;

        // vertex normal
        // vn -0.0000 1.0000 -0.0000
        } else if (char1 == 'v' && char2 == 'n') {
            
            float nx, ny, nz;
            sscanf(current_line, "vn %f %f %f", 
                &normals[normalsIndex].x,
                &normals[normalsIndex].y,
                &normals[normalsIndex].z);

            
            normalsIndex++;

        // UV coordinate
        // vt 1.000000 0.000000
        } else if (char1 == 'v' && char2 == 't') {
            
            float u, v;
            sscanf(current_line, "vt %f %f",
            &uvs[uvsIndex].x,
            &uvs[uvsIndex].y);

            uvsIndex++;

        // f 5/1/1 8/4/2 4/2/2
        // f 5/1/1 --> for this vertex of this face, the position can be found as the 5th entry into "v "
        } else if (char1 == 'f') {
            int pos1, uv1, norm1, pos2, uv2, norm2, pos3, uv3, norm3;

            sscanf(current_line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &pos1, &uv1, &norm1, &pos2, &uv2, &norm2, &pos3, &uv3, &norm3);
            
            triangles[triIndex].v1.position = positions[pos1 - 1];
            triangles[triIndex].v1.uv = uvs[uv1 - 1];
            triangles[triIndex].v1.normal = normals[norm1 - 1];

            triangles[triIndex].v2.position = positions[pos2 - 1];
            triangles[triIndex].v2.uv = uvs[uv2 - 1];
            triangles[triIndex].v2.normal = normals[norm2 - 1];

            triangles[triIndex].v3.position = positions[pos3 - 1];
            triangles[triIndex].v3.uv = uvs[uv3 - 1];
            triangles[triIndex].v3.normal = normals[norm3 - 1];


            triIndex++;
        }
    }
    fclose(fptr);
    
    free(positions);
    free(normals);
    free(uvs);

    OBJ object = {
        .tris = triangles,
        .num_tris = triIndex
    };

    printf("Loaded OBJ: %s\n", obj_file_path);
    return object;
}

void Free_OBJ(OBJ *obj) {
    free(obj->tris);
}



void Render_OBJ(const OBJ *obj, TEX *texture_img, float pos_x, float pos_y, float pos_z, float scale_x, float scale_y, float scale_z, uint32_t *pixels) {

    QuatRotMat quat_rot_mat;
    
    if (rot_mat_defined) {

        float sin_half_theta = sinf((rot_mat_theta * PI/180)/2);
        float cos_half_theta = cosf((rot_mat_theta * PI/180)/2);
        
        Quat q = {
            .w = cos_half_theta,
            .i = sin_half_theta * rot_mat_nx,
            .j = sin_half_theta * rot_mat_ny,
            .k = sin_half_theta * rot_mat_nz
        };

        
        float w = q.w;
        float x = q.i;
        float y = q.j;
        float z = q.k;
        
        
        quat_rot_mat = (QuatRotMat){
            1.0f - 2.0f*(y*y + z*z), 2.0f*(x*y - w*z), 2.0f*(x*z + w*y),
            2.0f*(x*y + w*z), 1.0f - 2.0f*(x*x + z*z), 2.0f*(y*z - w*x),
            2.0f*(x*z - w*y), 2.0f*(y*z + w*x), 1.0f - 2.0f*(x*x + y*y)
        };

    }
    // SRT
    for (int i = 0; i < obj->num_tris; i++) {
        Triangle tri = obj->tris[i];

        tri.v1.position.x += pos_x;
        tri.v1.position.y += pos_y;
        tri.v1.position.z += pos_z;

        tri.v2.position.x += pos_x;
        tri.v2.position.y += pos_y;
        tri.v2.position.z += pos_z;

        tri.v3.position.x += pos_x;
        tri.v3.position.y += pos_y;
        tri.v3.position.z += pos_z;

        if (rot_mat_defined) {
            tri.v1.position.x -= rot_mat_px;
            tri.v2.position.x -= rot_mat_px;
            tri.v3.position.x -= rot_mat_px;

            tri.v1.position.y -= rot_mat_py;
            tri.v2.position.y -= rot_mat_py;
            tri.v3.position.y -= rot_mat_py;

            tri.v1.position.z -= rot_mat_pz;
            tri.v2.position.z -= rot_mat_pz;
            tri.v3.position.z -= rot_mat_pz;


            tri.v1.position = QuatRotate(tri.v1.position, quat_rot_mat);
            tri.v2.position = QuatRotate(tri.v2.position, quat_rot_mat);
            tri.v3.position = QuatRotate(tri.v3.position, quat_rot_mat);

            tri.v1.normal = QuatRotate(tri.v1.normal, quat_rot_mat);
            tri.v2.normal = QuatRotate(tri.v2.normal, quat_rot_mat);
            tri.v3.normal = QuatRotate(tri.v3.normal, quat_rot_mat);

            
            tri.v1.position.x += rot_mat_px;
            tri.v2.position.x += rot_mat_px;
            tri.v3.position.x += rot_mat_px;

            tri.v1.position.y += rot_mat_py;
            tri.v2.position.y += rot_mat_py;
            tri.v3.position.y += rot_mat_py;

            tri.v1.position.z += rot_mat_pz;
            tri.v2.position.z += rot_mat_pz;
            tri.v3.position.z += rot_mat_pz;
        }

        // scale up
        tri.v1.position.x *= scale_x;
        tri.v1.position.y *= scale_y;
        tri.v2.position.x *= scale_x;
        tri.v2.position.y *= scale_y;
        tri.v3.position.x *= scale_x;
        tri.v3.position.y *= scale_y;

        // center, shift
        tri.v1.position.x += (WIDTH / 2);
        tri.v1.position.y += (HEIGHT / 2);
        tri.v2.position.x += (WIDTH / 2);
        tri.v2.position.y += (HEIGHT / 2);
        tri.v3.position.x += (WIDTH / 2);
        tri.v3.position.y += (HEIGHT / 2);


        // too close
        if (tri.v1.position.z <= 0.1f || tri.v2.position.z <= 0.1f || tri.v3.position.z <= 0.1f) {
            continue;
        }
        RasterizeTriangle2d(tri, texture_img, pixels);
    }
}


TEX Load_Tex(const char *texture_path) {

    int width, height, channels;

    unsigned char *img = stbi_load(
        texture_path,
        &width,
        &height,
        &channels,
        4
    );

    if (!img) {
        fprintf(stderr, "\033[31mFailed to load texture: %s\033[0m\n", texture_path);
        TEX error_return = {0};

        return error_return;
    }
    uint32_t *texture_data = malloc(width * height * sizeof(uint32_t));

    for (int i = 0; i < width * height; i++) {
        unsigned char r = img[i * 4 + 0];
        unsigned char g = img[i * 4 + 1];
        unsigned char b = img[i * 4 + 2];
        unsigned char a = img[i * 4 + 3];

        texture_data[i] =
            ((uint32_t)a << 24 |
            (uint32_t)r << 16 |
            (uint32_t)g << 8 |
            (uint32_t)b);
    }

    TEX texture = {
        .ARGB = texture_data,
        .texture_width = width,
        .texture_height = height
    };

    stbi_image_free(img);

    printf("Loaded TEX: %s\n", texture_path);
    return texture;
}


void Free_Tex(TEX *Texture) {
    free(Texture->ARGB);

    // prevents double free
    Texture->ARGB = NULL;
}