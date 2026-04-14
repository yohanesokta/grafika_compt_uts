#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

// Inisialisasi maze
void initMaze(Maze *m){

    // Loop semua cell
    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){

            // Set koordinat cell (untuk OpenGL)
            m->grid[y][x].x1 = x * CELL_SIZE;
            m->grid[y][x].x2 = m->grid[y][x].x1 + CELL_SIZE;

            m->grid[y][x].y1 = y * CELL_SIZE;
            m->grid[y][x].y2 = m->grid[y][x].y1 + CELL_SIZE;

            // Default semua jadi tembok
            m->grid[y][x].wall = 1;
        }
    }

    // Buka beberapa titik (start & exit)
    m->grid[1][1].wall = 0; // titik awal
    m->grid[0][WIDTH/2].wall = 0; // pintu atas
    m->grid[HEIGHT-1][WIDTH/2].wall = 0; // pintu bawah
}

// Print maze
void printMaze(Maze *m) {

    for(int y=0; y<HEIGHT; y++) {
        for(int x=0; x<WIDTH; x++) {

            printf("(%d,%d,%d,%d) ",
                m->grid[y][x].x1,
                m->grid[y][x].x2,
                m->grid[y][x].y1,
                m->grid[y][x].y2
            );
        }
        printf("\n\n");
    }
}

// Generate maze menggunakan DFS + stack (backtracking)
void divide(Maze *m, int x, int y, int w, int h){

    // Stack untuk algoritma DFS (menyimpan posisi)
    int stack[WIDTH*HEIGHT][2];
    int top = 0;

    // Array untuk menandai sudah dikunjungi
    int visited[HEIGHT][WIDTH] = {0};

    // Arah gerak (lompat 2 cell)
    int dx[4] = {0,0,2,-2};
    int dy[4] = {2,-2,0,0};

    // Mulai dari (1,1)
    stack[top][0] = 1;
    stack[top][1] = 1;
    top++;

    visited[1][1] = 1;

    // Selama stack masih ada
    while(top > 0){

        // Ambil posisi sekarang
        int cx = stack[top-1][0];
        int cy = stack[top-1][1];

        // Arah array (acak)
        int dirs[4] = {0,1,2,3};

        // Shuffle arah agar random
        for(int i=0;i<4;i++){
            int r = rand()%4;
            int tmp = dirs[i];
            dirs[i] = dirs[r];
            dirs[r] = tmp;
        }

        int moved = 0;

        // Coba segala arah
        for(int i=0;i<4;i++){

            int nx = cx + dx[dirs[i]];
            int ny = cy + dy[dirs[i]];

            // Cek valid & belum dikunjungi
            if(nx > 0 && nx < WIDTH-1 && ny > 0 && ny < HEIGHT-1 && !visited[ny][nx]){

                visited[ny][nx] = 1;

                // Hapus tembok di tengah
                m->grid[cy + dy[dirs[i]]/2][cx + dx[dirs[i]]/2].wall = 0;

                // Buka jalan ke cell baru
                m->grid[ny][nx].wall = 0;

                // Push ke stack
                stack[top][0] = nx;
                stack[top][1] = ny;
                top++;

                moved = 1;
                break;
            }
        }

        // Jika sudah dilalui, maka backtrack
        if(!moved)
            top--;
    }
}