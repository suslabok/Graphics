#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct Point
{
    int x, y;
};
Point p1 = {100, 100}, p2 = {700, 500};

void bresenhamLine(int x1, int y1, int x2, int y2)
{
    bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
    if (steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = std::abs(y2 - y1);
    int error = dx / 2;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = y1;

    glBegin(GL_POINTS);
    for (int x = x1; x <= x2; x++)
    {
        if (steep)
            glVertex2i(y, x);
        else
            glVertex2i(x, y);

        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(2.0f);
    glColor3f(1, 1, 1);
    bresenhamLine(p1.x, p1.y, p2.x, p2.y);

    glPointSize(6.0f);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    glVertex2i(p1.x, p1.y);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_POINTS);
    glVertex2i(p2.x, p2.y);
    glEnd();

    glFlush();
}

void init()
{
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int)
{
    if (key == 27)
        exit(0);
}

void setPredefinedLine(int choice)
{
    switch (choice)
    {
    case 1: // |m| < 1 (gentle slope)
        p1 = {150, 300};
        p2 = {650, 400};
        break;
    case 2: // |m| >= 1 (steep slope)
        p1 = {300, 150};
        p2 = {400, 550};
        break;
    default:
        std::cout << "Invalid choice, using default line.\n";
        break;
    }
}
int main(int argc, char **argv)
{
    std::cout << "Choose a predefined line based on slope magnitude |m|:\n";
    std::cout << "1. |m| < 1\n2. |m| >= 1\nEnter choice: ";
    int choice;
    std::cin >> choice;
    setPredefinedLine(choice);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Bresenham Line Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
