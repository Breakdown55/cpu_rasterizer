#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "utils.h"
#include "config.h"
#include "draw2d.h"
#include "draw3d.h"
#include "quaternion_operations.h"
#include "obj_loader.h"
#include "vector_operations.h"
#include "lighting.h"




extern uint32_t pixels[];



Vec3 v(float x, float y, float z) {
    Vec3 v = {x, y, z};
    return v;
}

uint32_t mapSDLcolor(RGBA color) {
    // AAAAAAAA RRRRRRRR GGGGGGGG BBBBBBBB

    uint32_t converted_color = ((uint32_t)color.A << 24) | ((uint32_t)color.R << 16) | ((uint32_t)color.G << 8) | ((uint32_t)color.B);
    
    return converted_color;
}


void DrawPixel(int x, int y, RGBA color, uint32_t *pixels) {
    pixels[y * WIDTH + x] = mapSDLcolor(color);
}

void DrawCornerRectangle(int x, int y, int width, int height, RGBA color, uint32_t *pixels) {
    uint32_t pixel_color = mapSDLcolor(color);

    for (int i = y; i < y + height; i++) {
        uint32_t *row = &pixels[i * WIDTH];

        for (int j = x; j < x + width; j++) {
            row[j] = pixel_color;
        }
    }
}


void DrawCenterRectangle(int center_x, int center_y, int width, int height, RGBA color, uint32_t *pixels) {
    DrawCornerRectangle(center_x - width/2, center_y - height/2, width, height, color, pixels);
}



void DrawCircle(int center_x, int center_y, int radius, RGBA color, uint32_t *pixels) {
    uint32_t pixel_color = mapSDLcolor(color);

    int r2 = radius*radius;

    int min_x, max_x;

    int min_y = center_y - radius; 
    int max_y = center_y + radius;

    int dx, dy;

    for (int y = min_y; y <= max_y; y++) {
        uint32_t *row = &pixels[y * WIDTH];
        
        dy = y - center_y;
        dx = sqrtf(r2 - dy*dy);

        max_x = center_x + dx;
        min_x = center_x - dx;

        min_x = max_i(0, min_x);
        max_x = min_i(max_x, WIDTH - 1);
        
        if (y < 0 || y > HEIGHT) {
            continue;
        }

        for (int x = min_x; x <= max_x; x++) {
            row[x] = pixel_color;
        }
    }
}


void DrawVLine(int x1, int y1, int length, RGBA color, uint32_t *pixels) {
    uint32_t pixel_color = mapSDLcolor(color);

    for (int y = y1; y < y1 + length; y++) {
        pixels[WIDTH * y + x1] = pixel_color;
    }
}


void DrawHLine(int x1, int y1, int length, RGBA color, uint32_t *pixels) {
    uint32_t pixel_color = mapSDLcolor(color);
    uint32_t *row = &pixels[y1 * WIDTH];

    for (int x = x1; x < x1 + length; x++) {
        row[x] = pixel_color;
    }
}

// TODO implement brehenshams
void DrawLine(int x1, int y1, int x2, int y2, int thickness, RGBA color, uint32_t *pixels) {
    uint32_t pixel_color = mapSDLcolor(color);

    
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = max_i(abs(dx), abs(dy));    // pixels the longer axis spans
    if (steps == 0) steps = 1;              // avoid div by 0

    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++) {
        int pixel_x = (int)roundf(x);
        int pixel_y = (int)roundf(y);

        for (int offset_y = -thickness; offset_y <= thickness; offset_y++) {
            for (int offset_x = -thickness; offset_x <= thickness; offset_x++) {
                int final_x = pixel_x + offset_x;
                int final_y = pixel_y + offset_y;
                
                if (final_x >= 0 && final_x < WIDTH && final_y >= 0 && final_y < HEIGHT) {
                    pixels[final_y * WIDTH + final_x] = pixel_color;
                }
                
            }
        }
        x += x_inc;
        y += y_inc;
        
    }
    
}






float Zbuffer[WIDTH * HEIGHT];

void ClearZBuffer() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        Zbuffer[i] = 0.0f;
    }
}

void RasterizeTriangle2d(Triangle tri, TEX *texture, uint32_t *pixels) {
    float cx = WIDTH / 2;
    float cy = HEIGHT / 2;

    // translate, divide, translate back
    float x3 = ((tri.v1.position.x - cx) * FOCAL_LENGTH / tri.v1.position.z) + cx;
    float y3 = (-(tri.v1.position.y - cy) * FOCAL_LENGTH / tri.v1.position.z) + cy;

    float x2 = ((tri.v2.position.x - cx) * FOCAL_LENGTH / tri.v2.position.z) + cx;
    float y2 = (-(tri.v2.position.y - cy) * FOCAL_LENGTH / tri.v2.position.z) + cy;

    float x1 = ((tri.v3.position.x - cx) * FOCAL_LENGTH / tri.v3.position.z) + cx;
    float y1 = (-(tri.v3.position.y - cy) * FOCAL_LENGTH / tri.v3.position.z) + cy;
    


    float cross_product = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
    int sign = cross_product > 0 ? 1 : -1;

    // cull w/ winding
    
    if (cross_product > 0 && culling_mode == CULL_BACK) {
        return;
    } else if (cross_product == 0) {
        return;
    }
    


    // find bounding box
    float x_min = fminf(fminf(x1, x2), x3);
    float x_max = fmaxf(fmaxf(x1, x2), x3);
    float y_min = fminf(fminf(y1, y2), y3);
    float y_max = fmaxf(fmaxf(y1, y2), y3);

    // clamp
    x_min = (int)floorf(clamp_f(x_min, 0, WIDTH - 1));
    x_max = (int)ceilf(clamp_f(x_max, 0, WIDTH - 1));
    y_min = (int)floorf(clamp_f(y_min, 0, HEIGHT - 1));
    y_max = (int)ceilf(clamp_f(y_max, 0, HEIGHT - 1));


    float edge1, edge2, edge3;

    float area = (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
    float invD = 1.0f / fabsf(area);

    // functions for edge1,2,3 
    // edge1 = (y2 - y1)*x - (x2 - x1)*y - (x1*y2 - x2*y1); --> standard form: E(x,y) = A*x + B*y + C
    float A1 = y2 - y1;
    float B1 = -(x2 - x1);
    float C1 = -(x1*y2 - x2*y1);

    float A2 = y3 - y2;
    float B2 = -(x3 - x2);
    float C2 = -(x2*y3 - x3*y2);

    float A3 = y1 - y3;
    float B3 = -(x1 - x3);
    float C3 = -(x3*y1 - x1*y3);

    float z3 = tri.v1.position.z;
    float z2 = tri.v2.position.z;
    float z1 = tri.v3.position.z;

    // perspective correct UV interpolation
    float reciprocal_Z1 = 1.0f / z1;
    float reciprocal_Z2 = 1.0f / z2;
    float reciprocal_Z3 = 1.0f / z3;
    


    float first_pixel_edge_value1 = A1*x_min + B1*y_min + C1;
    float first_pixel_edge_value2 = A2*x_min + B2*y_min + C2;
    float first_pixel_edge_value3 = A3*x_min + B3*y_min + C3;
    

    for (int y = y_min; y <= y_max; y++) {
        // E(x,y) = A*x + B*y + C


        float edge1 = first_pixel_edge_value1;
        float edge2 = first_pixel_edge_value2;
        float edge3 = first_pixel_edge_value3;
        for (int x = x_min; x <= x_max; x++) {
            
            if ((edge1 >= 0 && edge2 >= 0 && edge3 >= 0)) {
    
                float w1 = edge2 * invD; // v1
                float w2 = edge3 * invD; // v2
                float w3 = edge1 * invD; // v3

                float depth = w1*reciprocal_Z1 + w2*reciprocal_Z2 + w3*reciprocal_Z3;
                int index = y * WIDTH + x;




                if (depth > Zbuffer[index]) {
                    
                    Zbuffer[index] = depth;
                    Vec3 v3normals = {tri.v1.normal.x, tri.v1.normal.y, tri.v1.normal.z};
                    Vec3 v2normals = {tri.v2.normal.x, tri.v2.normal.y, tri.v2.normal.z};
                    Vec3 v1normals = {tri.v3.normal.x, tri.v3.normal.y, tri.v3.normal.z};

                    pixels[y * WIDTH + x] = UV_to_ARGB(
                        w1, w2, w3, 
                        v1normals, v2normals, v3normals,
                        depth, reciprocal_Z1, reciprocal_Z2, reciprocal_Z3, 
                        edge1, edge2, edge3, 
                        tri.v3.uv, tri.v2.uv, tri.v1.uv,  // v3 and v1 swap
                        texture
                    );

                }

            
            }
            // step right
            edge1 += A1;
            edge2 += A2;
            edge3 += A3;
        }
        first_pixel_edge_value1 += B1;
        first_pixel_edge_value2 += B2;
        first_pixel_edge_value3 += B3;
    }
    /*
    printf("x1: %f, y1: %f\n", x1, y1);
    printf("x2: %f, y2: %f\n", x2, y2);
    printf("x3: %f, y3: %f\n", x3, y3);

    printf("Cross Product: %f\n\n\n", cross_product);
    */
}

uint32_t UV_to_ARGB(float w1, float w2, float w3, Vec3 v1_normal, Vec3 v2_normal, Vec3 v3_normal, float depth, float reciprocal_Z1, float reciprocal_Z2, float reciprocal_Z3, float edge1, float edge2, float edge3, Vec2 v1_uv, Vec2 v2_uv, Vec2 v3_uv, TEX *texture) {
    // barycentric weights, how much does each vertex contribute. encodes x,y

    float interpolated_nx = (w1*(v1_normal.x*reciprocal_Z1)) + (w2*(v2_normal.x*reciprocal_Z2)) + (w3*(v3_normal.x*reciprocal_Z3));
    float interpolated_ny = (w1*(v1_normal.y*reciprocal_Z1)) + (w2*(v2_normal.y*reciprocal_Z2)) + (w3*(v3_normal.y*reciprocal_Z3));
    float interpolated_nz = (w1*(v1_normal.z*reciprocal_Z1)) + (w2*(v2_normal.z*reciprocal_Z2)) + (w3*(v3_normal.z*reciprocal_Z3));


    Vec3 normalized_interpolated_normal = NormalizeVec3f(interpolated_nx, interpolated_ny, interpolated_nz);

    //                                                                  
    float brightness = (normalized_interpolated_normal.x * -light_x) +
                    (normalized_interpolated_normal.y * -light_y) +
                    (normalized_interpolated_normal.z * -light_z);

    brightness = clamp_f(brightness, 0.0f, 1.0f);
    brightness = brightness*brightness;

    float light = fminf((brightness * light_brightness) + ambient_light, 1.0f);



    float interpolated_invZdepth = depth;

    float interpolatedUoverZ = w1*(v1_uv.x*interpolated_invZdepth) + w2*(v2_uv.x*interpolated_invZdepth) + w3*(v3_uv.x*interpolated_invZdepth);
    float interpolatedVoverZ = w1*(v1_uv.y*interpolated_invZdepth) + w2*(v2_uv.y*interpolated_invZdepth) + w3*(v3_uv.y*interpolated_invZdepth);

    float u = interpolatedUoverZ / interpolated_invZdepth;
    float v = interpolatedVoverZ / interpolated_invZdepth;

    int texture_width = texture->texture_width;
    int texture_height = texture->texture_height;

    int x_tex = u * (texture_width - 1);
    int y_tex = (1.0f-v) * (texture_height - 1);

    x_tex = clamp_i(x_tex, 0, texture_width - 1);
    y_tex = clamp_i(y_tex, 0, texture_height - 1);

    uint32_t ARGB = texture->ARGB[y_tex * texture_width + x_tex];

    // extract individual colors
    // >> 16 turns AAAAAAAA RRRRRRRR GGGGGGGG BBBBBBBB --> 00000000 00000000 AAAAAAAA RRRRRRRR
    //    0xFF =   00000000 00000000 00000000 11111111, so & 0xFF turns 00000000 00000000 AAAAAAAA RRRRRRRR --> 00000000 00000000 00000000 RRRRRRRR, then we can * brightness 
    
    uint8_t r = ((ARGB >> 16) & 0xFF) * light;
    uint8_t g = ((ARGB >> 8) & 0xFF) * light;
    uint8_t b = (ARGB & 0xFF) * light;
    uint8_t a = (ARGB >> 24 & 0xFF);

    uint32_t lighted_ARGB = ((uint32_t)a << 24 | (uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b);
    


    return lighted_ARGB;
}
