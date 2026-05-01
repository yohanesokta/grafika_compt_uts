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
//18
#include <math.h>

Maze maze; // variabel global maze
bool is3D = false; //1
//15
float rotY = 45.0f; // kiri-kanan
float rotX = 30.0f; // atas-bawah

// Struktur posisi (player & objek lain)
struct movement
{
    float x;
    float y;
};


void reInitMaze();
//14
void drawWall3D(float x, float z, float h, bool isPlayer);
void drawFloor3D();

// Posisi player (mulai di tengah atas)
movement player {(float)(WIDTH/2), (float)(HEIGHT-1)};

// Posisi angka (nim)
movement c_nim {1.0,1.0};

// Fungsi render (digambar tiap frame)
//3
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!is3D)
    {
        // ================= 2D =================
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, 0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // WALL
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                if (maze.grid[y][x].wall)
                {
                    glColor3f(0.1, 0.0, 1.0);
                    glRecti(
                        maze.grid[y][x].x1,
                        maze.grid[y][x].y1,
                        maze.grid[y][x].x2,
                        maze.grid[y][x].y2);
                }
            }
        }

        // PLAYER
        glColor3f(1.0, 0.0, 0.0);
        glRectf(player.x * CELL_SIZE, player.y * CELL_SIZE,
                (player.x + 1) * CELL_SIZE, (player.y + 1) * CELL_SIZE);
    }
    else
    {
        // ================= 3D =================
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(40.0, 1.0, 0.5, 300.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float cx = (WIDTH * CELL_SIZE) / 2.0f;
        float cz = (HEIGHT * CELL_SIZE) / 2.0f;

        float radius = 40.0f;

        float camX = cx + radius * cos(rotY * 3.14159 / 180.0f) * cos(rotX * 3.14159 / 180.0f);
        float camY = radius * sin(rotX * 3.14159 / 180.0f);
        float camZ = cz + radius * sin(rotY * 3.14159 / 180.0f) * cos(rotX * 3.14159 / 180.0f);

        gluLookAt(camX, camY, camZ,
                cx, 0.0, cz,
                0.0, 1.0, 0.0);

        // FLOOR
        drawFloor3D();

        // WALL
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                if (maze.grid[y][x].wall)
                {
                    drawWall3D(x, y, 2.5f, false);
                }
            }
        }

        // PLAYER
        drawWall3D(player.x, player.y, 1.5f, true);
    }

    glutSwapBuffers();
}

//4
void drawWall3D(float x, float z, float h, bool isPlayer)
{
    float x0 = x * CELL_SIZE;
    float z0 = z * CELL_SIZE;
    float s = CELL_SIZE;

    glBegin(GL_QUADS);

    if (isPlayer)
    {
        // 🔴 PLAYER (merah)
        // ATAS
        glColor3f(1.0, 0.3, 0.3);
        glVertex3f(x0, h, z0);
        glVertex3f(x0+s, h, z0);
        glVertex3f(x0+s, h, z0+s);
        glVertex3f(x0, h, z0+s);

        // DEPAN
        glColor3f(0.9, 0.0, 0.0);
        glVertex3f(x0,0,z0+s);
        glVertex3f(x0+s,0,z0+s);
        glVertex3f(x0+s,h,z0+s);
        glVertex3f(x0,h,z0+s);

        // KANAN
        glColor3f(0.6, 0.0, 0.0);
        glVertex3f(x0+s,0,z0);
        glVertex3f(x0+s,0,z0+s);
        glVertex3f(x0+s,h,z0+s);
        glVertex3f(x0+s,h,z0);
        // BELAKANG
        glColor3f(0.8, 0.0, 0.0);
        glVertex3f(x0,0,z0);
        glVertex3f(x0+s,0,z0);
        glVertex3f(x0+s,h,z0);
        glVertex3f(x0,h,z0);

        // KIRI
        glColor3f(0.7, 0.0, 0.0);
        glVertex3f(x0,0,z0);
        glVertex3f(x0,0,z0+s);
        glVertex3f(x0,h,z0+s);
        glVertex3f(x0,h,z0);
    }
    else
    {
        // 🔵 WALL (biru)
        // ATAS
        glColor3f(0.4, 0.7, 1.0);
        glVertex3f(x0, h, z0);
        glVertex3f(x0+s, h, z0);
        glVertex3f(x0+s, h, z0+s);
        glVertex3f(x0, h, z0+s);

        // DEPAN
        glColor3f(0.2, 0.5, 1.0);
        glVertex3f(x0,0,z0+s);
        glVertex3f(x0+s,0,z0+s);
        glVertex3f(x0+s,h,z0+s);
        glVertex3f(x0,h,z0+s);

        // KANAN
        glColor3f(0.1, 0.3, 0.8);
        glVertex3f(x0+s,0,z0);
        glVertex3f(x0+s,0,z0+s);
        glVertex3f(x0+s,h,z0+s);
        glVertex3f(x0+s,h,z0);

        // BELAKANG
        glColor3f(0.15, 0.4, 0.9);
        glVertex3f(x0,0,z0);
        glVertex3f(x0+s,0,z0);
        glVertex3f(x0+s,h,z0);
        glVertex3f(x0,h,z0);

        // KIRI
        glColor3f(0.1, 0.35, 0.85);
        glVertex3f(x0,0,z0);
        glVertex3f(x0,0,z0+s);
        glVertex3f(x0,h,z0+s);
        glVertex3f(x0,h,z0);
    }

    glEnd();
}
//5
void drawFloor3D()
{
    float w = WIDTH * CELL_SIZE;
    float d = HEIGHT * CELL_SIZE;

    glColor3f(0.7, 0.7, 0.7); // abu terang

    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(w,0,0);
    glVertex3f(w,0,d);
    glVertex3f(0,0,d);
    glEnd();
}

//6
bool isWall(float px, float py)
{
    int x = (int)px;
    int y = (int)py;

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return true;

    return maze.grid[y][x].wall;
}

// Inisialisasi OpenGL
void myinit()
{
    //12
    glEnable(GL_DEPTH_TEST);

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
    //10
    float nx = player.x + dx;
    float ny = player.y + dy;

    //7
    // cek apakah nabrak tembok
    float margin = 0.05f; // sedikit offset agar tidak nyangkut di tepi
    float size   = 1.0f - margin * 2; // ukuran efektif player

    // Cek 4 sudut bounding box player
    bool blocked =
        isWall(nx + margin,        ny + margin)        || // kiri-atas
        isWall(nx + margin + size, ny + margin)        || // kanan-atas
        isWall(nx + margin,        ny + margin + size) || // kiri-bawah
        isWall(nx + margin + size, ny + margin + size);   // kanan-bawah

    if (!blocked) {
        player.x = nx;
        player.y = ny;
    }

    // Refresh tampilan
    //11
    // glutDisplayFunc(display);
    glutPostRedisplay();
}

float speed = 0.2;

// Input keyboard
void keyboard(unsigned char key, int x, int y)
{
    printf("Key Pressed : %u \n",key);

    switch (key)
    {
    case 27: // ESC
    case 113: // q
        exit(0);
        break;

    //19
    case 119: // w (atas)
        movement_handler(0,-speed);
        break;

    case 97: // a (kiri)
        movement_handler(-speed,0);
        break;

    case 115: // s (bawah)
        movement_handler(0,speed);
        break;

    case 100: // d (kanan)
        movement_handler(speed,0);
        break;

    case 'c': // refresh maze
    case 'C':
        reInitMaze();
        break;

    //8
    case 'v':
    case 'V':
        is3D = !is3D;
        break;
    case 'j': case 'J': rotY -= 5.0f; break;
    case 'l': case 'L': rotY += 5.0f; break;

    case 'i': case 'I': rotX -= 5.0f; break;
    case 'k': case 'K': rotX += 5.0f; break;
    }
    //17
    if (rotY > 360.0f) rotY -= 360.0f;
    if (rotY < 0.0f)   rotY += 360.0f;
    glutPostRedisplay();

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

    //13
    // glutDisplayFunc(display);

    glutPostRedisplay();
}

// Main program
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    // Mode tampilan
    // glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    //9
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Ukuran window
    glutInitWindowSize(500, 500);

    // Judul window
    glutCreateWindow("OpenGL Maze Game - UTS");

    // Generate maze
    reInitMaze();

    
    // Set input keyboard
    //14
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    // Setup OpenGL
    myinit();

    // Loop utama
    glutMainLoop();

    return 0;
}
