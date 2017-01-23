/************************************
COSC 482 Computer Graphics
Homework 3
Ren Wan
Feb 25, 2014
************************************/

#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>

#include "Circle.h"
#include "Cross.h"
#include "Line.h"
#include "Board.h"
#include "LoadShaders.h"

#define PI 3.1415926535897932385

using namespace std;

enum Mark {empty, xmark, omark};
enum Turn {xturn, oturn};
enum Won {xwon, owon, draw, nowon};

// Array for recording player's locations
Mark Grid[] =
{
    empty, empty, empty,
    empty, empty, empty,
    empty, empty, empty
};

Turn turn = xturn;
Won  won  = nowon;

// Transformation matrix array
mat4 T[] =
{
    Translate(-1.0,  1.0, 0.0) * Scale(1.0) * RotateZ(0.0),
    Translate( 0.0,  1.0, 0.0) * Scale(1.0) * RotateZ(0.0),
    Translate( 1.0,  1.0, 0.0) * Scale(1.0) * RotateZ(0.0),
    Translate(-1.0,  0.0, 0.0) * Scale(1.0) * RotateZ(0.0),
    Translate( 0.0,  0.0, 0.0) * Scale(1.0) * RotateZ(0.0),
    Translate( 1.0,  0.0, 0.0) * Scale(1.0) * RotateZ(0.0),
    Translate(-1.0, -1.0, 0.0) * Scale(1.0) * RotateZ(0.0),
    Translate( 0.0, -1.0, 0.0) * Scale(1.0) * RotateZ(0.0),
    Translate( 1.0, -1.0, 0.0) * Scale(1.0) * RotateZ(0.0)
};

vector<Cross> xPlayer;
vector<Circle> oPlayer;

Board* b;              // Game board
Line*  l;              // Winning line

GLuint program;
GLuint projloc;
GLuint modeloc;

// function prototypes
void init();
void printOpenGLErrors();
void display();
void keyboard(unsigned char key, int x, int y);
void SpKeyFcn(int key, int x, int y);
void reshape(GLint width, GLint height);
void MouseEvent(int button, int state, int x, int y);
void update();



// main function
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(512, 512);
    glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutCreateWindow("HW_GAME");

    if (glewInit())
    {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit( EXIT_FAILURE );
    }

    std::cout << "Press ESC or select Quit to properly exit." << std::endl;

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpKeyFcn);
    glutMouseFunc(MouseEvent);
    glutReshapeFunc(reshape);

    glutMainLoop();
}


void init()
{
//    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    b = new Board();
    l = new Line();

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

    glUniformMatrix4fv(modeloc, 1, GL_TRUE, b->getT());
    b->draw();

//*
    for(int i=0; i<xPlayer.size(); i++)
    {
        glUniformMatrix4fv(modeloc, 1, GL_TRUE, xPlayer[i].getT());
        xPlayer[i].draw();
    }
    for(int i=0; i<oPlayer.size(); i++)
    {
        glUniformMatrix4fv(modeloc, 1, GL_TRUE, oPlayer[i].getT());
        oPlayer[i].draw();
    }
//*/

    if(won == xwon || won == owon)
    {
        glUniformMatrix4fv(modeloc, 1, GL_TRUE, l->getT());
        l->draw();
    }

    glutSwapBuffers();

    glFinish();

    printOpenGLErrors();
}



void reshape(GLint width, GLint height)
{
    glViewport(0, 0, width, height);

    mat4 projection;

    if (height == 0 || width == 0)
        projection = Ortho2D(-1, 1, -1, 1);
    else if (width > height)
        projection = Ortho2D(-1.5*width/height, 1.5*width/height, -1.5, 1.5);
    else
        projection = Ortho2D(-1.5, 1.5, -1.5*height/width, 1.5*height/width);

    glUniformMatrix4fv(projloc, 1, GL_TRUE, projection);
}



void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 033:  // ASCII Escape Key Code
        delete b;
        delete l;
        exit(EXIT_SUCCESS);
        break;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glutPostRedisplay();
}



void SpKeyFcn(int key, int x, int y)
{
    if(key == GLUT_KEY_F1)
    {
        for(int i = 0; i < 9; i++)
            Grid[i] = empty;

        xPlayer.clear();
        oPlayer.clear();
        turn = xturn;
        update();
    }

    glutPostRedisplay();
}



void MouseEvent(int button, int state, int x, int y)
{
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && won == nowon)
    {
        GLint w = glutGet(GLUT_WINDOW_WIDTH);
        GLint h = glutGet(GLUT_WINDOW_HEIGHT);

        int index = 9;

        if(w>h && x>(w-h)/2 && x<(w+h)/2)
        {
            if(x<w/2-h/6)
            {
                if((GLfloat)y/h <= 1.0/3)        index = 0;
                else if((GLfloat)y/h <= 2.0/3)   index = 3;
                else                             index = 6;
            }
            else if(x<w/2+h/6)
            {
                if((GLfloat)y/h <= 1.0/3)        index = 1;
                else if((GLfloat)y/h <= 2.0/3)   index = 4;
                else                             index = 7;
            }
            else
            {
                if((GLfloat)y/h <= 1.0/3)        index = 2;
                else if((GLfloat)y/h <= 2.0/3)   index = 5;
                else                             index = 8;
            }
        }
        else if ((w<h && y>(h-w)/2 && y<(h+w)/2) || w == h)
        {
            if(y<h/2-w/6)
            {
                if((GLfloat)x/w <= 1.0/3)        index = 0;
                else if((GLfloat)x/w <= 2.0/3)   index = 1;
                else                             index = 2;
            }
            else if(y<h/2+w/6)
            {
                if((GLfloat)x/w <= 1.0/3)        index = 3;
                else if((GLfloat)x/w <= 2.0/3)   index = 4;
                else                             index = 5;
            }
            else
            {
                if((GLfloat)x/w <= 1.0/3)        index = 6;
                else if((GLfloat)x/w <= 2.0/3)   index = 7;
                else                             index = 8;
            }
        }

//        cout << "Index is : " << index << endl;

        if(index != 9)
        {
            if(Grid[index] == empty)
            {
                if(turn == xturn)
                {
                    xPlayer.push_back(Cross());
                    xPlayer[xPlayer.size()-1].setT(T[index]);
                    Grid[index] = xmark;
                    turn = oturn;
                }
                else
                {
                    oPlayer.push_back(Circle());
                    oPlayer[oPlayer.size()-1].setT(T[index]);
                    Grid[index] = omark;
                    turn = xturn;
                }
            }

            update();
        }
    }
}



void update()
{
    if(Grid[0] == Grid[1] && Grid[0] == Grid[2] && Grid[0] != empty)
    {
        won = Grid[0]==xmark?xwon:owon;
        l->setT(0.0, 1.0, 1.0, 0.0);
    }
    else if(Grid[3] == Grid[4] && Grid[3] == Grid[5] && Grid[3] != empty)
    {
        won = Grid[3]==xmark?xwon:owon;
        l->setT(0.0, 0.0, 1.0, 0.0);
    }
    else if(Grid[6] == Grid[7] && Grid[6] == Grid[8] && Grid[6] != empty)
    {
        won = Grid[6]==xmark?xwon:owon;
        l->setT(0.0, -1.0, 1.0, 0.0);
    }
    else if(Grid[0] == Grid[3] && Grid[0] == Grid[6] && Grid[0] != empty)
    {
        won = Grid[0]==xmark?xwon:owon;
        l->setT(-1.0, 0.0, 1.0, 90.0);
    }
    else if(Grid[1] == Grid[4] && Grid[1] == Grid[7] && Grid[1] != empty)
    {
        won = Grid[1]==xmark?xwon:owon;
        l->setT(0.0, 0.0, 1.0, 90.0);
    }
    else if(Grid[2] == Grid[5] && Grid[2] == Grid[8] && Grid[2] != empty)
    {
        won = Grid[2]==xmark?xwon:owon;
        l->setT(1.0, 0.0, 1.0, 90.0);
    }
    else if(Grid[0] == Grid[4] && Grid[0] == Grid[8] && Grid[0] != empty)
    {
        won = Grid[0]==xmark?xwon:owon;
        l->setT(0.0, 0.0, 1.0/cos(PI/4), 135.0);
    }
    else if(Grid[2] == Grid[4] && Grid[2] == Grid[6] && Grid[2] != empty)
    {
        won = Grid[2]==xmark?xwon:owon;
        l->setT(0.0, 0.0, 1.0/cos(PI/4), 45.0);
    }
    else if(Grid[0] != empty && Grid[1] != empty && Grid[2] != empty &&
            Grid[3] != empty && Grid[4] != empty && Grid[5] != empty &&
            Grid[6] != empty && Grid[7] != empty && Grid[8] != empty)
        won = draw;
    else
        won = nowon;

    if(won == nowon)
    {
        if(turn == xturn)    glutSetWindowTitle("X's turn");
        else                 glutSetWindowTitle("O's turn");
    }
    else
    {
        if(won == xwon)      glutSetWindowTitle("X Won");
        else if(won == owon) glutSetWindowTitle("O Won");
        else                 glutSetWindowTitle("Draw");
    }
}
