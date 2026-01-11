#include <GL/glut.h>
#include <iostream>
#include <cmath>

// Global variables for orthographic projection
float left = -5.0f, right = 5.0f;
float bottom = -5.0f, top = 5.0f;
float nearPlane = 0.1f, farPlane = 100.0f;

// View parameters
float eyeX = 5.0f, eyeY = 5.0f, eyeZ = 5.0f;
float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

bool showInfo = false;

void setupOrthographic()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
}

void drawScene()
{
    // Draw coordinate axes
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-3.0, 0.0, 0.0);
    glVertex3f(3.0, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -3.0, 0.0);
    glVertex3f(0.0, 3.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -3.0);
    glVertex3f(0.0, 0.0, 3.0);
    glEnd();
    glPushMatrix();
    glTranslatef(0.0, 0.0, 2.0);
    glColor3f(1.0, 0.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();

    glColor3f(1.0, 1.0, 0.0);
    glutWireCube(1.5);

    glPushMatrix();
    glTranslatef(0.0, 0.0, -2.0);
    glColor3f(0.0, 0.0, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setupOrthographic();

    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

    drawScene();

    // Display near/far distances if info is enabled
    if (showInfo)
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 0, 600, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2i(10, 580);
        char info[100];
        sprintf(info, "ORTHOGRAPHIC PROJECTION");
        for (int i = 0; info[i] != '\0'; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, info[i]);
        }

        glRasterPos2i(10, 550);
        sprintf(info, "Near: %.1f, Far: %.1f", nearPlane, farPlane);
        for (int i = 0; info[i] != '\0'; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);
        }

        glRasterPos2i(10, 530);
        sprintf(info, "Bounds: [%.1f, %.1f] x [%.1f, %.1f]", left, right, bottom, top);
        for (int i = 0; info[i] != '\0'; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);
        }

        glRasterPos2i(10, 510);
        sprintf(info, "Eye: (%.1f, %.1f, %.1f)", eyeX, eyeY, eyeZ);
        for (int i = 0; info[i] != '\0'; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '+':
    case '=':
        left += 0.5f;
        right -= 0.5f;
        bottom += 0.5f;
        top -= 0.5f;
        if (right - left < 1.0f)
        {
            left = -0.5f;
            right = 0.5f;
        }
        if (top - bottom < 1.0f)
        {
            bottom = -0.5f;
            top = 0.5f;
        }
        std::cout << "Zoom in - Bounds: [" << left << ", " << right << "]" << std::endl;
        break;
    case '-':
    case '_':
        left -= 0.5f;
        right += 0.5f;
        bottom -= 0.5f;
        top += 0.5f;
        std::cout << "Zoom out - Bounds: [" << left << ", " << right << "]" << std::endl;
        break;
    case 'n':
    case 'N':
        nearPlane += 0.5f;
        if (nearPlane >= farPlane - 1.0f)
            nearPlane = farPlane - 1.0f;
        std::cout << "Near: " << nearPlane << std::endl;
        break;
    case 'f':
    case 'F':
        nearPlane -= 0.5f;
        if (nearPlane <= 0.1f)
            nearPlane = 0.1f;
        std::cout << "Near: " << nearPlane << std::endl;
        break;
    case 'g':
    case 'G':
        farPlane += 5.0f;
        std::cout << "Far: " << farPlane << std::endl;
        break;
    case 'h':
    case 'H':
        farPlane -= 5.0f;
        if (farPlane <= nearPlane + 1.0f)
            farPlane = nearPlane + 1.0f;
        std::cout << "Far: " << farPlane << std::endl;
        break;
    case 'i':
    case 'I':
        showInfo = !showInfo;
        std::cout << "Info display " << (showInfo ? "ON" : "OFF") << std::endl;
        break;
    case 'r':
    case 'R':
        left = -5.0f;
        right = 5.0f;
        bottom = -5.0f;
        top = 5.0f;
        nearPlane = 0.1f;
        farPlane = 100.0f;
        eyeX = eyeY = eyeZ = 5.0f;
        std::cout << "Reset all parameters" << std::endl;
        break;
    case 27: // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        eyeX -= 0.5f;
        std::cout << "Eye X: " << eyeX << std::endl;
        break;
    case GLUT_KEY_RIGHT:
        eyeX += 0.5f;
        std::cout << "Eye X: " << eyeX << std::endl;
        break;
    case GLUT_KEY_UP:
        eyeY += 0.5f;
        std::cout << "Eye Y: " << eyeY << std::endl;
        break;
    case GLUT_KEY_DOWN:
        eyeY -= 0.5f;
        std::cout << "Eye Y: " << eyeY << std::endl;
        break;
    case GLUT_KEY_PAGE_UP:
        eyeZ += 0.5f;
        std::cout << "Eye Z: " << eyeZ << std::endl;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eyeZ -= 0.5f;
        std::cout << "Eye Z: " << eyeZ << std::endl;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Orthographic Projection with Near/Far Planes");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    std::cout << "\n=== ORTHOGRAPHIC PROJECTION DEMO ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "+/- - Zoom in/out (adjust bounds)" << std::endl;
    std::cout << "N/F - Adjust near plane closer/farther" << std::endl;
    std::cout << "G/H - Adjust far plane farther/closer" << std::endl;
    std::cout << "Arrow Keys - Move camera (X,Y)" << std::endl;
    std::cout << "Page Up/Down - Move camera (Z)" << std::endl;
    std::cout << "I - Toggle info display" << std::endl;
    std::cout << "R - Reset all parameters" << std::endl;
    std::cout << "ESC - Exit" << std::endl;
    std::cout << "====================================" << std::endl;

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}