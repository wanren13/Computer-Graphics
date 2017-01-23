#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "LoadShaders.h"
#include "vec_mat.h"
#include "cube.h"
#include "Models.h"
#include "axes.h"
#include "SphericalCamera.h"
#include "YPRCamera.h"

#define PI 3.14159265358979323846
#define PI_DIV_180 0.017453292519943296
#define deg PI_DIV_180

#define BUFFER_OFFSET(x) ((const void*) (x))

GLint vPosition = 0;
GLint vColor = 1;

GLenum  mode = GL_FILL;
GLuint program;

GLboolean backcolorblack = GL_TRUE;
GLboolean drawCube = GL_TRUE;

cube c1(1, 0, 0);
axes coords;

/*  Set mouse tracking.  */
int mouseDown = 0;
int startX = 0;
int startY = 0;

GLboolean drawAxes = GL_TRUE;

GLint winWidth;
GLint winHeight;

mat4 projection;
mat4 model;
mat4 view;

SphericalCamera sphcamera;
YPRCamera yprcamera;
GLint CameraNumber = 1;

Models obj;
GLint rotang = 0;

void printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        std::cout << "OpenGL Error: " << errString << std::endl;
    }
}

void init()
{
    obj.createSphereOBJ(1, 20, 20);
    obj.load(vPosition, vColor);
    obj.setDrawBorder(GL_TRUE);
    obj.setColor(1, 0, 0);

    sphcamera.setPosition(5.0, 30, 30);
    yprcamera.setPosition(5,5,5);
    yprcamera.setView(-5, -5, -5);
    yprcamera.setUpViaY();

    c1.load(vPosition, vColor);
    coords.load(vPosition, vColor);
    model = Scale(1);

    glEnable(GL_DEPTH_TEST);

//  Load the shaders

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "vertexShader.vert"},
        {GL_FRAGMENT_SHADER, "PassThrough.frag"},
        {GL_NONE, NULL}
    };

    program = LoadShaders(shaders);
    glUseProgram(program);
}

void reshape(GLint width, GLint height)
{
    winWidth = width;
    winHeight = height;

    glViewport(0, 0, width, height);
    projection = Perspective(60.0f, (float)width/(float)height, 0.001f, 100.0f);
    glutPostRedisplay();
}

void SpKeyFcn(int key, int x, int y)
{
    GLint ctrlDown = glutGetModifiers() & GLUT_ACTIVE_CTRL;
    GLint altDown = glutGetModifiers() & GLUT_ACTIVE_ALT;
    GLint shiftDown = glutGetModifiers() & GLUT_ACTIVE_SHIFT;

    switch (key)
    {
    case GLUT_KEY_F1:
        glEnable(GL_DEPTH_TEST);
        break;

    case GLUT_KEY_F2:
        glDisable(GL_DEPTH_TEST);
        break;

    case GLUT_KEY_F3:
        glEnable(GL_CULL_FACE);
        break;

    case GLUT_KEY_F4:
        glDisable(GL_CULL_FACE);
        break;

    case GLUT_KEY_F9:
        rotang++;
        model = Rotate(rotang, 1, 1, 1);
        break;

    case GLUT_KEY_F10:
        rotang--;
        model = Rotate(rotang, 1, 1, 1);
        break;

    case GLUT_KEY_F11:
        rotang = 0;
        model = Rotate(rotang, 1, 1, 1);
        break;
    }

    if (CameraNumber == 1)
    {
        if (ctrlDown)
        {
            switch (key)
            {
            case GLUT_KEY_DOWN:
                sphcamera.addR(-0.2);
                break;

            case GLUT_KEY_UP:
                sphcamera.addR(0.2);
                break;
            }
        }
        else
        {
            switch (key)
            {
            case GLUT_KEY_UP:
                sphcamera.addPsy(-2);
                break;

            case GLUT_KEY_DOWN:
                sphcamera.addPsy(2);
                break;

            case GLUT_KEY_LEFT:
                sphcamera.addTheta(-2);
                break;

            case GLUT_KEY_RIGHT:
                sphcamera.addTheta(2);
                break;
            }
        }
    }
    else if (CameraNumber == 2)
    {

        if (ctrlDown)
        {
            switch (key)
            {
            case GLUT_KEY_DOWN:
                yprcamera.moveForward(-0.2);
                break;

            case GLUT_KEY_UP:
                yprcamera.moveForward(0.2);
                break;
            }
        }
        else if (altDown)
        {
            switch (key)
            {
            case GLUT_KEY_LEFT:
                yprcamera.addRoll(-1);
                break;

            case GLUT_KEY_RIGHT:
                yprcamera.addRoll(1);
                break;
            }
        }
        else if (shiftDown)
        {
            switch (key)
            {
            case GLUT_KEY_LEFT:
                yprcamera.moveRight(-0.2);
                break;

            case GLUT_KEY_RIGHT:
                yprcamera.moveRight(0.2);
                break;

            case GLUT_KEY_UP:
                yprcamera.moveUp(0.2);
                break;

            case GLUT_KEY_DOWN:
                yprcamera.moveUp(-0.2);
                break;
            }
        }
        else
        {
            switch (key)
            {
            case GLUT_KEY_UP:
                yprcamera.addPitch(1);
                break;

            case GLUT_KEY_DOWN:
                yprcamera.addPitch(-1);
                break;

            case GLUT_KEY_LEFT:
                yprcamera.addYaw(1);
                break;

            case GLUT_KEY_RIGHT:
                yprcamera.addYaw(-1);
                break;
            }
        }
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    GLint ctrlDown = glutGetModifiers() & GLUT_ACTIVE_CTRL;
    GLint altDown = glutGetModifiers() & GLUT_ACTIVE_ALT;

    switch(key)
    {
    case 033:  // ASCII Escape Key Code
        exit(EXIT_SUCCESS);
        break;

    case 'm':
        if (mode == GL_FILL)
            mode = GL_LINE;
        else
            mode = GL_FILL;

        glPolygonMode(GL_FRONT_AND_BACK, mode);
        break;

    case 'c':
        if (backcolorblack)
        {
            glClearColor(1, 1, 1, 1);
            backcolorblack = GL_FALSE;
        }
        else
        {
            glClearColor(0, 0, 0, 1);
            backcolorblack = GL_TRUE;
        }
        break;

    case 'g':
        c1.setColor(0, 1, 0);
        break;

    case 't':
        c1.setBorderColor(1, 0, 0);
        break;

    case 'h':
        c1.setBorderColor(0, 0, 1);
        break;

    case 'n':
        c1.setBorderColor(0, 1, 0);
        break;

    case 'y':
        c1.setBorderColor(0, 0, 0);
        break;

    case 'r':
        c1.setColor(1, 0, 0);
        break;

    case 'b':
        c1.setColor(0, 0, 1);
        break;

    case 'q':
        c1.setColorCube(GL_TRUE);
        break;

    case 'w':
        c1.setColorCube(GL_FALSE);
        break;

    case 'a':
        c1.setDrawBorder(GL_TRUE);
        break;

    case 's':
        c1.setDrawBorder(GL_FALSE);
        break;

    case 'z':
        drawAxes = GL_TRUE;
        break;

    case 'x':
        drawAxes = GL_FALSE;
        break;

    case 'o':
        c1.setDrawFaces(GL_TRUE);
        break;

    case 'p':
        c1.setDrawFaces(GL_FALSE);
        break;

    case '1':
        CameraNumber = 1;
        break;

    case '2':
        CameraNumber = 2;
        break;

    case '3':
        drawCube = GL_TRUE;
        break;

    case '4':
        drawCube = GL_FALSE;
        break;

    }

    if (altDown)
    {
        switch(key)
        {
        case 'g':
            obj.createCylinderOBJ(1, 1, 3, 20, 20);
            break;
        case 'h':
            obj.createCylinderOBJ(0, 1, 3, 20, 20);
            break;
        case 'j':
            obj.createCylinderOBJ(1, 2, 3, 20, 20);
            break;
        case 'k':
            obj.createCylinderOBJ(3, 1, 3, 20, 20);
            break;
        case 'v':
        {
            GLfloat x[] = {0, 1, 2};
            GLfloat y[] = {0.3, 0.5, 0.1};
            obj.createLatheOBJ(x, y, 3, 20);
        }
        break;
        case 'b':
        {
            GLfloat x[] = {0.5, 0.7, 0.9, 1.2, 2.1, 2.2, 2.3, 2.7, 3.1};
            GLfloat y[] = {0.3, 0.5, 0.1, 0.3, 0.5, 0.1, 0.3, 0.5, 0.1};
            obj.createLatheOBJ(x, y, 9, 20);
        }
        break;
        case 'n':
        {
            GLfloat x[30];
            GLfloat y[30];
            x[0] = 0;
            for (int i = 1; i < 30; i++)
                x[i] = x[i-1] + 0.5*rand()/RAND_MAX;
            for (int i = 0; i < 30; i++)
                y[i] = 2.0*rand()/RAND_MAX;
            obj.createLatheOBJ(x, y, 30, 20);
        }
        break;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

        case 'q':
            obj.createSphereOBJ(1, 20, 20);
            break;
        case 'w':
            //obj.createPartialSphereOBJ(1, 30, 30, -PI, PI/2, -PI/4, PI/4);
            obj.createPartialSphereOBJ(1, 30, 30, -PI, PI/2, -PI/2, PI/4);
            break;
        case 'e':
            obj.createTorusOBJ(1, 2, 25, 25);
            break;
        case 'r':
            obj.createPartialTorusOBJ(1, 2, 25, 25, 0, PI/2, 0, PI);
            break;
        case 't':
            obj.createTrefoilOBJ(0.5, 1.5, 2, 0.2, 1.0, 200, 20);
            break;
        case 'y':
            obj.createUmbilicTorusOBJ(100, 20);
            break;
        case 'u':
            obj.createBraidedTorusOBJ(0.5, 1.25, 2, 0.2, 1.0, 200, 20);
            break;
        case 'i':
            obj.createExpHornOBJ(0.5, 1, 2, 20, 20);
            break;
        case 'o':
            obj.createQuadHornOBJ(0.5, 1, 2, 20, 20);
            break;
        case 'p':
            obj.createHelicalTorusOBJ(1.0, 2.0, 0.2, -2*PI, 2*PI, 50, 20);
            break;
        case 'a':
            obj.createMobiusOBJ(-0.5, 0.5, 30, 30);
            break;
        case 's':
            obj.createTessellatedWallOBJ(2, 2, 1, 1);
            break;
        case 'd':
            obj.createTessellatedWallOBJ(2, 2, 30, 30);
            break;
        case 'f':
            obj.createTessellatedWallOBJ(2, 2, 200, 200);
            break;
        }
        obj.reloadData();
    }

    glutPostRedisplay();
}

void detectUpDown(int button, int state, int x, int y)
{
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
    {
        mouseDown = 1;
        startX = x;
        startY = y;
    }
    else
        mouseDown = 0;
}

void detectMove(int x, int y)
{
    GLint ctrlDown = glutGetModifiers() & GLUT_ACTIVE_CTRL;
    GLint altDown = glutGetModifiers() & GLUT_ACTIVE_ALT;
    GLint shiftDown = glutGetModifiers() & GLUT_ACTIVE_SHIFT;

    if (mouseDown == 1)
    {
        if (ctrlDown)
        {
            sphcamera.addR((startY-y)/20.0);
            startY = y;
        }
        else
        {
            sphcamera.addTheta((x-startX)/2.0);
            startX = x;

            sphcamera.addPsy((y-startY)/2.0);
            startY = y;
        }

        glutPostRedisplay();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (CameraNumber == 1)
        view = sphcamera.lookAt();
    else if (CameraNumber == 2)
        view = yprcamera.lookAt();

    mat4 axesscale = Scale(20);

    GLuint projloc = glGetUniformLocation(program, "ProjModelView");
    glUniformMatrix4fv(projloc, 1, GL_TRUE, projection*view*axesscale);

    if (drawAxes)
        coords.draw();

    projloc = glGetUniformLocation(program, "ProjModelView");
    glUniformMatrix4fv(projloc, 1, GL_TRUE, projection*view*model);

    if (drawCube)
        c1.draw();
    else
        obj.draw();

    glutSwapBuffers();
    printOpenGLErrors();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow(argv[0]);

    if (glewInit())
    {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(EXIT_FAILURE);
    }

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutSpecialFunc(SpKeyFcn);
    glutMouseFunc(detectUpDown);
    glutMotionFunc(detectMove);

    glutMainLoop();
}
