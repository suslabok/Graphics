#include <GL/glut.h>
#include <iostream>

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    int width = glutGet(GLUT_SCREEN_WIDTH);
    int height = glutGet(GLUT_SCREEN_HEIGHT);

    std::cout << "Screen resolution: " << width << " x " << height << std::endl;

    return 0;
}
