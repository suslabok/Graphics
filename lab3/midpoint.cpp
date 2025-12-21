#include <GL/glut.h>
#include <iostream>

int xc = 300, yc = 300;
int rx = 200, ry = 120;

void plotEllipsePoints(int xc, int yc, int x, int y)
{
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
}

void midpointEllipse(int xc, int yc, int rx, int ry)
{
    float dx, dy, d1, d2;
    int x = 0;
    int y = ry;

    glBegin(GL_POINTS);

    /* Region 1 */
    d1 = (ry * ry) - (rx * rx * ry) + (0.25f * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    while (dx < dy)
    {
        plotEllipsePoints(xc, yc, x, y);

        if (d1 < 0)
        {
            x++;
            dx += 2 * ry * ry;
            d1 += dx + (ry * ry);
        }
        else
        {
            x++;
            y--;
            dx += 2 * ry * ry;
            dy -= 2 * rx * rx;
            d1 += dx - dy + (ry * ry);
        }
    }

    /* Region 2 */
    d2 = (ry * ry * (x + 0.5f) * (x + 0.5f)) +
         (rx * rx * (y - 1) * (y - 1)) -
         (rx * rx * ry * ry);

    while (y >= 0)
    {
        plotEllipsePoints(xc, yc, x, y);

        if (d2 > 0)
        {
            y--;
            dy -= 2 * rx * rx;
            d2 += (rx * rx) - dy;
        }
        else
        {
            y--;
            x++;
            dx += 2 * ry * ry;
            dy -= 2 * rx * rx;
            d2 += dx - dy + (rx * rx);
        }
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 1.0f);
    glPointSize(2);
    midpointEllipse(xc, yc, rx, ry);
    glFlush();
}

void init()
{
    glClearColor(0, 0, 0, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 600);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Midpoint Ellipse Algorithm");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
