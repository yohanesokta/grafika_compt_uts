#include <stdio.h>
#include <stdlib.h>
#include "maze.h"


void initMaze(Maze *maze){

    
    for(int row=0; row<HEIGHT; row++){
        for(int col=0; col<WIDTH; col++){

            
            maze->grid[row][col].x1 = col * CELL_SIZE;
            maze->grid[row][col].x2 = maze->grid[row][col].x1 + CELL_SIZE;

            maze->grid[row][col].y1 = row * CELL_SIZE;
            maze->grid[row][col].y2 = maze->grid[row][col].y1 + CELL_SIZE;

            
            maze->grid[row][col].wall = 1;
        }
    }

    
    maze->grid[1][1].wall = 0; 
    maze->grid[0][WIDTH/2].wall = 0; 
    maze->grid[HEIGHT-1][WIDTH/2].wall = 0; 
}


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


void divide(Maze *maze, int startX, int startY, int width, int height){

    
    int positionStack[WIDTH*HEIGHT][2];
    int stackTop = 0;

    
    int visited[HEIGHT][WIDTH] = {0};

    
    int dirX[4] = {0,0,2,-2};
    int dirY[4] = {2,-2,0,0};

    
    positionStack[stackTop][0] = 1;
    positionStack[stackTop][1] = 1;
    stackTop++;

    visited[1][1] = 1;

    
    while(stackTop > 0){

        
        int currentX = positionStack[stackTop-1][0];
        int currentY = positionStack[stackTop-1][1];

        
        int directionOrder[4] = {0,1,2,3};

        
        for(int i=0;i<4;i++){
            int randomIndex = rand()%4;
            int temp = directionOrder[i];
            directionOrder[i] = directionOrder[randomIndex];
            directionOrder[randomIndex] = temp;
        }

        int moved = 0;

        
        for(int i=0;i<4;i++){

            int nextX = currentX + dirX[directionOrder[i]];
            int nextY = currentY + dirY[directionOrder[i]];

            
            if(nextX > 0 && nextX < WIDTH-1 &&
               nextY > 0 && nextY < HEIGHT-1 &&
               !visited[nextY][nextX]){

                visited[nextY][nextX] = 1;

                
                maze->grid[currentY + dirY[directionOrder[i]]/2]
                          [currentX + dirX[directionOrder[i]]/2].wall = 0;

                
                maze->grid[nextY][nextX].wall = 0;

                
                positionStack[stackTop][0] = nextX;
                positionStack[stackTop][1] = nextY;
                stackTop++;

                moved = 1;
                break;
            }
        }

        
        if(!moved)
            stackTop--;
    }
}