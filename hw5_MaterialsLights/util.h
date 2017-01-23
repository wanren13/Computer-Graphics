#ifndef ULTI_H_INCLUDED
#define ULTI_H_INCLUDED

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

GLfloat randGLF(GLfloat low, GLfloat high)
{
    return low + static_cast <GLfloat> (rand()) /( static_cast <GLfloat> (RAND_MAX/(high-low)));
}

float randF(float low, float high)
{
    return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

double randD(double low, double high)
{
    return low + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(high-low)));
}

GLint randGLI(GLint low, GLint high)
{
    return low + rand()%high;
}

int randI(int low, int high)
{
    return low + rand()%high;
}

GLfloat a2cX(int x)
{
    GLint w = glutGet(GLUT_WINDOW_WIDTH);
    return -1.0+2.0*x/w;
}

GLfloat a2cY(int y)
{
    GLint h = glutGet(GLUT_WINDOW_HEIGHT);
    return -1.0+2.0*(h-y)/h;
}

int c2aX(GLfloat x)
{
    GLint w = glutGet(GLUT_WINDOW_WIDTH);
    return (x+1.0)/2.0*w;
}

int c2aY(GLfloat y)
{
    GLint h = glutGet(GLUT_WINDOW_HEIGHT);
    return (1-(y+1.0)/2.0)*h;
}

#endif // ULTI_H_INCLUDED
