#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WIDTH 700
#define HEIGHT 400
#define ROWS HEIGHT
#define COLS WIDTH

#define DIFF_A 0.2097
#define DIFF_B 0.105
#define FEED 0.054
#define KILL 0.062

// Define arrays and pixel buffer
float A[ROWS][COLS];
float B[ROWS][COLS];
float Anext[ROWS][COLS];
float Bnext[ROWS][COLS];
Uint32 pixels[ROWS * COLS];


void initialize_arrays(){
    // Initialize arrays
    srand(time(NULL));
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            A[r][c] = 1.0;
            B[r][c] = 0.0;
            Anext[r][c] = 0.0;
            Bnext[r][c] = 0.0;
        }
    }

    // Random seeding of seeds
    int numSeeds = 10;
    for(int i = 0; i < numSeeds; i++){
        int centerR = (rand() % (ROWS - 20)) + 10;
        int centerC = (rand() % (COLS - 20)) + 10;
        for(int r = centerR - 10; r < centerR + 10; r++){
            for(int c = centerC - 10; c < centerC + 10; c++){
                A[r][c] = 0.5;
                B[r][c] = 0.5;
            }
        }
    }
}

float getLaplacian(float arr[ROWS][COLS], int r, int c){
    float center = arr[r][c];
    float up = arr[(r-1 + ROWS) % ROWS][c];
    float down = arr[(r+1) % ROWS][c];
    float left = arr[r][(c-1 + COLS) % COLS];
    float right = arr[r][(c+1) % COLS];
    
    return up + down + left + right - (4.0 * center);
}

void simulate(){
    // Simulation: Apply Gray-Scott model to array A and B
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            float lapA = getLaplacian(A, r, c);
            Anext[r][c] = (A[r][c] + (DIFF_A * lapA) - (A[r][c] * B[r][c] * B[r][c]) + (FEED * (1 - A[r][c])));

            float lapB = getLaplacian(B, r, c);
            Bnext[r][c] = (B[r][c] + (DIFF_B * lapB) + (A[r][c] * B[r][c] * B[r][c]) - ((KILL + FEED) * B[r][c]));
        }
    }
    
    // Copy back to original
    memcpy(A, Anext, sizeof(A));
    memcpy(B, Bnext, sizeof(B));
}

void render(Uint32 *pixels){
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            Uint8 col = B[r][c] > 0.1 ? 0 : 255;
            //Uint8 col = (Uint8)(A[r][c] * 255);

            // Modify RGB pack to change colour
            pixels[r * COLS + c] = (255 << 24) | (col << 16) | (col << 8) | col;
        }
    }
}

int main(int argc, char *argv[]){
    // SDL variables
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("reaction-diffusion-system", WIDTH, HEIGHT, 0);
    if (!window) { SDL_Log("Window error: %s", SDL_GetError()); SDL_Quit(); return 1; }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) { SDL_Log("Renderer error: %s", SDL_GetError()); SDL_Quit(); return 1; }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    if (!texture) { SDL_Log("Texture error: %s", SDL_GetError()); SDL_Quit(); return 1; }

    initialize_arrays();

    // Simulation Loop
    int running = 1;
    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_EVENT_QUIT) running = 0;
        }

        simulate();
        render(pixels);

        SDL_RenderClear(renderer);
        SDL_UpdateTexture(texture, NULL, pixels, COLS * sizeof(Uint32));
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}