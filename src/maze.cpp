#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 10
#define CELL_SIZE 10

typedef struct {
    int x1, x2;
    int y1, y2;
    int wall;
} Cell;

Cell maze[HEIGHT][WIDTH];

void initMaze() {

    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {

            maze[y][x].x1 = x * CELL_SIZE;
            maze[y][x].x2 = maze[y][x].x1 + CELL_SIZE;

            maze[y][x].y1 = y * CELL_SIZE;
            maze[y][x].y2 = maze[y][x].y1 + CELL_SIZE;

            maze[y][x].wall = 0;
        }
    }
}

void printMaze() {

    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {

            printf("(%d,%d,%d,%d) ",
                maze[y][x].x1,
                maze[y][x].x2,
                maze[y][x].y1,
                maze[y][x].y2
            );
        }
        printf("\n\n");
    }
}

void divide(int x, int y, int w, int h) {

    if(w < 2 || h < 2)
        return;

    int horizontal = rand() % 2;

    if(horizontal) {

        int wy = y + rand() % h;
        int passage = x + rand() % w;

        for(int i = x; i < x+w; i++) {
            if(i != passage)
                maze[wy][i].wall = 1;
        }

        divide(x, y, w, wy-y);
        divide(x, wy+1, w, y+h-wy-1);

    }
    else {

        int wx = x + rand() % w;
        int passage = y + rand() % h;

        for(int i = y; i < y+h; i++) {
            if(i != passage)
                maze[i][wx].wall = 1;
        }

        divide(x, y, wx-x, h);
        divide(wx+1, y, x+w-wx-1, h);
    }
}

int main() {
    srand(time(NULL));
    initMaze();
    divide(0,0,WIDTH,HEIGHT);
    printMaze();
    return 0;
}