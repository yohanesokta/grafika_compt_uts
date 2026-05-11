#include "demo3.h"

bool view3D = false;
float rotationX = 60.0f;
float rotationZ = 0.0f;
float nimRotation = 0.0f;

void draw_persegi(float x1, float y1, float z1,
                  float x2, float y2, float z2,
                  float x3, float y3, float z3,
                  float x4, float y4, float z4,
                  float alpha) {
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
    draw_persegi(x1, y2, z1, x1, y2, z2, x2, y2, z2, x2, y2, z1, alpha);

    // Bottom face
    glColor4f(r * 0.4f, g * 0.4f, b * 0.4f, alpha);
    draw_persegi(x1, y1, z1, x2, y1, z1, x2, y1, z2, x1, y1, z2, alpha);

    // Front face
    glColor4f(r * 0.8f, g * 0.8f, b * 0.8f, alpha);
    draw_persegi(x1, y1, z2, x2, y1, z2, x2, y2, z2, x1, y2, z2, alpha);

    // Back face
    glColor4f(r * 0.6f, g * 0.6f, b * 0.6f, alpha);
    draw_persegi(x1, y1, z1, x1, y2, z1, x2, y2, z1, x2, y1, z1, alpha);

    // Left face
    glColor4f(r * 0.7f, g * 0.7f, b * 0.7f, alpha);
    draw_persegi(x1, y1, z1, x1, y1, z2, x1, y2, z2, x1, y2, z1, alpha);

    // Right face
    glColor4f(r * 0.5f, g * 0.5f, b * 0.5f, alpha);
    draw_persegi(x2, y1, z1, x2, y2, z1, x2, y2, z2, x2, y1, z2, alpha);
}

void drawNIM(float baseX, float baseY, float baseZ, float scale, bool is3D) {
    float coords[][4] = {
        {-1.8, 1.6, -1.0, 1.8},
        {-1.8, -1.8, -1.0, -1.6},
        {-1.8, -1.6, -1.6, 1.6},
        {-1.2, -1.6, -1.0, 1.6},
        {-0.8, 1.6, 0.6, 1.8},
        {0.4, -1.8, 0.6, 1.6},
        {0.8, 1.6, 1.8, 1.8},
        {0.8, -1.8, 1.0, 1.6},
        {1.0, -1.8, 1.8, -1.6},
        {1.6, -1.6, 1.8, 0.0},
        {1.0, -0.2, 1.6, 0.0}
    };

    if (is3D) {
        for (int i = 0; i < 11; i++) {
            // Passing color directly (Black: 0, 0, 0)
            draw_3d_kotak(baseX + coords[i][0] * scale, baseY + coords[i][1] * scale, baseZ - 0.2f * scale,
                          baseX + coords[i][2] * scale, baseY + coords[i][3] * scale, baseZ + 0.2f * scale,
                          0.0f, 0.0f, 0.0f, 1.0f);
        }
    } else {
        glColor3f(0.0, 0.0, 0.0);
        for (int i = 0; i < 11; i++) {
            glRectf(baseX + coords[i][0] * scale, baseY + coords[i][1] * scale,
                    baseX + coords[i][2] * scale, baseY + coords[i][3] * scale);
        }
    }
}
