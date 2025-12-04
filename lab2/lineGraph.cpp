#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>

const int WIDTH = 700;
const int HEIGHT = 500;
const int MARGIN = 60;

struct Point
{
    float x, y;
};

std::vector<Point> data;
float minX, maxX, minY, maxY;

float colors[][3] = {
    {1.0f, 0.2f, 0.2f},
    {0.2f, 1.0f, 0.2f},
    {0.2f, 0.2f, 1.0f},
    {1.0f, 1.0f, 0.2f},
    {1.0f, 0.2f, 1.0f},
    {0.2f, 1.0f, 1.0f},
    {1.0f, 0.5f, 0.2f},
    {0.5f, 0.2f, 1.0f},
};

void drawLineDDA(int x1, int y1, int x2, int y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    float xInc = dx / steps;
    float yInc = dy / steps;
    float x = x1, y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        glVertex2f(round(x), round(y));
        x += xInc;
        y += yInc;
    }
    glEnd();
}

int mapX(float x)
{
    return MARGIN + ((x - minX) / (maxX - minX)) * (WIDTH - 2 * MARGIN);
}

int mapY(float y)
{
    return MARGIN + ((y - minY) / (maxY - minY)) * (HEIGHT - 2 * MARGIN);
}

void drawAxes()
{
    glColor3f(0.6f, 0.6f, 0.6f);
    glLineWidth(2.0);

    glBegin(GL_LINES);
    glVertex2i(MARGIN, MARGIN);
    glVertex2i(WIDTH - MARGIN, MARGIN);
    glVertex2i(MARGIN, MARGIN);
    glVertex2i(MARGIN, HEIGHT - MARGIN);
    glEnd();
}

void drawGraph()
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glPointSize(2.0);

    for (size_t i = 0; i < data.size() - 1; i++)
    {
        int x1 = mapX(data[i].x);
        int y1 = mapY(data[i].y);
        int x2 = mapX(data[i + 1].x);
        int y2 = mapY(data[i + 1].y);
        drawLineDDA(x1, y1, x2, y2);
    }

    glPointSize(12.0);
    glBegin(GL_POINTS);
    for (size_t i = 0; i < data.size(); i++)
    {
        int colorIndex = i % (sizeof(colors) / sizeof(colors[0]));
        glColor3f(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
        glVertex2i(mapX(data[i].x), mapY(data[i].y));
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    drawGraph();
    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

void loadData()
{
    data.clear();
    data.push_back({0.5f, 80});
    data.push_back({1.5f, 40});
    data.push_back({2.8f, 60});
    data.push_back({3.5f, 30});
    data.push_back({4.2f, 90});
    data.push_back({5.0f, 50});
    data.push_back({6.1f, 100});
    data.push_back({7.4f, 70});
    data.push_back({8.0f, 85});
    data.push_back({9.3f, 45});

    minX = maxX = data[0].x;
    minY = maxY = data[0].y;

    for (size_t i = 1; i < data.size(); i++)
    {
        if (data[i].x < minX)
            minX = data[i].x;
        if (data[i].x > maxX)
            maxX = data[i].x;
        if (data[i].y < minY)
            minY = data[i].y;
        if (data[i].y > maxY)
            maxY = data[i].y;
    }

    float xPad = (maxX - minX) * 0.1f;
    float yPad = (maxY - minY) * 0.1f;
    minX -= xPad;
    maxX += xPad;
    minY -= yPad;
    maxY += yPad;
}

int main(int argc, char **argv)
{
    loadData();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Colored Points Line Graph");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
