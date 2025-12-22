#include <GL/glut.h>
#include <iostream>
#include <vector>

const int WIDTH = 800, HEIGHT = 600;
int xmin = 150, ymin = 150, xmax = 650, ymax = 450;

struct Line
{
    float x1, y1, x2, y2;
    bool clipped;
    float cx1, cy1, cx2, cy2;

    Line(float _x1, float _y1, float _x2, float _y2)
        : x1(_x1), y1(_y1), x2(_x2), y2(_y2), clipped(false) {}
};

std::vector<Line> lines;

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

bool liangBarskyClip(Line &line)
{
    float dx = line.x2 - line.x1;
    float dy = line.y2 - line.y1;
    float p[4] = {-dx, dx, -dy, dy};
    float q[4] = {line.x1 - xmin, xmax - line.x1, line.y1 - ymin, ymax - line.y1};
    float u1 = 0.0f, u2 = 1.0f;

    for (int i = 0; i < 4; i++)
    {
        if (p[i] == 0)
        {
            if (q[i] < 0)
            {
                line.clipped = true;
                return false;
            }
        }
        else
        {
            float u = q[i] / p[i];
            if (p[i] < 0)
            {
                if (u > u2)
                {
                    line.clipped = true;
                    return false;
                }
                if (u > u1)
                    u1 = u;
            }
            else
            {
                if (u < u1)
                {
                    line.clipped = true;
                    return false;
                }
                if (u < u2)
                    u2 = u;
            }
        }
    }

    if (u1 > u2)
    {
        line.clipped = true;
        return false;
    }

    line.cx1 = line.x1 + u1 * dx;
    line.cy1 = line.y1 + u1 * dy;
    line.cx2 = line.x1 + u2 * dx;
    line.cy2 = line.y1 + u2 * dy;
    line.clipped = true;
    return true;
}

void drawClippingWindow()
{
    glLineWidth(3.0f);
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawClippingWindow();

    for (auto &line : lines)
    {
        if (!line.clipped)
        {
            glLineWidth(3.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_LINES);
            glVertex2f(line.x1, line.y1);
            glVertex2f(line.x2, line.y2);
            glEnd();
        }
        else
        {
            glLineWidth(3.0f);
            glColor3f(0.5f, 0.5f, 0.5f);
            glBegin(GL_LINES);
            glVertex2f(line.x1, line.y1);
            glVertex2f(line.x2, line.y2);
            glEnd();

            if (line.cx1 != 0 || line.cy1 != 0 || line.cx2 != 0 || line.cy2 != 0)
            {
                glLineWidth(4.0f);
                glColor3f(0.0f, 1.0f, 0.0f);
                glBegin(GL_LINES);
                glVertex2f(line.cx1, line.cy1);
                glVertex2f(line.cx2, line.cy2);
                glEnd();
            }
        }
    }
    glFlush();
}

void performClipping()
{
    std::cout << "\n=== Clipping Results ===" << std::endl;
    for (auto &line : lines)
        if (liangBarskyClip(line))
            std::cout << "Line clipped" << std::endl;
        else
            std::cout << "Line rejected" << std::endl;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int)
{
    if (key == 27)
        exit(0);
    if (key == 'c' || key == 'C')
        performClipping();
    if (key == 'r' || key == 'R')
    {
        lines.clear();
        lines.emplace_back(50, 350, 750, 150);
        lines.emplace_back(200, 200, 600, 400);
        lines.emplace_back(100, 500, 700, 550);
        lines.emplace_back(750, 300, 600, 150);
        lines.emplace_back(300, 300, 300, 50);
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    lines.emplace_back(50, 350, 750, 150);
    lines.emplace_back(200, 200, 600, 400);
    lines.emplace_back(100, 500, 700, 550);
    lines.emplace_back(750, 300, 600, 150);
    lines.emplace_back(300, 300, 300, 50);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Liang-Barsky Clipping ");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
