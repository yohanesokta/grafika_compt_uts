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
#include <maze.h>
#include <keyboard.h>
#include <stdio.h>

Maze maze;
    
struct  movement
{
    int x;
    int y;
};

void reInitMaze();

movement player {0, 0};

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
    glRecti(player.x * CELL_SIZE, player.y * CELL_SIZE,(player.x + 1) * CELL_SIZE, (player.y + 1) * CELL_SIZE);

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

void keyboard(unsigned char key, int x, int y)
{   

    /*
        key saved value:
        27 = esc
        99 = c
        119 = w
        97 = a
        115 = s
        100 = d
    */


    printf("Key pressed: %d\n", key);
    switch (key)
    {
    case 27:
        exit(0);
        break;
   
    case 99:
        printf("Reinitializing maze...\n");
        reInitMaze();
        break;
    }
}


void reInitMaze(){
    srand(time(NULL));
    initMaze(&maze);
    divide(&maze, 0, 0, WIDTH, HEIGHT);
    glutDisplayFunc(display);
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Segitiga Titik");
    reInitMaze();
    glutKeyboardFunc(keyboard);
    myinit();
    glutMainLoop();
    return 0;
}
