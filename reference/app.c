#include <math.h>
#include <stdbool.h>
#include <stdio.h>

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

// Beberapa Teori Makro Ada di Documentasi, atau coba baca saja dari internet
// apa itu makro pada bahasa C. ini basic c yang berhubungan dalam bahasa
// pemrogramanya langsung. Memang seharusnya kita belajar C dulu baru OpenGLglut

#define WIDTH 800
#define HEIGH 800
#define WINDOW_TITLE "Game Testing"
#define SIZE 15
#define VIEW_SPEED 0.03

int state_maze = 0;
int viewMode = 0;
int playerX = 7;
int playerY = 0;
int maze_rotation_x = 0;
int maze_rotation_y = 0;
int winWidth = WIDTH;
int winHeight = HEIGH;
int isNight = 0;
int ambientOn = 1;
int diffuseOn = 1;
int specularOn = 1;
int transparencyOn = 0;

// Informasi Tentang Movement Camera Sesuai Modul Pak Yonathan
// Cuman 2 itu loh ya teori kamera kalo list bawahnya itu buat maze
// Jangan Lupa Baca Cuy Docs  nya we cape nulisnya
//
float pos[3] = {0.0, 0.0, 10.0};
GLdouble viewdir[] = {0.0, 0.0, -1.0};

int maze[SIZE][SIZE] = {{1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
                        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                        {1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
                        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
                        {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
                        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                        {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
                        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                        {1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1},
                        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1}};

int maze2[SIZE][SIZE] = {{1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                         {1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1},
                         {1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
                         {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
                         {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                         {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
                         {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
                         {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                         {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
                         {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                         {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                         {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1}};

void updateProjection(int width, int height) {
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (viewMode == 2) {
    // First person
    gluPerspective(65.0f, (float)width / (float)height, 1.0f, 100.0f);
  } else {
    // Top view & rotate view
    float size = 18.0f;

    float aspect = (float)width / (float)height;

    if (aspect >= 1.0f) {
      glOrtho(-size * aspect / 2, size * aspect / 2, size / 2, -size / 2, -100,
              100);
    } else {
      glOrtho(-size / 2, size / 2, size / (2 * aspect), -size / (2 * aspect),
              -100, 100);
    }
  }
  glMatrixMode(GL_MODELVIEW);
}

void reshape(int width, int height) {
  winWidth = width;
  winHeight = height;
  updateProjection(width, height);
}

void myinit() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void draw_cube(float x1, float y1, float z1, float x2, float y2, float z2,
               float r, float g, float b, float a) {
  glColor4f(r, g, b, a);
  glBegin(GL_QUADS);
  // Front face
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(x1, y1, z2);
  glVertex3f(x2, y1, z2);
  glVertex3f(x2, y2, z2);
  glVertex3f(x1, y2, z2);

  // Back face
  glNormal3f(0.0, 0.0, -1.0);
  glVertex3f(x1, y1, z1);
  glVertex3f(x2, y1, z1);
  glVertex3f(x2, y2, z1);
  glVertex3f(x1, y2, z1);

  // Left face
  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3f(x1, y1, z1);
  glVertex3f(x1, y1, z2);
  glVertex3f(x1, y2, z2);
  glVertex3f(x1, y2, z1);

  // Right face
  glNormal3f(1.0, 0.0, 0.0);
  glVertex3f(x2, y1, z1);
  glVertex3f(x2, y1, z2);
  glVertex3f(x2, y2, z2);
  glVertex3f(x2, y2, z1);

  // Top face
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(x1, y2, z1);
  glVertex3f(x1, y2, z2);
  glVertex3f(x2, y2, z2);
  glVertex3f(x2, y2, z1);

  // Bottom face
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(x1, y1, z1);
  glVertex3f(x1, y1, z2);
  glVertex3f(x2, y1, z2);
  glVertex3f(x2, y1, z1);
  glEnd();
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

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  if (viewMode == 2) {
    gluLookAt(pos[0], pos[1], pos[2], pos[0] + viewdir[0], pos[1] + viewdir[1],
              pos[2] + viewdir[2], 0.0, 1.0, 0.0);
  }

  glEnable(GL_DEPTH_TEST);

  glRotatef(maze_rotation_x, 0, 1, 0);
  glRotatef(maze_rotation_y, 1, 0, 0);

  glTranslatef(-SIZE / 2, -SIZE / 2, 0);

  // Draw Lantai awal awal
  draw_cube(-0.5, -0.5, -0.5, SIZE - 0.5, SIZE - 0.5, -0.52, 0.0, 1.0, 0.0,
            0.6);

  // Draw maze walls
  float wallAlpha = transparencyOn ? 0.7f : 1.0f;
  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      if (state_maze == 0) {
        if (maze[y][x] == 1) {
          draw_cube(x - 0.5, y - 0.5, -0.5, x + 0.5, y + 0.5, 0.5, 0.0, 0.0,
                    1.0, wallAlpha);
        }
      } else {
        if (maze2[y][x] == 1) {
          draw_cube(x - 0.5, y - 0.5, -0.5, x + 0.5, y + 0.5, 0.5, 0.0, 0.0,
                    1.0, wallAlpha);
        }
      }
    }
  }

  // Draw player nya
  draw_cube(playerX - 0.4, playerY - 0.4, -0.4, playerX + 0.4, playerY + 0.4,
            0.4, 1.0, 0.0, 0.0, 1.0);
  glutSwapBuffers();
}
struct {
  bool ispress;
  bool isup;
} charkeys[256];

int specialKey[1024];

void special_down_handle(int key, int x, int y) { specialKey[key] = 1; }
void special_up_handle(int key, int x, int y) { specialKey[key] = 0; }

void key_up_handle(unsigned char key, int x, int y) {
  charkeys[key].ispress = false;
  charkeys[key].isup = false;
}

void key_down_handle(unsigned char key) {
  if (!charkeys[key].ispress) {
    charkeys[key].ispress = true;
    charkeys[key].isup = true;
  } else {
    charkeys[key].isup = false;
  }
}

void keyboard_handler(unsigned char key, int x, int y) {
  int playerGOY = playerY;
  int playerGOX = playerX;
  key_down_handle(key);
  switch (key) {
  case '1':
    ambientOn = !ambientOn;
    break;
  case '2':
    diffuseOn = !diffuseOn;
    break;
  case '3':
    specularOn = !specularOn;
    break;
  case '4':
    isNight = !isNight;
    break;
  case '5':
    transparencyOn = !transparencyOn;
    break;
  case 'c':
    if (state_maze == 0) {
      state_maze = 1;
    } else {
      state_maze = 0;
    }
    break;
  case 'w':
    playerGOY -= 1;
    break;
  case 's':
    playerGOY += 1;
    break;
  case 'a':
    playerGOX -= 1;
    break;
  case 'd':
    playerGOX += 1;
    break;
  case 'v':
    viewMode++;
    if (viewMode > 2) {
      viewMode = 0;
    }
    printf("view =%d\n", viewMode);
    updateProjection(winWidth, winHeight);
    if (viewMode == 0) {
      maze_rotation_x = 0;
      maze_rotation_y = 0;
    } else if (viewMode == 1) {
      maze_rotation_y = 45;
    } else if (viewMode == 2) {
      maze_rotation_y = 270;
      maze_rotation_x = 0;
    }
    break;
  }
  if (maze[playerGOY][playerGOX] == 0) {
    playerX = playerGOX;
    playerY = playerGOY;
  }
  display();
}

void idlefunc() {
  if (viewMode == 1) {
    if (charkeys[108].ispress) {
      maze_rotation_x += 2;
    }
    if (charkeys[106].ispress) {
      maze_rotation_x -= 2;
    }
    if (charkeys[105].ispress) {
      maze_rotation_y -= 2;
    }
    if (charkeys[107].ispress) {
      maze_rotation_y += 2;
    }
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

// Teori Keyboard Handler Yaang Kupakai
// Sebuah Fungsi ada yang menerima input key bernama glutKeyboardUpFunc nah itu
// tapi ada delay nya loh ya.. di setiap penekanan Ada fungsi lagi namanya
// glutKeyboardUpFunc itu fungsi berjalan ketika keyboard dilepas nah aku
// gabungkan pada variable key ketika key ditekan maka :
// keys[key_apa_yang_ditekan] = true dan ketika di lepas akan jadi false
//
// contoh : selama key 'W' di tekan dan belum di lepas akan bernilai true
// jika sekali aja melepasnya maka 'W' jadi false . jadinya menciptakan realtime
// keyboard execution pada keyboard
//
// istimewanya aku dapat handler pada glutIdle dimana itu akan me redraw setiap
// saat. (Mulussss) sesuai fps yang di dapat jadi aku cek apakah
// keys['keys_tertentu'] itu true atau false misal : keys['W'] == true | jika
// iya maka player ku majukan
//
// itu mulusss banget
//
int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGH);
  glutCreateWindow(WINDOW_TITLE);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard_handler);
  glutKeyboardUpFunc(key_up_handle);
  glutSpecialFunc(special_down_handle);
  glutSpecialUpFunc(special_up_handle);
  glutIdleFunc(idlefunc);
  glutIgnoreKeyRepeat(1);
  myinit();
  glutMainLoop();
  return 0;
}
