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
#include "maze.h"
#include <stdio.h>

Maze maze;
    
struct  movement
{
    float x;
    float y;
};


void reInitMaze();

movement player {(float)(WIDTH/2), (float)(HEIGHT-1)};
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
    
    float scale = 0.5;
    float baseX = c_nim.x * CELL_SIZE + CELL_SIZE/2;
    float baseY = c_nim.y * CELL_SIZE + CELL_SIZE/2;

    // angka 0
    glRectf(baseX + (-1.8)*scale, baseY + (1.6)*scale, baseX + (-1.0)*scale, baseY + (1.8)*scale);
    glRectf(baseX + (-1.8)*scale, baseY + (-1.8)*scale, baseX + (-1.0)*scale, baseY + (-1.6)*scale);
    glRectf(baseX + (-1.8)*scale, baseY + (-1.6)*scale, baseX + (-1.6)*scale, baseY + (1.6)*scale);
    glRectf(baseX + (-1.2)*scale, baseY + (-1.6)*scale, baseX + (-1.0)*scale, baseY + (1.6)*scale);

    // angka 7
    glRectf(baseX + (-0.8)*scale, baseY + (1.6)*scale, baseX + (0.6)*scale, baseY + (1.8)*scale);

    glBegin(GL_POLYGON);
    glVertex2f(baseX + (-0.8)*scale, baseY + (-1.8)*scale);
    glVertex2f(baseX + (-0.6)*scale, baseY + (-1.8)*scale);
    glVertex2f(baseX + (0.6)*scale, baseY + (1.6)*scale);
    glVertex2f(baseX + (0.4)*scale, baseY + (1.6)*scale);
    glEnd();

    // angka 6
    glRectf(baseX + (0.8)*scale, baseY + (1.6)*scale, baseX + (1.8)*scale, baseY + (1.8)*scale);
    glRectf(baseX + (0.8)*scale, baseY + (-1.8)*scale, baseX + (1.0)*scale, baseY + (1.6)*scale);
    glRectf(baseX + (1.0)*scale, baseY + (-1.8)*scale, baseX + (1.8)*scale, baseY + (-1.6)*scale);
    glRectf(baseX + (1.6)*scale, baseY + (-1.6)*scale, baseX + (1.8)*scale, baseY + (0.0)*scale);
    glRectf(baseX + (1.0)*scale, baseY + (-0.2)*scale, baseX + (1.6)*scale, baseY + (0.0)*scale);

    // glRectf(c_nim.x + 0.0, c_nim.y+0.0, c_nim.x + 0.5, c_nim.y + 0.2);

    glFlush();
}

void myinit()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
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
    case 'c':
    case 'C':
        reInitMaze();
        break;
    }
}

float randomFloat() {
    return (float)rand() / RAND_MAX;
}

void reInitMaze(){
    srand(time(NULL));

    initMaze(&maze);
    divide(&maze, 0, 0, WIDTH, HEIGHT);

    int x,y;

    do{
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
    }
    while(
        maze.grid[y][x].wall ||
        x == 0 || x == WIDTH-1 ||
        y == 0 || y == HEIGHT-1
    );

    c_nim.x = x;
    c_nim.y = y;

    glutDisplayFunc(display);
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("OpenGL Maze Game - UTS");
    reInitMaze();
    glutKeyboardFunc(keyboard);
    myinit();
    glutMainLoop();
    return 0;
}
