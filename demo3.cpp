#include "demo3.h"

bool view3D = false;
float rotationX = 30.0f; // Tilted from above
float rotationY = 0.0f;  // Facing straight
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

void draw_3d_kotak(float x1, float y1, float z1, float x2, float y2, float z2, float alpha) {
    float r, g, b;
    float color[4];
    glGetFloatv(GL_CURRENT_COLOR, color);
    r = color[0];
    g = color[1];
    b = color[2];

    // Top face - Brightest
    glColor4f(r * 1.0f, g * 1.0f, b * 1.0f, alpha);
    draw_persegi(x1, y2, z1, x1, y2, z2, x2, y2, z2, x2, y2, z1, alpha);
    
    // Bottom face - Darkest
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
    
    // Reset color to original for next calls
    glColor4f(r, g, b, alpha);
}

void drawNIM(float baseX, float baseY, float baseZ, float scale, bool is3D) {
    auto draw_obj = [&](float x1, float y1, float x2, float y2) {
        if (is3D) {
            // In 3D, baseY is Y (up), baseZ is Z (depth)
            // We want it to be vertical on X-Y plane
            draw_3d_kotak(baseX + x1 * scale, baseY + y1 * scale, baseZ - 0.2f * scale, 
                          baseX + x2 * scale, baseY + y2 * scale, baseZ + 0.2f * scale);
        } else {
            glRectf(baseX + x1 * scale, baseY + y1 * scale, baseX + x2 * scale, baseY + y2 * scale);
        }
    };

    draw_obj(-1.8, 1.6, -1.0, 1.8);
    draw_obj(-1.8, -1.8, -1.0, -1.6);
    draw_obj(-1.8, -1.6, -1.6, 1.6);
    draw_obj(-1.2, -1.6, -1.0, 1.6);

    draw_obj(-0.8, 1.6, 0.6, 1.8);
    draw_obj(0.4, -1.8, 0.6, 1.6);

    draw_obj(0.8, 1.6, 1.8, 1.8);
    draw_obj(0.8, -1.8, 1.0, 1.6);
    draw_obj(1.0, -1.8, 1.8, -1.6);
    draw_obj(1.6, -1.6, 1.8, 0.0);
    draw_obj(1.0, -0.2, 1.6, 0.0);
}
