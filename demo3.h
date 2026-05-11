#ifndef DEMO3_H
#define DEMO3_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

extern bool view3D;
extern float rotationX;
extern float rotationZ;
extern float nimRotation;

void draw_persegi(float x1, float y1, float z1,
                  float x2, float y2, float z2,
                  float x3, float y3, float z3,
                  float x4, float y4, float z4,
                  float alpha = 1.0f);

void draw_3d_kotak(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b, float alpha = 1.0f);

void drawNIM(float baseX, float baseY, float baseZ, float scale, bool is3D);

#endif
