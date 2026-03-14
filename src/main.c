#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

//#define WIDTH 900
//#define HEIGHT 600
#define ROWS 5
#define COLS 5
#define DIFF_A 0.2097
#define DIFF_B 0.105
#define FEED 0.055
#define KILL 0.062

float A[ROWS][COLS];
float B[ROWS][COLS];
float Anext[ROWS][COLS];
float Bnext[ROWS][COLS];


void printArray(float arr[ROWS][COLS], const char *name){
    printf("%s", name);
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            printf("%.1f", arr[i][j]);
            printf("%s", " ");
        }
        printf("\n");
    }
}

float getLaplacian(float arr[ROWS][COLS], int x, int y){
    float center = arr[x][y];
    float up = arr[(x-1 + ROWS) % ROWS][y];
    float down = arr[(x+1) % ROWS][y];
    float left = arr[x][(y-1 + COLS) % COLS];
    float right = arr[x][(y+1) % COLS];
    
    return up + down + left + right - (4.0 * center);
}

void simulate(){
    // Simulate Array A and B
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

    // Print new arrays
    printArray(A, "\nArray A (New):\n");
    printArray(B, "\nArray B (New):\n");
}

int main(int argc, char *argv[]){
    // Initialize arrays
    srand(time(NULL));
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            A[r][c] = 1;
            B[r][c] = 0;
            B[1][1] = 0.5;
            Anext[r][c] = Bnext[r][c] = 0;

            //if(((rand() % 100) + 1) < 5){
            //    A[r][c] = 0;
            //    B[r][c] = 1;
            //}
        }
    }

    // Print Original Arrays
    printArray(A, "\nArray A (Original):\n");
    printArray(B, "\nArray B (Original):\n");

    // Simulation Loop
    for (int i = 0; i <= 3; i++){
        simulate();
    }

    return 0;
}