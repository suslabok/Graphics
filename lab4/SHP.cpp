#include <GL/glut.h>
#include <iostream>
#include <vector>

const int WIDTH = 800, HEIGHT = 600;
int xmin = 150, ymin = 150, xmax = 550, ymax = 450;

bool showOriginal = true;
int currentStep = 0;

struct Point
{
    float x, y;
    Point(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

std::vector<Point> original;
std::vector<Point> polygon;

bool insideLeft(Point p) { return p.x >= xmin; }
bool insideRight(Point p) { return p.x <= xmax; }
bool insideBottom(Point p) { return p.y >= ymin; }
bool insideTop(Point p) { return p.y <= ymax; }

Point intersect(Point p1, Point p2, int edge)
{
    Point p;
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    if (abs(dx) < 0.001f) // Vertical line
    {
        p.x = p1.x;
        p.y = (edge == 2) ? ymin : ymax;
        return p;
    }

    float m = dy / dx;
    switch (edge)
    {
    case 0:
        p.x = xmin;
        p.y = p1.y + (xmin - p1.x) * m;
        break; // Left
    case 1:
        p.x = xmax;
        p.y = p1.y + (xmax - p1.x) * m;
        break; // Right
    case 2:
        p.y = ymin;
        p.x = p1.x + (ymin - p1.y) / m;
        break; // Bottom
    case 3:
        p.y = ymax;
        p.x = p1.x + (ymax - p1.y) / m;
        break; // Top
    }
    return p;
}

void clipEdge(int edge)
{
    std::vector<Point> result;
    if (polygon.empty())
        return;

    for (int i = 0; i < polygon.size(); i++)
    {
        Point curr = polygon[i];
        Point prev = polygon[(i + polygon.size() - 1) % polygon.size()];

        bool currIn = (edge == 0) ? insideLeft(curr) : (edge == 1) ? insideRight(curr)
                                                   : (edge == 2)   ? insideBottom(curr)
                                                                   : insideTop(curr);
        bool prevIn = (edge == 0) ? insideLeft(prev) : (edge == 1) ? insideRight(prev)
                                                   : (edge == 2)   ? insideBottom(prev)
                                                                   : insideTop(prev);

        if (prevIn && currIn)
            result.push_back(curr);
        else if (prevIn && !currIn)
            result.push_back(intersect(prev, curr, edge));
        else if (!prevIn && currIn)
        {
            result.push_back(intersect(prev, curr, edge));
            result.push_back(curr);
        }
    }
    polygon = result;
}

void drawClippingWindow()
{
    glLineWidth(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();
}

void drawPolygon(const std::vector<Point> &poly, float r, float g, float b)
{
    if (poly.empty())
        return;

    glLineWidth(3.0f);
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (const auto &p : poly)
        glVertex2f(p.x, p.y);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (const auto &p : poly)
        glVertex2f(p.x, p.y);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawClippingWindow();

    if (showOriginal && currentStep == 0)
        drawPolygon(original, 0.7f, 0.7f, 0.7f);
    drawPolygon(polygon, 0.2f, 0.6f, 1.0f);
    glFlush();
}

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case ' ': // Space - next step
        if (currentStep < 4)
        {
            clipEdge(currentStep);
            currentStep++;
            showOriginal = false;
        }
        glutPostRedisplay();
        break;
    case 'r': // Reset
        polygon = original;
        currentStep = 0;
        showOriginal = true;
        glutPostRedisplay();
        break;
    }
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

int main(int argc, char **argv)
{
    // Initialize default polygon
    original = {Point(100, 150), Point(400, 200), Point(450, 400), Point(200, 350)};
    polygon = original;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
