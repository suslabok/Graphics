#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

struct Point2D
{
    float x, y;
    Point2D(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

struct Matrix3x3
{
    float m[3][3];
};

Point2D applyMatrix(Matrix3x3 mat, Point2D p)
{
    Point2D r;
    r.x = mat.m[0][0] * p.x + mat.m[0][1] * p.y + mat.m[0][2];
    r.y = mat.m[1][0] * p.x + mat.m[1][1] * p.y + mat.m[1][2];
    return r;
}

Matrix3x3 multiplyMatrix(Matrix3x3 a, Matrix3x3 b)
{
    Matrix3x3 r{};
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                r.m[i][j] += a.m[i][k] * b.m[k][j];
    return r;
}

// Translation
Matrix3x3 getTranslationMatrix(float tx, float ty)
{
    return {{{1, 0, tx},
             {0, 1, ty},
             {0, 0, 1}}};
}

// Rotation about point
Matrix3x3 getRotationMatrix(float angle, float px, float py)
{
    float rad = angle * M_PI / 180.0f;

    Matrix3x3 t1 = {{{1, 0, -px},
                     {0, 1, -py},
                     {0, 0, 1}}};

    Matrix3x3 r = {{{cos(rad), -sin(rad), 0},
                    {sin(rad), cos(rad), 0},
                    {0, 0, 1}}};

    Matrix3x3 t2 = {{{1, 0, px},
                     {0, 1, py},
                     {0, 0, 1}}};

    return multiplyMatrix(t2, multiplyMatrix(r, t1));
}

// Scaling about point
Matrix3x3 getScalingMatrix(float sx, float sy, float px, float py)
{
    Matrix3x3 t1 = {{{1, 0, -px},
                     {0, 1, -py},
                     {0, 0, 1}}};

    Matrix3x3 s = {{{sx, 0, 0},
                    {0, sy, 0},
                    {0, 0, 1}}};

    Matrix3x3 t2 = {{{1, 0, px},
                     {0, 1, py},
                     {0, 0, 1}}};

    return multiplyMatrix(t2, multiplyMatrix(s, t1));
}

// Reflection about horizontal line y = c
Matrix3x3 getReflectionXMatrix(float c)
{
    return {{{1, 0, 0},
             {0, -1, 2 * c},
             {0, 0, 1}}};
}

// Reflection about vertical line x = c
Matrix3x3 getReflectionYMatrix(float c)
{
    return {{{-1, 0, 2 * c},
             {0, 1, 0},
             {0, 0, 1}}};
}

// Shearing X
Matrix3x3 getShearingXMatrix(float shx)
{
    return {{{1, shx, 0},
             {0, 1, 0},
             {0, 0, 1}}};
}

// Shearing Y
Matrix3x3 getShearingYMatrix(float shy)
{
    return {{{1, 0, 0},
             {shy, 1, 0},
             {0, 0, 1}}};
}

int WIDTH = 600, HEIGHT = 600;

std::vector<Point2D> originalShape;
std::vector<Point2D> transformedShape;
std::string currentTransformationText = "Original Shape";

void initShape()
{
    originalShape.clear();

    // Changed triangle points
    originalShape.push_back(Point2D(200, 200));
    originalShape.push_back(Point2D(350, 200));
    originalShape.push_back(Point2D(275, 350));

    transformedShape = originalShape;
}

void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawAxes()
{
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_LINES);
    glVertex2i(0, HEIGHT / 2);
    glVertex2i(WIDTH, HEIGHT / 2);
    glVertex2i(WIDTH / 2, 0);
    glVertex2i(WIDTH / 2, HEIGHT);
    glEnd();
}

void drawShape(const std::vector<Point2D> &shape, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (auto &p : shape)
        glVertex2f(p.x, p.y);
    glEnd();
}

void drawText(int x, int y, const std::string &text)
{
    glColor3f(0, 0, 0);
    glRasterPos2i(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    drawShape(originalShape, 0, 0, 1);
    drawShape(transformedShape, 0, 1, 0);

    drawText(10, HEIGHT - 20, currentTransformationText);
    glFlush();
}

void reshape(int w, int h)
{
    WIDTH = w;
    HEIGHT = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void applyTransformation(int choice)
{
    Matrix3x3 m;

    switch (choice)
    {
    case 1:
        m = getTranslationMatrix(-70, 80);
        currentTransformationText = "Translation (-70, 80)";
        break;
    case 2:
        m = getRotationMatrix(120, WIDTH / 2.0f, HEIGHT / 2.0f);
        currentTransformationText = "Rotation 120 degrees";
        break;
    case 3:
        m = getScalingMatrix(0.6f, 1.2f, WIDTH / 2.0f, HEIGHT / 2.0f);
        currentTransformationText = "Scaling (0.6x, 1.2x)";
        break;
    case 4:
        m = getReflectionXMatrix(HEIGHT / 2.0f);
        currentTransformationText = "Reflection about X-axis";
        break;
    case 5:
        m = getReflectionYMatrix(WIDTH / 2.0f);
        currentTransformationText = "Reflection about Y-axis";
        break;
    case 6:
        m = getShearingXMatrix(0.5f);
        currentTransformationText = "Shearing X";
        break;
    case 7:
        m = getShearingYMatrix(0.5f);
        currentTransformationText = "Shearing Y";
        break;
    case 8:
    {
        Matrix3x3 t = getTranslationMatrix(-40, 50);
        Matrix3x3 r = getRotationMatrix(-60, WIDTH / 2.0f, HEIGHT / 2.0f);
        Matrix3x3 s = getScalingMatrix(0.8f, 0.7f, WIDTH / 2.0f, HEIGHT / 2.0f);
        Matrix3x3 sh = getShearingYMatrix(-0.4f);

        m = multiplyMatrix(t, multiplyMatrix(r, multiplyMatrix(s, sh)));
        currentTransformationText = "Composite: Translate -> Rotate -> Scale -> ShearY";
        break;
    }

    default:
        return;
    }

    transformedShape.clear();
    for (auto &p : originalShape)
        transformedShape.push_back(applyMatrix(m, p));

    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int)
{
    if (key == '0')
    {
        transformedShape = originalShape;
        currentTransformationText = "Original Shape";
    }
    else if (key >= '1' && key <= '8')
        applyTransformation(key - '0');
    else if (key == 'q' || key == 'Q')
        exit(0);

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    std::cout << "\n===== 2D TRANSFORMATIONS =====\n";
    std::cout << "1-Translation\n2-Rotation\n3-Scaling\n";
    std::cout << "4-Reflection X\n5-Reflection Y\n";
    std::cout << "6-Shearing X\n7-Shearing Y\n";
    std::cout << "8-Composite\n0-Reset\nQ-Quit\n";

    initShape();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("2D Transformations - Single File");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
