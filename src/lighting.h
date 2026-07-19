#ifndef LIGHTING_H
#define LIGHTING_H


extern float light_x;
extern float light_y;
extern float light_z;

extern float light_brightness;
extern float ambient_light;

typedef struct {
    Vec3 direction;
    float brightness;
    float ambient;
} Light;


void SetLightVector(Light light);



#endif