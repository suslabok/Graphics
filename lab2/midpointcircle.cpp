#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

int centerX = 300;
int centerY = 300;
int radius = 150;

void plotCirclePoints(int xc, int yc, int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
    glEnd();
}

void drawCircleMidpoint(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    std::cout << "\nMidpoint Circle Algorithm: " << std::endl;
    std::cout << "Center: (" << xc << ", " << yc << ")" << std::endl;
    std::cout << "Radius: " << r << std::endl;
    std::cout << "Initial decision parameter d = " << d << std::endl;

    plotCirclePoints(xc, yc, x, y);

    while (x < y)
    {
        x++;
        if (d < 0)
            d += 2 * x + 1;
        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }
        plotCirclePoints(xc, yc, x, y);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(4.0);
    glColor3f(0.0f, 0.75f, 1.0f);

    drawCircleMidpoint(centerX, centerY, radius);

    glPointSize(9.0);
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_POINTS);
    glVertex2i(centerX, centerY);
    glEnd();

    glColor3f(0.9f, 0.1f, 0.1f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2i(centerX, centerY);
    glVertex2i(centerX + radius, centerY);
    glEnd();

    glFlush();
}

void init()
{
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

int main(int argc, char **argv)
{
    std::cout << "Midpoint Circle Drawing Algorithm - OpenGL" << std::endl;
    std::cout << "Press ESC in the window to exit." << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Midpoint Circle Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
