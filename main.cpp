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
void drawWall3D(
    float xBawah, float yBawah, float zBawah,
    float xAtas,  float yAtas,  float zAtas,
    bool isPlayer);

void drawFloor3D();

//25
void drawSegment(float x1, float y1, float x2, float y2, float thick);
void drawDigit7Seg(int digit, float ox, float oy, float w, float h, float thick);
void drawNIM2D();
//28
void drawDigit7Seg3D(int digit, float ox, float oy, float oz, float s);
void drawNIM3D();
void buildbox(float xBawah, float yBawah, float zBawah,float xAtas, float yAtas, float zAtas);

// Posisi player (mulai di tengah atas)
//26
movement player {(float)(WIDTH/2), 0.0f};

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

        //24
        drawNIM2D();
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

        //20
        gluLookAt(40, 40, 40,   // posisi kamera tetap
                cx, 0.0, cz,  // lihat ke tengah maze
                0.0, 1.0, 0.0);

        glTranslatef(cx, 0.0f, cz); // pindah ke tengah maze
        glRotatef(rotY, 0.0f, 1.0f, 0.0f); // rotasi kiri-kanan
        glRotatef(rotX, 1.0f, 0.0f, 0.0f); // rotasi atas-bawah
        glTranslatef(-cx, 0.0f, -cz); // balik lagi

        //21
        // WALL dulu
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                if (maze.grid[y][x].wall)
                {
                    drawWall3D(
                        x * CELL_SIZE,
                        0.0f,
                        y * CELL_SIZE,

                        (x + 1) * CELL_SIZE,
                        2.5f,
                        (y + 1) * CELL_SIZE,

                        false
                    );
                }
            }
        }

        // PLAYER
        drawWall3D(
            player.x * CELL_SIZE,
            0.0f,
            player.y * CELL_SIZE,

            (player.x + 1) * CELL_SIZE,
            1.5f,
            (player.y + 1) * CELL_SIZE,

            true
        );

        // TRANSPARAN
        glDepthMask(GL_FALSE);  
        drawFloor3D();
        drawNIM3D();
        glDepthMask(GL_TRUE);
    }

    glutSwapBuffers();
}

//4
void drawWall3D(
    float xBawah, float yBawah, float zBawah,
    float xAtas,  float yAtas,  float zAtas,
    bool isPlayer)
{
    glBegin(GL_QUADS);

    if (isPlayer)
    {
        glColor3f(1.0, 0.0, 0.0);
    }
    else
    {
        glColor3f(0.1, 0.3, 1.0);
    }

    // DEPAN
    glVertex3f(xBawah, yBawah, zBawah);
    glVertex3f(xBawah, yAtas,  zBawah);
    glVertex3f(xAtas,  yAtas,  zBawah);
    glVertex3f(xAtas,  yBawah, zBawah);

    // BELAKANG
    glVertex3f(xAtas,  yAtas,  zAtas);
    glVertex3f(xAtas,  yBawah, zAtas);
    glVertex3f(xBawah, yBawah, zAtas);
    glVertex3f(xBawah, yAtas,  zAtas);

    // KIRI
    glVertex3f(xBawah, yBawah, zBawah);
    glVertex3f(xBawah, yAtas,  zBawah);
    glVertex3f(xBawah, yAtas,  zAtas);
    glVertex3f(xBawah, yBawah, zAtas);

    // KANAN
    glVertex3f(xAtas, yBawah, zBawah);
    glVertex3f(xAtas, yAtas,  zBawah);
    glVertex3f(xAtas, yAtas,  zAtas);
    glVertex3f(xAtas, yBawah, zAtas);

    // BAWAH
    glVertex3f(xBawah, yBawah, zBawah);
    glVertex3f(xBawah, yBawah, zAtas);
    glVertex3f(xAtas,  yBawah, zAtas);
    glVertex3f(xAtas,  yBawah, zBawah);

    // ATAS
    glVertex3f(xBawah, yAtas, zBawah);
    glVertex3f(xBawah, yAtas, zAtas);
    glVertex3f(xAtas,  yAtas, zAtas);
    glVertex3f(xAtas,  yAtas, zBawah);

    glEnd();
}
//5
void drawFloor3D()
{
    float w = WIDTH * CELL_SIZE;
    float d = HEIGHT * CELL_SIZE;

    glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // abu terang

    glBegin(GL_QUADS);
    glVertex3f(0,-0.01,0);
    glVertex3f(w,-0.01,0);
    glVertex3f(w,-0.01,d);
    glVertex3f(0,-0.01,d);
    glEnd();
}
//23
void drawSegment(float x1, float y1, float x2, float y2, float thick)
{
    float dx = x2 - x1, dy = y2 - y1;
    float len = sqrt(dx*dx + dy*dy);
    float nx = -dy/len * thick, ny = dx/len * thick;

    glBegin(GL_QUADS);
    glVertex2f(x1+nx, y1+ny);
    glVertex2f(x1-nx, y1-ny);
    glVertex2f(x2-nx, y2-ny);
    glVertex2f(x2+nx, y2+ny);
    glEnd();
}

void drawDigit7Seg(int digit, float ox, float oy, float w, float h, float thick)
{
    bool seg[10][7] = {
    //  a      b      c      d      e      f      g
        {true,  true,  true,  true,  true,  true,  false}, // 0
        {false, true,  true,  false, false, false, false}, // 1
        {true,  true,  false, true,  true,  false, true},  // 2
        {true,  true,  true,  true,  false, false, true},  // 3
        {false, true,  true,  false, false, true,  true},  // 4
        {true,  false, true,  true,  false, true,  true},  // 5
        {true,  false, true,  true,  true,  true,  true},  // 6
        {true,  true,  true,  false, false, false, false}, // 7
        {true,  true,  true,  true,  true,  true,  true},  // 8
        {true,  true,  true,  true,  false, true,  true},  // 9
    };

    glColor3f(1.0f, 0.2f, 0.0f);

    float mid = oy + h / 2.0f;

    if (seg[digit][0]) drawSegment(ox,   oy,  ox+w, oy,  thick); // a atas
    if (seg[digit][1]) drawSegment(ox+w, oy,  ox+w, mid, thick); // b kanan-atas
    if (seg[digit][2]) drawSegment(ox+w, mid, ox+w, oy+h,thick); // c kanan-bawah
    if (seg[digit][3]) drawSegment(ox,   oy+h,ox+w, oy+h,thick); // d bawah
    if (seg[digit][4]) drawSegment(ox,   mid, ox,   oy+h,thick); // e kiri-bawah
    if (seg[digit][5]) drawSegment(ox,   oy,  ox,   mid, thick); // f kiri-atas
    if (seg[digit][6]) drawSegment(ox,   mid, ox+w, mid, thick); // g tengah
}

void drawNIM2D()
{
    float cellPx = c_nim.x * CELL_SIZE;
    float cellPy = c_nim.y * CELL_SIZE;

    float margin = CELL_SIZE * 0.02f;
    float digitW = CELL_SIZE * 0.27f;
    float digitH = CELL_SIZE * 0.75f;
    float gap    = CELL_SIZE * 0.05f;
    float thick  = CELL_SIZE * 0.06f;
    float startY = cellPy + (CELL_SIZE - digitH) / 2.0f;

    drawDigit7Seg(0, cellPx + margin,                    startY, digitW, digitH, thick);
    drawDigit7Seg(7, cellPx + margin + (digitW+gap),     startY, digitW, digitH, thick);
    drawDigit7Seg(6, cellPx + margin + (digitW+gap)*2,   startY, digitW, digitH, thick);
}

//29
void buildbox (float xBawah, float yBawah, float zBawah,
               float xAtas, float yAtas, float zAtas)
{
    // depan
    glBegin(GL_POLYGON);
    glVertex3f(xBawah, yBawah, zBawah);
    glVertex3f(xBawah, yAtas, zBawah);
    glVertex3f(xAtas, yAtas, zBawah);
    glVertex3f(xAtas, yBawah, zBawah);
    glEnd();

    // belakang
    glBegin(GL_POLYGON);
    glVertex3f(xAtas, yAtas, zAtas);
    glVertex3f(xAtas, yBawah, zAtas);
    glVertex3f(xBawah, yBawah, zAtas);
    glVertex3f(xBawah, yAtas, zAtas);
    glEnd();

    // kiri
    glBegin(GL_POLYGON);
    glVertex3f(xBawah, yBawah, zBawah);
    glVertex3f(xBawah, yAtas, zBawah);
    glVertex3f(xBawah, yAtas, zAtas);
    glVertex3f(xBawah, yBawah, zAtas);
    glEnd();

    // kanan
    glBegin(GL_POLYGON);
    glVertex3f(xAtas, yBawah, zBawah);
    glVertex3f(xAtas, yAtas, zBawah);
    glVertex3f(xAtas, yAtas, zAtas);
    glVertex3f(xAtas, yBawah, zAtas);
    glEnd();

    // bawah
    glBegin(GL_POLYGON);
    glVertex3f(xBawah, yBawah, zBawah);
    glVertex3f(xBawah, yBawah, zAtas);
    glVertex3f(xAtas, yBawah, zAtas);
    glVertex3f(xAtas, yBawah, zBawah);
    glEnd();

    // atas
    glBegin(GL_POLYGON);
    glVertex3f(xBawah, yAtas, zBawah);
    glVertex3f(xBawah, yAtas, zAtas);
    glVertex3f(xAtas, yAtas, zAtas);
    glVertex3f(xAtas, yAtas, zBawah);
    glEnd();
}

void drawDigit7Seg3D(int digit, float ox, float oy, float oz, float s)
{
    bool seg[10][7] = {
        {1,1,1,1,1,1,0},
        {0,1,1,0,0,0,0},
        {1,1,0,1,1,0,1},
        {1,1,1,1,0,0,1},
        {0,1,1,0,0,1,1},
        {1,0,1,1,0,1,1},
        {1,0,1,1,1,1,1},
        {1,1,1,0,0,0,0},
        {1,1,1,1,1,1,1},
        {1,1,1,1,0,1,1}
    };

    float w = s;
    float h = s * 2.0f;

    float t = s * 0.2f;

    // tipis ke atas
    float depthY = 0.2f;

    glColor3f(1.0f, 0.3f, 0.0f);

    // ======================
    // SEKARANG Z DIBALIK
    // ======================

    // a (atas)
    if(seg[digit][0])
        buildbox(
            ox, oy, oz,
            ox+w, oy+depthY, oz+t
        );

    // b
    if(seg[digit][1])
        buildbox(
            ox+w, oy, oz,
            ox+w+t, oy+depthY, oz+h/2
        );

    // c
    if(seg[digit][2])
        buildbox(
            ox+w, oy, oz+h/2,
            ox+w+t, oy+depthY, oz+h
        );

    // d (bawah)
    if(seg[digit][3])
        buildbox(
            ox, oy, oz+h,
            ox+w, oy+depthY, oz+h+t
        );

    // e
    if(seg[digit][4])
        buildbox(
            ox-t, oy, oz+h/2,
            ox, oy+depthY, oz+h
        );

    // f
    if(seg[digit][5])
        buildbox(
            ox-t, oy, oz,
            ox, oy+depthY, oz+h/2
        );

    // g
    if(seg[digit][6])
        buildbox(
            ox, oy, oz+h/2,
            ox+w, oy+depthY, oz+h/2+t
        );
}

void drawNIM3D()
{
    float scale = CELL_SIZE * 0.18f;
    float gap   = CELL_SIZE * 0.30f;

    // total panjang 3 digit
    float totalWidth = gap * 2 + scale;

    // posisi tengah cell maze
    float x = c_nim.x * CELL_SIZE + CELL_SIZE/2 - totalWidth/2;
    float z = c_nim.y * CELL_SIZE + CELL_SIZE/2 - scale;

    // sedikit di atas lantai
    float y = 1.0f;

    drawDigit7Seg3D(0, x, y, z, scale);
    drawDigit7Seg3D(7, x + gap, y, z, scale);
    drawDigit7Seg3D(6, x + gap * 2, y, z, scale);
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
    //27
    player.x = WIDTH / 2;
    player.y = 0;

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
