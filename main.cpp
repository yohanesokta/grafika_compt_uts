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
#include <math.h>
#include <stdio.h>
#include <time.h>

#define VIEW_SPEED 0.03

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

int viewMode = 0;
int maze_rotation_x = 0;
int maze_rotation_y = 0;
int winWidth = 500;
int winHeight = 500;

float pos[3] = {0.0, 0.0, -15.0};
float viewdir[3] = {0.0, 0.0, 2};
int specialKey[1024];
int isNight = 0;
int ambientOn = 1;
int diffuseOn = 1;
int specularOn = 1;

void special_down_handle(int key, int x, int y) { specialKey[key] = 1; }
void special_up_handle(int key, int x, int y) { specialKey[key] = 0; }

void reshape(int width, int height) {
  winWidth = width;
  winHeight = height;
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (viewMode == 2) {
    gluPerspective(65.0f, (float)width / (float)height, 1.0f, 100.0f);
  } else {
    float size = 40.0f; // Size to fit our 30x30 maze
    float aspect = (float)width / (float)height;

    if (aspect >= 1.0f) {
      glOrtho(-size * aspect / 2, size * aspect / 2, -size / 2, size / 2, -100,
              100);
    } else {
      glOrtho(-size / 2, size / 2, -size / (2 * aspect), size / (2 * aspect),
              -100, 100);
    }
  }
  glMatrixMode(GL_MODELVIEW);
}

void display() {
  GLfloat amb[4], diff[4], spec[4];

  if (isNight) {
    glClearColor(0.05, 0.05, 0.1, 1.0);
    for (int i = 0; i < 3; i++) {
      amb[i] = 0.1f;
      diff[i] = 0.3f;
      spec[i] = 0.3f;
    }
  } else {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    for (int i = 0; i < 3; i++) {
      amb[i] = 0.3f;
      diff[i] = 1.0f;
      spec[i] = 1.0f;
    }
  }
  amb[3] = diff[3] = spec[3] = 1.0f;

  if (!ambientOn) {
    amb[0] = amb[1] = amb[2] = 0.0f;
  }
  if (!diffuseOn) {
    diff[0] = diff[1] = diff[2] = 0.0f;
  }
  if (!specularOn) {
    spec[0] = spec[1] = spec[2] = 0.0f;
  }

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  if (viewMode == 2) {
    gluLookAt(pos[0], pos[1], pos[2], pos[0] + viewdir[0], pos[1] + viewdir[1],
              pos[2] + viewdir[2], 0.0, 1.0, 0.0);
  }

  glRotatef(maze_rotation_x, 0, 1, 0);
  glRotatef(maze_rotation_y, 1, 0, 0);
  glTranslatef(-WIDTH * CELL_SIZE / 2.0, -HEIGHT * CELL_SIZE / 2.0, 0.0);

  // maze
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (maze.grid[y][x].wall) {
        draw_3d_kotak(maze.grid[y][x].x1, maze.grid[y][x].y1, -CELL_SIZE / 2.0,
                      maze.grid[y][x].x2, maze.grid[y][x].y2, CELL_SIZE / 2.0,
                      0.1f, 0.0f, 1.0f, 1.0f);
      }
    }
  }

  // player
  draw_3d_kotak(player.x * CELL_SIZE + 0.2f, player.y * CELL_SIZE + 0.2f,
                -CELL_SIZE / 2.0 + 0.2f, (player.x + 1) * CELL_SIZE - 0.2f,
                (player.y + 1) * CELL_SIZE - 0.2f, CELL_SIZE / 2.0 - 0.2f, 1.0f,
                0.0f, 0.0f, 1.0f);

  // nim
  float baseX = c_nim.x * CELL_SIZE + CELL_SIZE / 2.0;
  float baseY = c_nim.y * CELL_SIZE + CELL_SIZE / 2.0;
  glPushMatrix();
  glTranslatef(baseX, baseY, 0.0f);
  glRotatef(nimRotation, nim3angle[0], nim3angle[1], nim3angle[2]);
  drawNIM(0, 0, 0, 0.35, true);
  glPopMatrix();

  // floor
  glDepthMask(GL_FALSE);
  glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
  draw_persegi(0, 0, -CELL_SIZE / 2.0 - 0.01, WIDTH * CELL_SIZE, 0,
               -CELL_SIZE / 2.0 - 0.01, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE,
               -CELL_SIZE / 2.0 - 0.01, 0, HEIGHT * CELL_SIZE,
               -CELL_SIZE / 2.0 - 0.01);
  glDepthMask(GL_TRUE);

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
  glShadeModel(GL_SMOOTH);

  // Lighting setup
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
  GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  GLfloat specReflection[] = {1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
  glMateriali(GL_FRONT, GL_SHININESS, 64);
}

bool checkCollision(float newX, float newY) {
  int gridX = (int)newX;
  int gridY = (int)newY;

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
  case '1':
    ambientOn = !ambientOn;
    display();
    break;

  case '2':
    diffuseOn = !diffuseOn;
    display();
    break;

  case '3':
    specularOn = !specularOn;
    display();
    break;

  case '4':
    isNight = !isNight;
    display();
    break;

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
    viewMode++;
    if (viewMode > 2) {
      viewMode = 0;
    }
    reshape(winWidth, winHeight);
    if (viewMode == 0) {
      maze_rotation_x = 0;
      maze_rotation_y = 0;
    } else if (viewMode == 1) {
      maze_rotation_y = 330;
    } else if (viewMode == 2) {
      maze_rotation_y = 270;
      maze_rotation_x = 0;
    }
    display();
    break;

  case 'i':
  case 'I':
    if (viewMode == 1) {
      maze_rotation_y -= 5;
      display();
    }
    break;

  case 'k':
  case 'K':
    if (viewMode == 1) {
      maze_rotation_y += 5;
      display();
    }
    break;

  case 'j':
  case 'J':
    if (viewMode == 1) {
      maze_rotation_x -= 5;
      display();
    }
    break;

  case 'l':
  case 'L':
    if (viewMode == 1) {
      maze_rotation_x += 5;
      display();
    }
    break;

  case 'c':
  case 'C':
    reInitMaze();
    break;
  case 'h':
    printf("pos :\n%f x %f x %f \n\n viewdir : \nn%f x %f x %f \n", pos[0],
           pos[1], pos[2], viewdir[0], viewdir[1], viewdir[2]);
    break;
  }
}
void idleFuction() {
  if (viewMode == 0) {
    nimRotation = 0;
  } else {
    nimRotation += 2.0f;
    if (nimRotation > 360)
      nimRotation -= 360;
  }

  if (viewMode == 2) {

    if (specialKey[GLUT_KEY_UP]) {
      pos[0] += 0.2 * viewdir[0];
      pos[1] += 0.2 * viewdir[1];
      pos[2] += 0.2 * viewdir[2];
    }
    if (specialKey[GLUT_KEY_DOWN]) {
      pos[0] -= 0.2 * viewdir[0];
      pos[1] -= 0.2 * viewdir[1];
      pos[2] -= 0.2 * viewdir[2];
    }

    if (specialKey[GLUT_KEY_LEFT]) {
      viewdir[0] = viewdir[0] * cos(VIEW_SPEED) + viewdir[2] * sin(VIEW_SPEED);
      viewdir[2] = -viewdir[0] * sin(VIEW_SPEED) + viewdir[2] * cos(VIEW_SPEED);
    }
    if (specialKey[GLUT_KEY_RIGHT]) {
      viewdir[0] =
          viewdir[0] * cos(-VIEW_SPEED) + viewdir[2] * sin(-VIEW_SPEED);
      viewdir[2] =
          -viewdir[0] * sin(-VIEW_SPEED) + viewdir[2] * cos(-VIEW_SPEED);
    }
  }

  display();
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
  glutReshapeFunc(reshape);
  glutSpecialFunc(special_down_handle);
  glutSpecialUpFunc(special_up_handle);
  glutIgnoreKeyRepeat(1);
  glutMouseFunc(mouse_handler);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idleFuction);
  myinit();
  glutMainLoop();
  return 0;
}
