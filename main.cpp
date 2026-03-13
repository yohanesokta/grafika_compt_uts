#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <_stdlib.h>
#else
#include <GL/glut.h>
#endif

#include <time.h>
#include "src/maze.h"
#include <stdio.h>

Maze maze;

struct  movement
{
    float x;
    float y;
};


void reInitMaze();

int scene = 0;
movement player;
movement c_nim {1.0,1.0};

void display()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.1, 0.0, 1.0);

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (maze.grid[y][x].wall)
            {
                glRecti(
                    maze.grid[y][x].x1,
                    maze.grid[y][x].y1,
                    maze.grid[y][x].x2,
                    maze.grid[y][x].y2);
            }
        }
    }
    glColor3f(1.0, 0.0, 0.0);
    glRectf(player.x * CELL_SIZE, player.y * CELL_SIZE,(player.x + 1) * CELL_SIZE, (player.y + 1) * CELL_SIZE);

    glColor3f(0.0,0.0,0.0);

    glRectf(c_nim.x + 0.3, c_nim.y+0.0, c_nim.x + 0.8, c_nim.y + 0.2);
    glRectf(c_nim.x + 0.0, c_nim.y+0.0, c_nim.x + 0.2, c_nim.y + 0.5);
    // glRectf(c_nim.x + 0.3, c_nim.y+0.5, c_nim.x + 0.8, c_nim.y + 0.7);
    glRectf(c_nim.x + 0.0, c_nim.y+0.7, c_nim.x + 0.2, c_nim.y + 1.2);
    glRectf(c_nim.x + 0.3, c_nim.y+1.0, c_nim.x + 0.8, c_nim.y + 1.2);
    glRectf(c_nim.x + 0.9, c_nim.y+0.0, c_nim.x + 1.1, c_nim.y + 0.5);
    glRectf(c_nim.x + 0.9, c_nim.y+0.7, c_nim.x + 1.1, c_nim.y + 1.2);

    // glRectf(c_nim.x + 1.4 + 0.3, c_nim.y+0.0, c_nim.x + 1.4 + 0.8, c_nim.y + 0.2);
    // glRectf(c_nim.x + 1.4 + 0.0, c_nim.y+0.0, c_nim.x + 1.4 + 0.2, c_nim.y + 0.5);
    // glRectf(c_nim.x + 1.4 + 0.3, c_nim.y+0.5, c_nim.x + 1.4 + 0.8, c_nim.y + 0.7);
    // glRectf(c_nim.x + 1.4 + 0.0, c_nim.y+0.7, c_nim.x + 1.4 + 0.2, c_nim.y + 1.2);
    glRectf(c_nim.x + 1.4 + 0.3, c_nim.y+1.0, c_nim.x + 1.4 + 0.8, c_nim.y + 1.2);
    glRectf(c_nim.x + 1.4 + 0.9, c_nim.y+0.0, c_nim.x + 1.4 + 1.1, c_nim.y + 0.5);
    glRectf(c_nim.x + 1.4 + 0.9, c_nim.y+0.7, c_nim.x + 1.4 + 1.1, c_nim.y + 1.2);

    glRectf(c_nim.x + 2.8 + 0.3, c_nim.y+0.0, c_nim.x + 2.8 + 0.8, c_nim.y + 0.2);
    glRectf(c_nim.x + 2.8 + 0.0, c_nim.y+0.0, c_nim.x + 2.8 + 0.2 , c_nim.y + 0.5);
    glRectf(c_nim.x + 2.8 + 0.3, c_nim.y+0.5, c_nim.x + 2.8 + 0.8, c_nim.y + 0.7);
    glRectf(c_nim.x + 2.8 + 0.0, c_nim.y+0.7, c_nim.x + 2.8 + 0.2, c_nim.y + 1.2);
    glRectf(c_nim.x + 2.8 + 0.3, c_nim.y+1.0, c_nim.x + 2.8 + 0.8 + 0.2, c_nim.y + 1.2);
    glRectf(c_nim.x + 2.8 + 0.9, c_nim.y+0.0, c_nim.x + 2.8 + 1.1, c_nim.y + 0.5);
    // glRectf(c_nim.x + 2.8 + 0.9, c_nim.y+0.7, c_nim.x + 2.8 + 1.1, c_nim.y + 1.2);




    glFlush();
}

void myinit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH * CELL_SIZE, 0, HEIGHT * CELL_SIZE);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void movement_handler(float dx, float dy) {
    player.x += dx;
    player.y += dy;
    glutDisplayFunc(display);
    glutPostRedisplay();
}


float speed = 0.2;

void keyboard(unsigned char key, int x, int y)
{
    printf("Key Pressed : %u \n",key);
    switch (key)
    {
    case 27:
    case 113:
        exit(0);
        break;
    case 119:
        movement_handler(0,speed);
        break;
    case 97:
        movement_handler(-speed,0);
        break;
    case 115:
        movement_handler(0,-speed);
        break;
    case 100:
        movement_handler(speed,0);
        break;
    case 99:
        reInitMaze();
        break;
    }
}

float frandom() {
    return (float) rand() / (float)RAND_MAX;
}

void reInitMaze(){
    if (scene %2 == 0){
        player = {(WIDTH)/2,HEIGHT-1};
    } else {
        player = {(WIDTH)/2,0};
    }
    scene += 1;
    srand(time(NULL));
    initMaze(&maze);
    divide(&maze, 0, 0, WIDTH, HEIGHT);
    c_nim = { frandom() * (float)((WIDTH * CELL_SIZE) - 4.2), frandom() * (float)((HEIGHT * CELL_SIZE) - 4.2) };
    glutDisplayFunc(display);
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Maze Game Grafkom UTS");
    reInitMaze();
    glutKeyboardFunc(keyboard);
    myinit();
    glutMainLoop();
    return 0;
}
