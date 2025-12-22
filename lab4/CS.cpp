#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>

const int WIDTH = 600, HEIGHT = 600;

const int xmin = 100, ymin = 100;
const int xmax = 500, ymax = 400;

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

struct Point
{
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

struct Line
{
    Point orig_start, orig_end;
    Point clipped_start, clipped_end;
    bool clipped = false;
    bool rejected = false;

    Line(float x0, float y0, float x1, float y1)
        : orig_start(x0, y0), orig_end(x1, y1),
          clipped_start(x0, y0), clipped_end(x1, y1) {}
};

std::vector<Line> lines;

void init()
{
    // White background
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    // Thicker lines
    glLineWidth(3.0f);
}

int computeOutCode(float x, float y)
{
    int code = INSIDE;
    if (x < xmin)
        code |= LEFT;
    else if (x > xmax)
        code |= RIGHT;
    if (y < ymin)
        code |= BOTTOM;
    else if (y > ymax)
        code |= TOP;
    return code;
}

bool cohenSutherlandClip(float &x0, float &y0, float &x1, float &y1)
{
    int outcode0 = computeOutCode(x0, y0);
    int outcode1 = computeOutCode(x1, y1);

    bool accept = false;

    while (true)
    {
        if (!(outcode0 | outcode1))
        {
            accept = true;
            break;
        }
        else if (outcode0 & outcode1)
        {
            break;
        }
        else
        {
            float x, y;
            int outcodeOut = outcode0 ? outcode0 : outcode1;

            if (outcodeOut & TOP)
            {
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            }
            else if (outcodeOut & BOTTOM)
            {
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            }
            else if (outcodeOut & RIGHT)
            {
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            }
            else if (outcodeOut & LEFT)
            {
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if (outcodeOut == outcode0)
            {
                x0 = x;
                y0 = y;
                outcode0 = computeOutCode(x0, y0);
            }
            else
            {
                x1 = x;
                y1 = y;
                outcode1 = computeOutCode(x1, y1);
            }
        }
    }
    return accept;
}

void drawLineSegment(Point p1, Point p2, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
}

void drawClippingWindow()
{

    glColor3f(1, 0, 0);
    glLineWidth(4.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();
    glLineWidth(3.0f);
}

bool isInside(Point p)
{
    return (p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawClippingWindow();

    for (auto &line : lines)
    {
        if (!line.clipped && !line.rejected)
        {
            // Original full line in blue
            drawLineSegment(line.orig_start, line.orig_end, 0, 0, 1);
        }
        else if (line.rejected)
        {
            // Fully outside line in gray
            drawLineSegment(line.orig_start, line.orig_end, 0.5f, 0.5f, 0.5f);
        }
        else if (line.clipped)
        {
            // Inside clipped part green
            drawLineSegment(line.clipped_start, line.clipped_end, 0, 1, 0);

            // Outside parts gray
            if (!isInside(line.orig_start))
                drawLineSegment(line.orig_start, line.clipped_start, 0.5f, 0.5f, 0.5f);

            if (!isInside(line.orig_end))
                drawLineSegment(line.clipped_end, line.orig_end, 0.5f, 0.5f, 0.5f);
        }
    }

    glFlush();
}

void keyboard(unsigned char key, int, int)
{
    if (key == 'c' || key == 'C')
    {
        for (auto &line : lines)
        {
            if (!line.clipped && !line.rejected)
            {
                float x0 = line.orig_start.x;
                float y0 = line.orig_start.y;
                float x1 = line.orig_end.x;
                float y1 = line.orig_end.y;

                if (cohenSutherlandClip(x0, y0, x1, y1))
                {
                    line.clipped_start = Point(x0, y0);
                    line.clipped_end = Point(x1, y1);
                    line.clipped = true;
                    line.rejected = false;

                    std::cout << "Line clipped to: (" << x0 << ", " << y0 << ") - (" << x1 << ", " << y1 << ")\n";
                }
                else
                {
                    line.clipped = false;
                    line.rejected = true;

                    std::cout << "Line rejected (completely outside).\n";
                }
            }
        }
        glutPostRedisplay();
    }
    else if (key == 'r' || key == 'R')
    {
        for (auto &line : lines)
        {
            line.clipped = false;
            line.rejected = false;
            line.clipped_start = line.orig_start;
            line.clipped_end = line.orig_end;
        }
        std::cout << "Reset all lines to original coordinates.\n";
        glutPostRedisplay();
    }
    else if (key == 'q' || key == 'Q')
    {
        exit(0);
    }
}

int main(int argc, char **argv)
{
    std::cout << "Cohen-Sutherland Line Clipping Algorithm\n";
    std::cout << "Press 'c' to clip all lines\n";
    std::cout << "Press 'r' to reset\n";
    std::cout << "Press 'q' to quit\n";

    lines.emplace_back(50, 350, 550, 150);
    lines.emplace_back(120, 120, 480, 380);
    lines.emplace_back(200, 450, 400, 450);
    lines.emplace_back(600, 300, 450, 150);
    lines.emplace_back(300, 300, 300, 50);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
