#ifndef MAZE_H
#define MAZE_H

// Ukuran maze (grid)
#define WIDTH 15
#define HEIGHT 15

// Ukuran tiap cell (dipakai untuk render di OpenGL)
#define CELL_SIZE 2

// Struktur untuk satu cell di maze
typedef struct {
    int x1, x2; // koordinat kiri dan kanan (untuk gambar)
    int y1, y2; // koordinat bawah dan atas (untuk gambar)
    int wall;   // 1 = dinding, 0 = jalan
} Cell;

// Struktur maze (2D grid)
typedef struct {
    Cell grid[HEIGHT][WIDTH];
} Maze;

// Inisialisasi maze (semua jadi tembok dulu)
void initMaze(Maze *m);

// Print isi maze (debug)
void printMaze(Maze *m);

// Generate maze (pakai algoritma DFS + stack)
void divide(Maze *m, int x, int y, int w, int h);

#endif
