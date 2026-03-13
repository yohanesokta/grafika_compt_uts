#pragma once
#ifndef MAZE_H
#define MAZE_H

#define WIDTH 7
#define HEIGHT 7
#define CELL_SIZE 2

typedef struct {
    int x1, x2;
    int y1, y2;
    int wall;
} Cell;



typedef struct {
    
    Cell grid[HEIGHT][WIDTH];
} Maze;

void initMaze(Maze* m);
void printMaze(Maze* m);
void divide(Maze* m, int x, int y, int w, int h);
#endif