/************************************
COSC 482 Computer Graphics
Homework 2
Ren Wan
Feb 24, 2014
************************************/

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "rectangle.h"
#include "polygon.h"

#include "LoadShaders.h"
#include "util.h"

#define TPS 120

rectangle *rects;
polygon *polygs;

int numOfSquares;
int numOfPolygons;

GLenum  mode = GL_FILL;

clock_t timer;
clock_t startTime;

double currSec = 0;
double prevSec = 0;
double diffSec;

double sum = 0.0;
long frames = 0;

GLuint program;
GLuint projloc;
GLuint modeloc;

// function prototypes
void printOpenGLErrors();
void display();
void keyboard(unsigned char key, int x, int y);
void reshape(GLint width, GLint height);
void init();
void update();


// main function
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(512, 512);
    glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutCreateWindow("HW_Animation");

    if (glewInit())
    {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit( EXIT_FAILURE );
    }

    std::cout << "Press ESC or select Quit to properly exit." << std::endl;

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);

    glutMainLoop();
}


void printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s\n", errString);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    for(int i=0; i<numOfSquares; i++){
        glUniformMatrix4fv(modeloc, 1, GL_TRUE, rects[i].getTransformationMatrix());
        rects[i].draw();
    }
    for(int i=0; i<numOfPolygons; i++){
        glUniformMatrix4fv(modeloc, 1, GL_TRUE, polygs[i].getTransformationMatrix());
        polygs[i].draw();
    }

    glutSwapBuffers();

    glFinish();

    printOpenGLErrors();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 033:  // ASCII Escape Key Code
        delete[] rects;
        delete[] polygs;
        exit(EXIT_SUCCESS);
        break;

    case 'm':
        if (mode == GL_FILL)
            mode = GL_LINE;
        else
            mode = GL_FILL;

        glPolygonMode(GL_FRONT_AND_BACK, mode);
        break;
    }

    glutPostRedisplay();
}


void reshape(GLint width, GLint height)
{
    glViewport(0, 0, width, height);

    mat4 projection;

    if (height == 0 || width == 0)
        projection = Ortho2D(-1, 1, -1, 1);
    else if (width > height)
    {
        projection = Ortho2D(-10.0*width/height, 10.0*width/height, -10, 10);
        rectangle::MAX_TRANSLATEX = 10.0*width/height;
        rectangle::MIN_TRANSLATEX = -10.0*width/height;
        rectangle::MAX_TRANSLATEY = 10.0;
        rectangle::MIN_TRANSLATEY = -10.0;

        polygon::MAX_TRANSLATEX = 10.0*width/height;
        polygon::MIN_TRANSLATEX = -10.0*width/height;
        polygon::MAX_TRANSLATEY = 10.0;
        polygon::MIN_TRANSLATEY = -10.0;
    }
    else
    {
        projection = Ortho2D(-10, 10, -10.0*height/width, 10.0*height/width);
        rectangle::MAX_TRANSLATEX = 10.0;
        rectangle::MIN_TRANSLATEX = -10.0;
        rectangle::MAX_TRANSLATEY = 10.0*height/width;
        rectangle::MIN_TRANSLATEY = -10.0*height/width;

        polygon::MAX_TRANSLATEX = 10.0;
        polygon::MIN_TRANSLATEX = -10.0;
        polygon::MAX_TRANSLATEY = 10.0*height/width;
        polygon::MIN_TRANSLATEY = -10.0*height/width;
    }

    glUniformMatrix4fv(projloc, 1, GL_TRUE, projection);
//*/
}


void init()
{
    srand(time(NULL));

    numOfSquares = randI(100, 300);
    numOfPolygons = randI(100, 300);

    GLfloat red;
    GLfloat green;
    GLfloat blue;

    GLfloat rotation;
    GLfloat scale;
    GLfloat translateX;
    GLfloat translateY;

    GLfloat rotationSpeed;
    GLfloat scaleSpeed;
    GLfloat translateXSpeed;
    GLfloat translateYSpeed;

    rects = new rectangle[numOfSquares];       //
    for(int i = 0; i < numOfSquares; i++)      //
    {
        red     = randGLF(0.0, 1.0);
        green   = randGLF(0.0, 1.0);
        blue    = randGLF(0.0, 1.0);

        translateX      = randGLF(-7.5, 7.5);
        translateY      = randGLF(-7.5, 7.5);
        rotation        = randGLF(0.0, 360.0);
        scale           = randGLF(0.8, 1.2);
        rotationSpeed   = randGLF(-2.0, 2.0);
        scaleSpeed      = randGLF(0.98, 1.0);
        translateXSpeed = randGLF(-0.2, 0.2);
        translateYSpeed = randGLF(-0.2, 0.2);

        rects[i].setColor(red, green, blue);
        rects[i].setRotation(rotation);
        rects[i].setScale(scale);
        rects[i].setTranslate(translateX, translateY);
        rects[i].setRotationSpeed(rotationSpeed);
        rects[i].setScaleSpeed(scaleSpeed);
        rects[i].setTranslateSpeed(translateXSpeed, translateYSpeed);
    }

    GLint sides;
    GLfloat radius;

    polygs = new polygon[numOfPolygons];
    for(int i = 0; i < numOfPolygons; i++)
    {
        red     = randGLF(0.0, 1.0);
        green   = randGLF(0.0, 1.0);
        blue    = randGLF(0.0, 1.0);

        sides   = randI(3, 1000);
        radius  = randGLF(0.25, 0.5);

        translateX      = randGLF(-7.5, 7.5);
        translateY      = randGLF(-7.5, 7.5);
        rotation        = randGLF(0.0, 360.0);
        scale           = randGLF(0.8, 1.2);
        rotationSpeed   = randGLF(-2.0, 2.0);
        scaleSpeed      = randGLF(0.98, 1.0);
        translateXSpeed = randGLF(-0.2, 0.2);
        translateYSpeed = randGLF(-0.2, 0.2);

        polygs[i].setAll(sides, radius, red, green, blue);
        polygs[i].setRotation(rotation);
        polygs[i].setScale(scale);
        polygs[i].setTranslate(translateX, translateY);
        polygs[i].setRotationSpeed(rotationSpeed);
        polygs[i].setScaleSpeed(scaleSpeed);
        polygs[i].setTranslateSpeed(translateXSpeed, translateYSpeed);

//        std::cout << i << std::endl;
    }

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "vertexShader.vert"},
        {GL_FRAGMENT_SHADER, "PassThrough.frag"},
        {GL_NONE, NULL}
    };

    program = LoadShaders(shaders);
    glUseProgram(program);

    projloc = glGetUniformLocation(program, "Projection");
    modeloc = glGetUniformLocation(program, "Model");

    startTime = clock();
}


void update()
{
    timer = clock();

    currSec = (double)(timer - startTime)/CLOCKS_PER_SEC;

    diffSec = currSec - prevSec;

    prevSec = currSec;

    sum += diffSec;

    if(sum >= 1.0/TPS)
    {
        sum = 0;

        for(int i = 0; i < numOfSquares; i++)
            rects[i].updateTransformations();

        for(int i = 0; i < numOfPolygons; i++)
            polygs[i].updateTransformations();
    }

    glutPostRedisplay();

    frames++;

    if (frames % 5000 == 0)
        std::cout << (int)(frames/currSec) << std::endl;

}

