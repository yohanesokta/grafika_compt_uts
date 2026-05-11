#include "demo3.h"

bool view3D = false;
float rotationX = 60.0f;
float rotationZ = 0.0f;
float nimRotation = 0.0f;

void draw_persegi(float x1, float y1, float z1,
                  float x2, float y2, float z2,
                  float x3, float y3, float z3,
                  float x4, float y4, float z4) {
    glBegin(GL_QUADS);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    glEnd();
}

void draw_3d_kotak(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b, float alpha) {
    // Top face
    glColor4f(r * 1.0f, g * 1.0f, b * 1.0f, alpha);
    draw_persegi(x1, y2, z1, x1, y2, z2, x2, y2, z2, x2, y2, z1);

    // Bottom face
    glColor4f(r * 0.4f, g * 0.4f, b * 0.4f, alpha);
    draw_persegi(x1, y1, z1, x2, y1, z1, x2, y1, z2, x1, y1, z2);

    // Front face
    glColor4f(r * 0.8f, g * 0.8f, b * 0.8f, alpha);
    draw_persegi(x1, y1, z2, x2, y1, z2, x2, y2, z2, x1, y2, z2);

    // Back face
    glColor4f(r * 0.6f, g * 0.6f, b * 0.6f, alpha);
    draw_persegi(x1, y1, z1, x1, y2, z1, x2, y2, z1, x2, y1, z1);

    // Left face
    glColor4f(r * 0.7f, g * 0.7f, b * 0.7f, alpha);
    draw_persegi(x1, y1, z1, x1, y1, z2, x1, y2, z2, x1, y2, z1);

    // Right face
    glColor4f(r * 0.5f, g * 0.5f, b * 0.5f, alpha);
    draw_persegi(x2, y1, z1, x2, y2, z1, x2, y2, z2, x2, y1, z2);
}

void drawNIM(float baseX, float baseY, float baseZ, float scale, bool is3D) {
    if (is3D) {
        draw_3d_kotak(baseX + (-1.8) * scale, baseY + (1.6) * scale, baseZ - 0.2f * scale, baseX + (-1.0) * scale, baseY + (1.8) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
        draw_3d_kotak(baseX + (-1.8) * scale, baseY + (-1.8) * scale, baseZ - 0.2f * scale, baseX + (-1.0) * scale, baseY + (-1.6) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
        draw_3d_kotak(baseX + (-1.8) * scale, baseY + (-1.6) * scale, baseZ - 0.2f * scale, baseX + (-1.6) * scale, baseY + (1.6) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
        draw_3d_kotak(baseX + (-1.2) * scale, baseY + (-1.6) * scale, baseZ - 0.2f * scale, baseX + (-1.0) * scale, baseY + (1.6) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);

        draw_3d_kotak(baseX + (-0.8) * scale, baseY + (1.6) * scale, baseZ - 0.2f * scale, baseX + (0.6) * scale, baseY + (1.8) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
        draw_3d_kotak(baseX + (0.4) * scale, baseY + (-1.8) * scale, baseZ - 0.2f * scale, baseX + (0.6) * scale, baseY + (1.6) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);

        draw_3d_kotak(baseX + (0.8) * scale, baseY + (1.6) * scale, baseZ - 0.2f * scale, baseX + (1.8) * scale, baseY + (1.8) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
        draw_3d_kotak(baseX + (0.8) * scale, baseY + (-1.8) * scale, baseZ - 0.2f * scale, baseX + (1.0) * scale, baseY + (1.6) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
        draw_3d_kotak(baseX + (1.0) * scale, baseY + (-1.8) * scale, baseZ - 0.2f * scale, baseX + (1.8) * scale, baseY + (-1.6) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
        draw_3d_kotak(baseX + (1.6) * scale, baseY + (-1.6) * scale, baseZ - 0.2f * scale, baseX + (1.8) * scale, baseY + (0.0) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
        draw_3d_kotak(baseX + (1.0) * scale, baseY + (-0.2) * scale, baseZ - 0.2f * scale, baseX + (1.6) * scale, baseY + (0.0) * scale, baseZ + 0.2f * scale, 0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        glColor3f(0.0, 0.0, 0.0);
        glRectf(baseX + (-1.8) * scale, baseY + (1.6) * scale, baseX + (-1.0) * scale, baseY + (1.8) * scale);
        glRectf(baseX + (-1.8) * scale, baseY + (-1.8) * scale, baseX + (-1.0) * scale, baseY + (-1.6) * scale);
        glRectf(baseX + (-1.8) * scale, baseY + (-1.6) * scale, baseX + (-1.6) * scale, baseY + (1.6) * scale);
        glRectf(baseX + (-1.2) * scale, baseY + (-1.6) * scale, baseX + (-1.0) * scale, baseY + (1.6) * scale);

        glRectf(baseX + (-0.8) * scale, baseY + (1.6) * scale, baseX + (0.6) * scale, baseY + (1.8) * scale);
        glRectf(baseX + (0.4) * scale, baseY + (-1.8) * scale, baseX + (0.6) * scale, baseY + (1.6) * scale);

        glRectf(baseX + (0.8) * scale, baseY + (1.6) * scale, baseX + (1.8) * scale, baseY + (1.8) * scale);
        glRectf(baseX + (0.8) * scale, baseY + (-1.8) * scale, baseX + (1.0) * scale, baseY + (1.6) * scale);
        glRectf(baseX + (1.0) * scale, baseY + (-1.8) * scale, baseX + (1.8) * scale, baseY + (-1.6) * scale);
        glRectf(baseX + (1.6) * scale, baseY + (-1.6) * scale, baseX + (1.8) * scale, baseY + (0.0) * scale);
        glRectf(baseX + (1.0) * scale, baseY + (-0.2) * scale, baseX + (1.6) * scale, baseY + (0.0) * scale);
    }
}
