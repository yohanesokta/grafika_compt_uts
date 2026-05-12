#include <GL/freeglut_std.h>
#include <GL/gl.h>
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

#include "demo3.h"
#include "maze.h"
#include <stdio.h>
#include <time.h>

Maze maze;
int nim3angle[3] = {0, 0, 1};

struct movement {
  float x;
  float y;
};

float speed = 1.0f;
void reInitMaze();

movement player{(float)(WIDTH / 2), (float)(HEIGHT - 1)};

movement c_nim{1.0, 1.0};

void display() {
  if (view3D) {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20, 20, -20, 20, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(rotationX, 1, 0, 0);
    glRotatef(rotationZ, 0, 0, 1);
    glTranslatef(-WIDTH * CELL_SIZE / 2.0, 0, HEIGHT * CELL_SIZE / 2.0);

    // maze
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        if (maze.grid[y][x].wall) {
          draw_3d_kotak(maze.grid[y][x].x1, 0, -maze.grid[y][x].y2,
                        maze.grid[y][x].x2, CELL_SIZE, -maze.grid[y][x].y1,
                        0.1f, 0.0f, 1.0f, 1.0f);
        }
      }
    }

    // player
    draw_3d_kotak(player.x * CELL_SIZE, 0.01, -(player.y + 1) * CELL_SIZE,
                  (player.x + 1) * CELL_SIZE, CELL_SIZE, -player.y * CELL_SIZE,
                  1.0f, 0.0f, 0.0f, 1.0f);

    // nim
    float baseX = c_nim.x * CELL_SIZE + CELL_SIZE / 2.0;
    float baseZ = c_nim.y * CELL_SIZE + CELL_SIZE / 2.0;
    glPushMatrix();
    glTranslatef(baseX, CELL_SIZE / 2.0, -baseZ);
    glRotatef(nimRotation, nim3angle[0], nim3angle[1], nim3angle[2]);
    drawNIM(0, 0, 0, 0.35, true);
    glPopMatrix();

    // floor
    glDepthMask(GL_FALSE);
    glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
    draw_persegi(0, -0.01, 0, WIDTH * CELL_SIZE, -0.01, 0, WIDTH * CELL_SIZE,
                 -0.01, -HEIGHT * CELL_SIZE, 0, -0.01, -HEIGHT * CELL_SIZE);
    glDepthMask(GL_TRUE);

  } else {
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH * CELL_SIZE, 0, HEIGHT * CELL_SIZE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.1, 0.0, 1.0);
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        if (maze.grid[y][x].wall) {
          glRecti(maze.grid[y][x].x1, maze.grid[y][x].y1, maze.grid[y][x].x2,
                  maze.grid[y][x].y2);
        }
      }
    }

    glColor3f(0.0, 0.0, 0.0);
    float scale = 0.5;
    float baseX = c_nim.x * CELL_SIZE + CELL_SIZE / 2;
    float baseY = c_nim.y * CELL_SIZE + CELL_SIZE / 2;
    drawNIM(baseX, baseY, 0, scale, false);

    glColor3f(1.0, 0.0, 0.0);
    glRectf(player.x * CELL_SIZE, player.y * CELL_SIZE,
            (player.x + 1) * CELL_SIZE, (player.y + 1) * CELL_SIZE);
  }
  glutSwapBuffers();
}

void myinit() {
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

bool checkCollision(float newX, float newY) {
  int gridX = (int)newX;
  int gridY = (int)newY;

  // limit
  if (gridX < 0 || gridX >= WIDTH || gridY < 0 || gridY >= HEIGHT) {
    return true;
  }

  return maze.grid[gridY][gridX].wall;
}

void movement_handler(float dx, float dy) {
  float newX = player.x + dx;
  float newY = player.y + dy;
  if (!checkCollision(newX, newY)) {
    player.x += dx;
    player.y += dy;
  }
  display();
}

const int KEY_ESC = 27;

void keyboard(unsigned char key, int x, int y) {
  printf("nilai key %d\n", key);
  if (key == KEY_ESC) {
    exit(0);
  }
  switch (key) {
  case 'w':
    movement_handler(0, speed);
    break;

  case 'a':
    movement_handler(-speed, 0);
    break;

  case 's':
    movement_handler(0, -speed);
    break;

  case 'd':
    movement_handler(speed, 0);
    break;

  case 'v':
  case 'V':
    view3D = !view3D;
    display();
    break;

  case 'i':
  case 'I':
    if (view3D) {
      rotationX += 5.0f;
      display();
    }
    break;

  case 'k':
  case 'K':
    if (view3D) {
      rotationX -= 5.0f;
      display();
    }
    break;

  case 'j':
  case 'J':
    if (view3D) {
      rotationZ -= 5.0f;
      display();
    }
    break;

  case 'l':
  case 'L':
    if (view3D) {
      rotationZ += 5.0f;
      display();
    }
    break;

  case 'c':
  case 'C':
    reInitMaze();
    break;
  }
}
void idleFuction() {
  if (view3D) {
    nimRotation += 2.0f;
    if (nimRotation > 360)
      nimRotation -= 360;
    display();
  }
}

float randomFloat() { return (float)rand() / RAND_MAX; }

void reInitMaze() {
  srand(time(NULL));

  initMaze(&maze);
  divide(&maze, 0, 0, WIDTH, HEIGHT);

  int x, y;

  do {
    x = rand() % WIDTH;
    y = rand() % HEIGHT;
  } while (maze.grid[y][x].wall || x == 0 || x == WIDTH - 1 || y == 0 ||
           y == HEIGHT - 1);

  c_nim.x = x;
  c_nim.y = y;

  player.x = WIDTH / 2;
  player.y = HEIGHT - 1;
  display();
}

void mouse_handler(int button, int state, int x, int y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    nim3angle[0] = 1;
    nim3angle[1] = 0;
    nim3angle[2] = 0;
    break;
  case GLUT_RIGHT_BUTTON:
    nim3angle[0] = 0;
    nim3angle[1] = 1;
    nim3angle[2] = 0;
    break;
  case GLUT_MIDDLE_BUTTON:
    nim3angle[0] = 0;
    nim3angle[1] = 0;
    nim3angle[2] = 1;
    break;
  }
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("OpenGL Maze Game - UTS");
  reInitMaze();
  glutDisplayFunc(display);
  glutMouseFunc(mouse_handler);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idleFuction);
  myinit();
  glutMainLoop();
  return 0;
}
