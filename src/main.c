
#include <SDL3/SDL.h> 
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "sdlmanager.h"
#include "draw2d.h"
#include "config.h" // contains WIDTH and HEIGHT
#include "draw3d.h"
#include "obj_loader.h"
#include "lighting.h"

uint32_t pixels[WIDTH * HEIGHT] = {0};

RGBA black = {0, 0, 0, 255};
RGBA red = {255, 0, 0, 255};

Light light = {
    .direction = {0.0f, 0.0f, 1.0f},
    .brightness = 0.8f,
    .ambient = 0.1f
};

float rotation = 0.0f;


int main(void) {
    CreateWindow(WIDTH, HEIGHT, "David and Fruits");
    SetCullingMode(CULL_BACK);
    
    SetLightVector(light);


    OBJ apple = Load_OBJ("objects/apple.obj");
    TEX apple_texture = Load_Tex("objects/apple_texture.jpeg");

    OBJ banana = Load_OBJ("objects/banana.obj");
    TEX banana_texture = Load_Tex("objects/banana_texture.png");
    
    OBJ david = Load_OBJ("objects/david.obj");
    TEX david_texture = Load_Tex("objects/david_texture.png");


    while (NoneExitingPollEvents()) {

        ClearBuffer(pixels, black);
    

        BEGIN_RotMat(rotation, 0.0f, 0.0f, 4.0f, 0.0f, 1.0f, 0.0f);


        Render_OBJ(&david, &david_texture,
            0.0f, -1.0f, 4.0f,
            200.0f, 200.0f, 200.0f,
            pixels);


        END_RotMat();
        

        BEGIN_RotMat(rotation, 0.0f, 0.0f, 4.0f, 0.0f, -1.0f, 0.0f);

        Render_OBJ(&apple, &apple_texture,
            0.0f, 0.0f, 2.0f, 
            100.0f, 100.0f, 100.0f, 
            pixels);

        Render_OBJ(&banana, &banana_texture,
            0.0f, 0.0f, 6.0f, 
            100.0f, 100.0f, 100.0f, 
            pixels);
        
        
        END_RotMat();
        


        rotation += 90.0f * GetDTs();

        PushToFrameBuffer(pixels, WIDTH);
        
    }

    Free_OBJ(&apple);
    Free_Tex(&apple_texture);

    Free_OBJ(&banana);
    Free_Tex(&banana_texture);

    Free_OBJ(&david);
    Free_Tex(&david_texture);


    
    SDL_Quit();
    return 0;
}

//  gcc main.c sdlmanager.c lighting.c draw2d.c vector_operations.c quaternion_operations.c draw3d.c obj_loader.c -O3 -march=native -ffast-math -I SDL3-3.4.8/x86_64-w64-mingw32/include/ -L SDL3-3.4.8/x86_64-w64-mingw32/lib -lSDL3 -o framebuffer
// 