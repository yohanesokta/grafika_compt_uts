#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

void initMaze(Maze* m) {

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            m->grid[y][x].x1 = x * CELL_SIZE;
            m->grid[y][x].x2 = m->grid[y][x].x1 + CELL_SIZE;

            m->grid[y][x].y1 = y * CELL_SIZE;
            m->grid[y][x].y2 = m->grid[y][x].y1 + CELL_SIZE;

            m->grid[y][x].wall = 1;
        }
    }

    m->grid[1][1].wall = 0;
    m->grid[0][WIDTH / 2].wall = 0;
    m->grid[HEIGHT - 1][WIDTH / 2].wall = 0;
}
void printMaze(Maze* m) {

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

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


void divide(Maze* m, int x, int y, int w, int h) {

    int stack[WIDTH * HEIGHT][2];
    int top = 0;

    int visited[HEIGHT][WIDTH] = { 0 };

    int dx[4] = { 0,0,2,-2 };
    int dy[4] = { 2,-2,0,0 };

    stack[top][0] = 1;
    stack[top][1] = 1;
    top++;

    visited[1][1] = 1;

    while (top > 0) {

        int cx = stack[top - 1][0];
        int cy = stack[top - 1][1];

        int dirs[4] = { 0,1,2,3 };

        for (int i = 0; i < 4; i++) {
            int r = rand() % 4;
            int tmp = dirs[i];
            dirs[i] = dirs[r];
            dirs[r] = tmp;
        }

        int moved = 0;

        for (int i = 0; i < 4; i++) {

            int nx = cx + dx[dirs[i]];
            int ny = cy + dy[dirs[i]];

            if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && !visited[ny][nx]) {

                visited[ny][nx] = 1;

                m->grid[cy + dy[dirs[i]] / 2][cx + dx[dirs[i]] / 2].wall = 0;
                m->grid[ny][nx].wall = 0;

                stack[top][0] = nx;
                stack[top][1] = ny;
                top++;

                moved = 1;
                break;
            }
        }

        if (!moved)
            top--;
    }
}