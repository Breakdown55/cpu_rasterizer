#include "draw2d.h"
#include "lighting.h"
#include "vector_operations.h"

float light_x = 0.0f;
float light_y = 0.0f;
float light_z = 0.0f;

float light_brightness = 0.0f;
float ambient_light = 0.0f;


void SetLightVector(Light light) {
    Vec3 nv = NormalizeVec3(light.direction);

    light_x = nv.x;
    light_y = nv.y;
    light_z = nv.z;

    ambient_light = light.ambient;
    light_brightness = light.brightness;
}