#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

// Inisialisasi maze
void initMaze(Maze *maze){

    // Loop semua cell
    for(int row=0; row<HEIGHT; row++){
        for(int col=0; col<WIDTH; col++){

            // Set koordinat cell (untuk OpenGL)
            maze->grid[row][col].x1 = col * CELL_SIZE;
            maze->grid[row][col].x2 = maze->grid[row][col].x1 + CELL_SIZE;

            maze->grid[row][col].y1 = row * CELL_SIZE;
            maze->grid[row][col].y2 = maze->grid[row][col].y1 + CELL_SIZE;

            // Default semua jadi tembok
            maze->grid[row][col].wall = 1;
        }
    }

    // Buka beberapa titik (start & exit)
    maze->grid[1][1].wall = 0; // titik awal
    maze->grid[0][WIDTH/2].wall = 0; // pintu atas
    maze->grid[HEIGHT-1][WIDTH/2].wall = 0; // pintu bawah
}

// Print maze
void printMaze(Maze *maze) {

    for(int row=0; row<HEIGHT; row++) {
        for(int col=0; col<WIDTH; col++) {

            printf("(%d,%d,%d,%d) ",
                maze->grid[row][col].x1,
                maze->grid[row][col].x2,
                maze->grid[row][col].y1,
                maze->grid[row][col].y2
            );
        }
        printf("\n\n");
    }
}

// Generate maze menggunakan DFS + stack (backtracking)
void divide(Maze *maze, int startX, int startY, int width, int height){

    // Stack untuk menyimpan posisi (x,y)
    int positionStack[WIDTH*HEIGHT][2];
    int stackTop = 0;

    // Penanda cell yang sudah dikunjungi
    int visited[HEIGHT][WIDTH] = {0};

    // Arah gerak (lompat 2 cell)
    int dirX[4] = {0,0,2,-2};
    int dirY[4] = {2,-2,0,0};

    // Mulai dari (1,1)
    positionStack[stackTop][0] = 1;
    positionStack[stackTop][1] = 1;
    stackTop++;

    visited[1][1] = 1;

    // Selama masih ada di stack
    while(stackTop > 0){

        // Posisi sekarang
        int currentX = positionStack[stackTop-1][0];
        int currentY = positionStack[stackTop-1][1];

        // Urutan arah (acak)
        int directionOrder[4] = {0,1,2,3};

        // Shuffle arah biar random
        for(int i=0;i<4;i++){
            int randomIndex = rand()%4;
            int temp = directionOrder[i];
            directionOrder[i] = directionOrder[randomIndex];
            directionOrder[randomIndex] = temp;
        }

        int moved = 0;

        // Coba semua arah
        for(int i=0;i<4;i++){

            int nextX = currentX + dirX[directionOrder[i]];
            int nextY = currentY + dirY[directionOrder[i]];

            // Cek valid & belum dikunjungi
            if(nextX > 0 && nextX < WIDTH-1 &&
               nextY > 0 && nextY < HEIGHT-1 &&
               !visited[nextY][nextX]){

                visited[nextY][nextX] = 1;

                // Hapus tembok di tengah
                maze->grid[currentY + dirY[directionOrder[i]]/2]
                          [currentX + dirX[directionOrder[i]]/2].wall = 0;

                // Buka jalan ke cell baru
                maze->grid[nextY][nextX].wall = 0;

                // Masukkan ke stack
                positionStack[stackTop][0] = nextX;
                positionStack[stackTop][1] = nextY;
                stackTop++;

                moved = 1;
                break;
            }
        }

        // Kalau mentok, mundur (backtrack)
        if(!moved)
            stackTop--;
    }
}