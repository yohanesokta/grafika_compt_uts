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

Maze maze; // variabel global maze

// Struktur posisi (player & objek lain)
struct movement
{
    float x;
    float y;
};

bool initDisplay = true;


void reInitMaze();

// Posisi player (mulai di tengah atas)
movement player {(float)(WIDTH/2), (float)(HEIGHT-1)};

// Posisi angka (nim)
movement c_nim {1.0,1.0};

// Fungsi render (digambar tiap frame)
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Warna biru untuk tembok
    glColor3f(0.1, 0.0, 1.0);

    // Gambar semua tembok
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

    // Warna hitam untuk angka
    glColor3f(0.0,0.0,0.0);

    float scale = 0.5;

    // Posisi tengah angka
    float baseX = c_nim.x * CELL_SIZE + CELL_SIZE/2;
    float baseY = c_nim.y * CELL_SIZE + CELL_SIZE/2;

    // Gambar angka "076" (menggunakan GL_POLYGON / GLRectf)

    // angka 0
    glRectf(baseX + (-1.8)*scale, baseY + (1.6)*scale, baseX + (-1.0)*scale, baseY + (1.8)*scale);
    glRectf(baseX + (-1.8)*scale, baseY + (-1.8)*scale, baseX + (-1.0)*scale, baseY + (-1.6)*scale);
    glRectf(baseX + (-1.8)*scale, baseY + (-1.6)*scale, baseX + (-1.6)*scale, baseY + (1.6)*scale);
    glRectf(baseX + (-1.2)*scale, baseY + (-1.6)*scale, baseX + (-1.0)*scale, baseY + (1.6)*scale);

    // angka 7
    glRectf(baseX + (-0.8)*scale, baseY + (1.6)*scale, baseX + (0.6)*scale, baseY + (1.8)*scale);
    glRectf(baseX + (0.4)*scale, baseY + (-1.8)*scale, baseX + (0.6)*scale, baseY + (1.6)*scale);

    // angka 6
    glRectf(baseX + (0.8)*scale, baseY + (1.6)*scale, baseX + (1.8)*scale, baseY + (1.8)*scale);
    glRectf(baseX + (0.8)*scale, baseY + (-1.8)*scale, baseX + (1.0)*scale, baseY + (1.6)*scale);
    glRectf(baseX + (1.0)*scale, baseY + (-1.8)*scale, baseX + (1.8)*scale, baseY + (-1.6)*scale);
    glRectf(baseX + (1.6)*scale, baseY + (-1.6)*scale, baseX + (1.8)*scale, baseY + (0.0)*scale);
    glRectf(baseX + (1.0)*scale, baseY + (-0.2)*scale, baseX + (1.6)*scale, baseY + (0.0)*scale);

    // glRectf(c_nim.x + 0.0, c_nim.y+0.0, c_nim.x + 0.5, c_nim.y + 0.2);
    // Player warna merah
    glColor3f(1.0, 0.0, 0.0);

    // Gambar player sebagai kotak
    glRectf(player.x * CELL_SIZE, player.y * CELL_SIZE,(player.x + 1) * CELL_SIZE, (player.y + 1) * CELL_SIZE);

    glFlush();
}

// Inisialisasi OpenGL
void myinit()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Setup koordinat 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH * CELL_SIZE, 0, HEIGHT * CELL_SIZE);

    glMatrixMode(GL_MODELVIEW);

    // Background putih
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

// Fungsi gerakan player
void movement_handler(float dx, float dy) {
    player.x += dx;
    player.y += dy;

    // Refresh tampilan
    glutDisplayFunc(display);
    glutPostRedisplay();
}

float speed = 0.2f;

const int KEY_ESC = 27;
// Input keyboard
void keyboard(unsigned char key, int x, int y)
{
    printf("Key Pressed : %u \n",key);
    if (key == KEY_ESC) {
        exit(0);
    }
    switch (key)
    {
    case 'w':
        movement_handler(0,speed);
        break;

    case 'a':
        movement_handler(-speed,0);
        break;

    case 's':
        movement_handler(0,-speed);
        break;

    case 'd':
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

// Generate ulang maze + reset posisi
void reInitMaze(){
    srand(time(NULL));

    initMaze(&maze); // inisialisasi
    divide(&maze, 0, 0, WIDTH, HEIGHT); // generate maze

    int x,y;

    // Cari posisi random yang bukan tembok
    do{
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
    }
    while(
        maze.grid[y][x].wall ||
        x == 0 || x == WIDTH-1 ||
        y == 0 || y == HEIGHT-1
    );

    // Set posisi angka
    c_nim.x = x;
    c_nim.y = y;

    // Reset player ke atas tengah
    player.x = WIDTH / 2;
    player.y = HEIGHT - 1;
    if (initDisplay) {
        initDisplay = false;
        glutDisplayFunc(display);
    }
    glutPostRedisplay();
}

// Main program
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    // Mode tampilan
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Ukuran window
    glutInitWindowSize(500, 500);

    // Judul window
    glutCreateWindow("OpenGL Maze Game - UTS");

    // Generate maze
    reInitMaze();

    // Set input keyboard
    glutKeyboardFunc(keyboard);

    // Setup OpenGL
    myinit();

    // Loop utama
    glutMainLoop();

    return 0;
}
