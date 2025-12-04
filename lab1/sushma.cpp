#include <GL/glut.h>
#include <vector>
using namespace std;

void poly(const vector<pair<float, float>> &p, float x, float y, float s)
{
    glBegin(GL_POLYGON);
    for (auto &v : p)
        glVertex2f(x + v.first * s, y + v.second * s);
    glEnd();
}

/* ---------- LETTER DEFINITIONS (POLYGONS ONLY) ---------- */
void drawA(float x, float y, float s)
{
    // Top horizontal bar
    poly({{0.1, 0.85}, {0.9, 0.85}, {0.9, 1}, {0.1, 1}}, x, y, s);
    // Left vertical stroke
    poly({{0.1, 0}, {0.3, 0}, {0.3, 1}, {0.1, 1}}, x, y, s);
    // Right vertical stroke
    poly({{0.7, 0}, {0.9, 0}, {0.9, 1}, {0.7, 1}}, x, y, s);
    // Middle crossbar
    poly({{0.3, 0.45}, {0.7, 0.45}, {0.7, 0.6}, {0.3, 0.6}}, x, y, s);
}

void drawY(float x, float y, float s)
{
    // Left upper vertical
    poly({{0.1, 0.5}, {0.3, 0.5}, {0.3, 1}, {0.1, 1}}, x, y, s);
    // Right upper vertical
    poly({{0.7, 0.5}, {0.9, 0.5}, {0.9, 1}, {0.7, 1}}, x, y, s);
    // Middle horizontal connector
    poly({{0.3, 0.5}, {0.7, 0.5}, {0.7, 0.65}, {0.3, 0.65}}, x, y, s);
    // Bottom stem
    poly({{0.4, 0}, {0.6, 0}, {0.6, 0.5}, {0.4, 0.5}}, x, y, s);
}

void drawU(float x, float y, float s)
{
    // Left vertical bar
    poly({{0, 0}, {0.2, 0}, {0.2, 1}, {0, 1}}, x, y, s);
    // Right vertical bar
    poly({{0.8, 0}, {1, 0}, {1, 1}, {0.8, 1}}, x, y, s);
    // Bottom connector
    poly({{0.2, 0}, {0.8, 0}, {0.8, 0.15}, {0.2, 0.15}}, x, y, s);
}

void drawS(float x, float y, float s)
{
    // Top horizontal
    poly({{0.1, 0.85}, {0.9, 0.85}, {0.9, 1}, {0.1, 1}}, x, y, s);
    // Top-left vertical
    poly({{0.1, 0.5}, {0.25, 0.5}, {0.25, 1}, {0.1, 1}}, x, y, s);
    // Middle horizontal
    poly({{0.1, 0.5}, {0.9, 0.5}, {0.9, 0.65}, {0.1, 0.65}}, x, y, s);
    // Bottom-right vertical
    poly({{0.75, 0}, {0.9, 0}, {0.9, 0.5}, {0.75, 0.5}}, x, y, s);
    // Bottom horizontal
    poly({{0.1, 0}, {0.9, 0}, {0.9, 0.15}, {0.1, 0.15}}, x, y, s);
}

void drawH(float x, float y, float s)
{
    // Left vertical bar
    poly({{0, 0}, {0.2, 0}, {0.2, 1}, {0, 1}}, x, y, s);
    // Right vertical bar
    poly({{0.8, 0}, {1, 0}, {1, 1}, {0.8, 1}}, x, y, s);
    // Horizontal crossbar
    poly({{0.2, 0.45}, {0.8, 0.45}, {0.8, 0.6}, {0.2, 0.6}}, x, y, s);
}

void drawM(float x, float y, float s)
{
    // Left vertical bar
    poly({{0, 0}, {0.2, 0}, {0.2, 1}, {0, 1}}, x, y, s);
    // Right vertical bar
    poly({{0.8, 0}, {1, 0}, {1, 1}, {0.8, 1}}, x, y, s);
    // Left diagonal
    poly({{0.2, 1}, {0.4, 1}, {0.5, 0.6}, {0.3, 0.6}}, x, y, s);
    // Right diagonal
    poly({{0.6, 1}, {0.8, 1}, {0.7, 0.6}, {0.5, 0.6}}, x, y, s);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.2, 0.1, 0.8);

    float s = 0.4f;    // Letter size
    float gap = 0.15f; // Gap between letters
    int letters = 6;

    float totalWidth = letters * s + (letters - 1) * gap;
    float x = -totalWidth / 2.0f; // Start x to center text
    float y = -0.1f;              // Vertical center

    drawS(x, y, s);
    x += gap + s;
    drawU(x, y, s);
    x += gap + s;
    drawS(x, y, s);
    x += gap + s;
    drawH(x, y, s);
    x += gap + s;
    drawM(x, y, s);
    x += gap + s;
    drawA(x, y, s);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)w / (float)h;
    if (aspect >= 1.0)
        glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1000, 400);
    glutCreateWindow("SUSHMA - Polygon Letters");

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
