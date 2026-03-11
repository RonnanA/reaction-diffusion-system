#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WIDTH 900
#define HEIGHT 600
#define COLOUR_WHITE 0xffffffff

int main(int argc, char *argv[]){
    
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Rect rect;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Reaction-Diffusion",
        WIDTH,
        HEIGHT,
        0
    );

    surface = SDL_GetWindowSurface(window);
    rect = (SDL_Rect) {200,200,200,200};
    SDL_FillSurfaceRect(surface, &rect, COLOUR_WHITE);
    SDL_UpdateWindowSurface(window);


    SDL_Delay(5000);
    return 0;
}