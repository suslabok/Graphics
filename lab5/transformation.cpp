#include <GL/glut.h>
#include <iostream>
#include <cmath>

// Global variables for transformations
float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;
float rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
float shearX = 0.0f, shearY = 0.0f;

enum TransformMode
{
    TRANSLATION,
    ROTATION,
    SCALING,
    SHEARING
};
TransformMode currentMode = TRANSLATION;

// Function prototypes
void display();
void handleKeyboard(unsigned char key, int x, int y);
void handleSpecialKeys(int key, int x, int y);
void reshape(int w, int h);
void init();
void reset();
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    std::cout << "\n3D TRANSFORMATION DEMO" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "1 - Translation Mode" << std::endl;
    std::cout << "2 - Rotation Mode" << std::endl;
    std::cout << "3 - Scaling Mode" << std::endl;
    std::cout << "4 - Shearing Mode" << std::endl;
    std::cout << "Arrow Keys - Apply transformation" << std::endl;
    std::cout << "+/- - Scale up/down (in scaling mode)" << std::endl;
    std::cout << "R - Reset current transformation" << std::endl;
    std::cout << "A - Reset all transformations" << std::endl;
    std::cout << "ESC - Exit" << std::endl;
}

void reset()
{
    switch (currentMode)
    {
    case TRANSLATION:
        translateX = translateY = translateZ = 0.0f;
        break;
    case ROTATION:
        rotateX = rotateY = rotateZ = 0.0f;
        break;
    case SCALING:
        scaleX = scaleY = scaleZ = 1.0f;
        break;
    case SHEARING:
        shearX = shearY = 0.0f;
        break;
    }
    glutPostRedisplay();
}

void resetAll()
{
    translateX = translateY = translateZ = 0.0f;
    rotateX = rotateY = rotateZ = 0.0f;
    scaleX = scaleY = scaleZ = 1.0f;
    shearX = shearY = 0.0f;
    glutPostRedisplay();
}

void drawCube()
{
    glBegin(GL_QUADS);

    // Front face (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Back face (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Top face (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Bottom face (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Left face (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera position
    glTranslatef(0.0f, 0.0f, -8.0f);

    // Apply transformations
    glTranslatef(translateX, translateY, translateZ);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX, scaleY, scaleZ);

    // Apply shearing transformation
    if (shearX != 0.0f || shearY != 0.0f)
    {
        GLfloat shearMatrix[16] = {
            1.0f, shearY, 0.0f, 0.0f,
            shearX, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
        glMultMatrixf(shearMatrix);
    }

    drawCube();
    glutSwapBuffers();
}

void handleKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        currentMode = TRANSLATION;
        std::cout << "Mode: TRANSLATION" << std::endl;
        break;
    case '2':
        currentMode = ROTATION;
        std::cout << "Mode: ROTATION" << std::endl;
        break;
    case '3':
        currentMode = SCALING;
        std::cout << "Mode: SCALING" << std::endl;
        break;
    case '4':
        currentMode = SHEARING;
        std::cout << "Mode: SHEARING" << std::endl;
        break;
    case 'r':
    case 'R':
        reset();
        std::cout << "Reset current transformation" << std::endl;
        break;
    case 'a':
    case 'A':
        resetAll();
        std::cout << "Reset all transformations" << std::endl;
        break;
    case '+':
    case '=':
        if (currentMode == SCALING)
        {
            scaleX += 0.1f;
            scaleY += 0.1f;
            scaleZ += 0.1f;
            std::cout << "Scale: " << scaleX << std::endl;
        }
        break;
    case '-':
    case '_':
        if (currentMode == SCALING)
        {
            scaleX -= 0.1f;
            scaleY -= 0.1f;
            scaleZ -= 0.1f;
            if (scaleX < 0.1f)
                scaleX = 0.1f;
            if (scaleY < 0.1f)
                scaleY = 0.1f;
            if (scaleZ < 0.1f)
                scaleZ = 0.1f;
            std::cout << "Scale: " << scaleX << std::endl;
        }
        break;
    case 27: // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void handleSpecialKeys(int key, int x, int y)
{
    const float step = 0.5f;
    const float rotStep = 10.0f;
    const float scaleStep = 0.1f;
    const float shearStep = 0.1f;

    switch (currentMode)
    {
    case TRANSLATION:
        switch (key)
        {
        case GLUT_KEY_LEFT:
            translateX -= step;
            std::cout << "Translate X: " << translateX << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            translateX += step;
            std::cout << "Translate X: " << translateX << std::endl;
            break;
        case GLUT_KEY_UP:
            translateY += step;
            std::cout << "Translate Y: " << translateY << std::endl;
            break;
        case GLUT_KEY_DOWN:
            translateY -= step;
            std::cout << "Translate Y: " << translateY << std::endl;
            break;
        case GLUT_KEY_PAGE_UP:
            translateZ += step;
            std::cout << "Translate Z: " << translateZ << std::endl;
            break;
        case GLUT_KEY_PAGE_DOWN:
            translateZ -= step;
            std::cout << "Translate Z: " << translateZ << std::endl;
            break;
        }
        break;

    case ROTATION:
        switch (key)
        {
        case GLUT_KEY_LEFT:
            rotateY -= rotStep;
            std::cout << "Rotate Y: " << rotateY << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            rotateY += rotStep;
            std::cout << "Rotate Y: " << rotateY << std::endl;
            break;
        case GLUT_KEY_UP:
            rotateX -= rotStep;
            std::cout << "Rotate X: " << rotateX << std::endl;
            break;
        case GLUT_KEY_DOWN:
            rotateX += rotStep;
            std::cout << "Rotate X: " << rotateX << std::endl;
            break;
        case GLUT_KEY_PAGE_UP:
            rotateZ += rotStep;
            std::cout << "Rotate Z: " << rotateZ << std::endl;
            break;
        case GLUT_KEY_PAGE_DOWN:
            rotateZ -= rotStep;
            std::cout << "Rotate Z: " << rotateZ << std::endl;
            break;
        }
        break;

    case SCALING:
        switch (key)
        {
        case GLUT_KEY_LEFT:
            scaleX -= scaleStep;
            if (scaleX < 0.1f)
                scaleX = 0.1f;
            std::cout << "Scale X: " << scaleX << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            scaleX += scaleStep;
            std::cout << "Scale X: " << scaleX << std::endl;
            break;
        case GLUT_KEY_UP:
            scaleY += scaleStep;
            std::cout << "Scale Y: " << scaleY << std::endl;
            break;
        case GLUT_KEY_DOWN:
            scaleY -= scaleStep;
            if (scaleY < 0.1f)
                scaleY = 0.1f;
            std::cout << "Scale Y: " << scaleY << std::endl;
            break;
        case GLUT_KEY_PAGE_UP:
            scaleZ += scaleStep;
            std::cout << "Scale Z: " << scaleZ << std::endl;
            break;
        case GLUT_KEY_PAGE_DOWN:
            scaleZ -= scaleStep;
            if (scaleZ < 0.1f)
                scaleZ = 0.1f;
            std::cout << "Scale Z: " << scaleZ << std::endl;
            break;
        }
        break;

    case SHEARING:
        switch (key)
        {
        case GLUT_KEY_LEFT:
            shearX -= shearStep;
            std::cout << "Shear X: " << shearX << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            shearX += shearStep;
            std::cout << "Shear X: " << shearX << std::endl;
            break;
        case GLUT_KEY_UP:
            shearY += shearStep;
            std::cout << "Shear Y: " << shearY << std::endl;
            break;
        case GLUT_KEY_DOWN:
            shearY -= shearStep;
            std::cout << "Shear Y: " << shearY << std::endl;
            break;
        }
        break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Transformations");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(handleSpecialKeys);

    glutMainLoop();
    return 0;
}