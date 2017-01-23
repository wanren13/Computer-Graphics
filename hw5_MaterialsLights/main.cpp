/*************************************************************************
COSC 482 Computer Graphics
Homework 5
Ren Wan
April 7th, 2014
*************************************************************************/


#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include "LoadShaders.h"
#include "vec_mat.h"
#include "cube.h"
#include "Models.h"
#include "axes.h"
#include "teapot.h"
#include "SphericalCamera.h"
#include "YPRCamera.h"
#include "YPR2DCamera.h"
#include "MaterialLight.h"
#include "util.h"

#define PI 3.14159265358979323846
#define PI_DIV_180 0.017453292519943296
#define deg PI_DIV_180
#define TimesPerSecond 100

#define BUFFER_OFFSET(x) ((const void*) (x))

using namespace std;

clock_t timer;
clock_t startTime;

double currSec = 0;
double prevSec = 0;
double diffSec;

double sum = 0.0;

GLint vPosition = 0;
GLint vColor = 1;
GLint vNormal = 2;

GLenum mode = GL_FILL;
GLuint program;

GLboolean backcolorblack = GL_TRUE;
GLboolean updateTeaPot   = GL_FALSE;
GLboolean updateTree     = GL_FALSE;
GLboolean enableFog      = GL_FALSE;
GLboolean camera2donly   = GL_FALSE;

bool fogon = false;
float maxdistance = 7;

axes coords;

/*  Set mouse tracking.  */
int mouseDown = 0;
int startX = 0;
int startY = 0;

GLboolean drawAxes   = GL_TRUE;
GLboolean drawTeaPot = GL_FALSE;
GLboolean drawCube   = GL_FALSE;
GLboolean drawLight  = GL_TRUE;

GLint winWidth;
GLint winHeight;

mat4 projection;
mat4 view;
vec4 GlobalAmbient;

//YPR2DCamera lightPosition[2];
SphericalCamera sphcamera;
YPRCamera yprcamera;
YPR2DCamera ypr2dcamera;
GLint CameraNumber = 1;

Models obj[101];
teapot tPot;
Light Lts[10];
mat4 modelMatrices[101];
Material Mats[101];
Models lightobj;

GLint r[] = {3, 4, 5, 6, 7};
double tPotAng[5];
double tPotRotateAng[] = {0, 0, 0, 0, 0};
double tPotSpeed[5];
double tPotRotateSpeed[5];
mat4 tPotMatrices[15];
Material tMats[15];
vec3 tPotAxis[5];

float twinkleTimes[100];
float slightTimers[100];
Models slightobj;

cube cubeObj;
mat4 cubeMatrices[121];

int numTeaPots = 1;
int numObjects = 1;
int numLights = 1;
int numCubes  = 121;

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

void LoadLights(Light Lt[], int num)
{
    using namespace std;
    string arrayname = "Lt";  // array name in the shader.

    for (int i = 0; i < num; i++)
    {
        char n[10];
        itoa(i, n, 10);
        string arraypos = arrayname;
        arraypos += "[";
        arraypos += n;
        arraypos += "]";

        string locID = arraypos;
        locID.append(".on");
        glUniform1i(glGetUniformLocation(program, locID.c_str()), Lt[i].on);

        locID = arraypos;
        locID.append(".position");
        glUniform4fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].position);

        locID = arraypos;
        locID.append(".ambient");
        glUniform4fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].ambient);

        locID = arraypos;
        locID.append(".diffuse");
        glUniform4fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].diffuse);

        locID = arraypos;
        locID.append(".specular");
        glUniform4fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].specular);

        locID = arraypos;
        locID.append(".spotDirection");
        glUniform3fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].spotDirection);

        locID = arraypos;
        locID.append(".attenuation");
        glUniform3fv(glGetUniformLocation(program, locID.c_str()), 1, Lt[i].attenuation);

        locID = arraypos;
        locID.append(".spotCutoff");
        glUniform1f(glGetUniformLocation(program, locID.c_str()), Lt[i].spotCutoff);

        locID = arraypos;
        locID.append(".spotExponent");
        glUniform1f(glGetUniformLocation(program, locID.c_str()), Lt[i].spotExponent);
    }
}


void TurnLightsOn(Light Lt[], int num)
{
    using namespace std;
    string arrayname = "Lt";  // array name in the shader.

    for (int i = 0; i < num; i++)
    {
        char n[10];
        itoa(i, n, 10);
        string arraypos = arrayname;
        arraypos += "[";
        arraypos += n;
        arraypos += "]";

        Lt[i].on = true;
        glUniform1i(glGetUniformLocation(program, arraypos.append(".on").c_str()), Lt[i].on);
    }
}

void TurnLightsOff(Light Lt[], int num)
{
    using namespace std;
    string arrayname = "Lt";  // array name in the shader.

    for (int i = 0; i < num; i++)
    {
        char n[10];
        itoa(i, n, 10);
        string arraypos = arrayname;
        arraypos += "[";
        arraypos += n;
        arraypos += "]";

        Lt[i].on = false;
        glUniform1i(glGetUniformLocation(program, arraypos.append(".on").c_str()), Lt[i].on);
    }
}

void LoadMaterial(Material Mat)
{
    glUniform4fv(glGetUniformLocation(program, "Mat.ambient"), 1, Mat.ambient);
    glUniform4fv(glGetUniformLocation(program, "Mat.diffuse"), 1, Mat.diffuse);
    glUniform4fv(glGetUniformLocation(program, "Mat.specular"), 1, Mat.specular);
    glUniform4fv(glGetUniformLocation(program, "Mat.emission"), 1, Mat.emission);
    glUniform1f(glGetUniformLocation(program, "Mat.shininess"), Mat.shininess);
}

void createLayout1()
{
    CameraNumber=CameraNumber==3?1:CameraNumber;
    numLights = 3;
    numObjects = 3;
    numTeaPots = 0;

    for (int i = 0; i < 3; i++)
        Lts[i] = defaultLight;

    Lts[0].position = vec4(2, 3, 4, 1);
    Lts[1].position = vec4(-4, 3, -4, 1);
    Lts[2].position = vec4(1, 3, -4, 1);

    float fact = 0.8;
    for (int i = 0; i < 3; i++)
    {
        Lts[i].diffuse = fact*Lts[i].diffuse;
        Lts[i].specular = fact*Lts[i].specular;
    }

    obj[0].createSphereOBJ(1, 20, 20);
    obj[1].createTorusOBJ(0.5, 1, 30, 30);
    obj[2].createTrefoilOBJ(0.5, 1.5, 2, 0.2, 1.0, 200, 20);

    for (int i = 0; i < 3; i++)
        obj[i].reloadData();

    Mats[0] = bronze;
    Mats[1] = chrome;
    Mats[2] = polishedgold;

    modelMatrices[0] = Translate(1, 0, 2);
    modelMatrices[1] = Translate(-1, 0, -2) * Rotate(45, 1, 2, 3);
    modelMatrices[2] = Rotate(90, 1, 0, 0);

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);
}

void createLayout2()
{
    CameraNumber=CameraNumber==3?1:CameraNumber;
    numLights = 3;
    numObjects = 1;
    numTeaPots = 0;

    GLfloat x[30];
    GLfloat y[30];
    x[0] = 0;

    for (int i = 1; i < 30; i++)
        x[i] = x[i-1] + 0.5*rand()/RAND_MAX;
    for (int i = 0; i < 30; i++)
        y[i] = 0.25+0.5*rand()/RAND_MAX;

    obj[0].createLatheOBJ(x, y, 30, 100);
    obj[0].reloadData();

    for (int i = 0; i < 3; i++)
        Lts[i] = defaultLight;

    Lts[0].position = vec4(2, 3, 4, 1);
    Lts[1].position = vec4(-4, 3, -4, 1);
    Lts[2].position = vec4(1, 3, -4, 1);

    float fact = 0.8;
    for (int i = 0; i < 3; i++)
    {
        Lts[i].diffuse = fact*Lts[i].diffuse;
        Lts[i].specular = fact*Lts[i].specular;
    }

    Mats[0] = bluePlastic;

    modelMatrices[0] = Translate(0, 0, -3);

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);
}

void createLayout3()
{
    CameraNumber=CameraNumber==3?1:CameraNumber;
    numLights = 3;
    numObjects = 1;
    numTeaPots = 0;

    obj[0].createTessellatedWallOBJ(2, 2, 1, 1);
    obj[0].reloadData();

    modelMatrices[0] = Rotate(-90, 1, 0, 0)*Scale(10, 10, 10);

    Mats[0] = polishedcopper;

    for (int i = 0; i < 3; i++)
        Lts[i] = defaultLight;

    Lts[0].position = vec4(2, 3, 4, 1);
    Lts[1].position = vec4(-4, 3, -4, 1);
    Lts[2].position = vec4(1, 3, -4, 1);

    float fact = 0.5;
    for (int i = 0; i < 3; i++)
    {
        Lts[i].diffuse = fact*Lts[i].diffuse;
        Lts[i].specular = fact*Lts[i].specular;
        Lts[i].spotDirection = -1*Lts[i].position.toVec3();
        Lts[i].spotCutoff = 15;
    }

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);
}

void createLayout4()
{
    CameraNumber=CameraNumber==3?1:CameraNumber;
    numLights = 10;
    numObjects = 1;
    numTeaPots = 0;

    obj[0].createTessellatedWallOBJ(2, 2, 1, 1);
    obj[0].reloadData();

    modelMatrices[0] = Rotate(-90, 1, 0, 0)*Scale(10, 10, 10);

    Mats[0] = bronze;

    for (int i = 0; i < 10; i++)
    {
        Lts[i] = defaultLight;
        Lts[i].position = vec4(5*cos(2.0*PI/10.0*i), 3, 5*sin(2.0*PI/10.0*i), 1);
    }

    float fact = 0.25;
    for (int i = 0; i < 10; i++)
    {
        Lts[i].diffuse = fact*Lts[i].diffuse;
        Lts[i].specular = fact*Lts[i].specular;
        Lts[i].spotDirection = -1*Lts[i].position.toVec3();
        Lts[i].spotCutoff = 15;
    }

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);
}

void createLayout5()
{
    CameraNumber=CameraNumber==3?1:CameraNumber;
    numLights = 10;
    numObjects = 5;
    numTeaPots = 0;

    obj[0].createSphereOBJ(1, 30, 30);
    obj[1].createSphereOBJ(1, 30, 30);
    obj[2].createSphereOBJ(1, 30, 30);
    obj[3].createSphereOBJ(1, 30, 30);
    obj[4].createSphereOBJ(1, 30, 30);

    for (int i = 0; i < 5; i++)
        obj[i].reloadData();

    modelMatrices[0] = Identity();
    modelMatrices[1] = Translate(4, -1, 2);
    modelMatrices[2] = Translate(1, 3, 0);
    modelMatrices[3] = Translate(0, -4, 0);
    modelMatrices[4] = Translate(-1, 2, 1);

    Mats[0] = bronze;
    Mats[1] = gold;
    Mats[2] = pewter;
    Mats[3] = silver;
    Mats[4] = redPlastic;

    for (int i = 0; i < 10; i++)
    {
        Lts[i] = defaultLight;
        Lts[i].position = vec4(7*cos(2.0*PI/10.0*i), 0, 7*sin(2.0*PI/10.0*i), 1);
    }

    float fact = 0.25;
    for (int i = 0; i < 10; i++)
    {
        Lts[i].diffuse = fact*Lts[i].diffuse;
        Lts[i].specular = fact*Lts[i].specular;
    }

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);
}

void createLayout6()
{
    CameraNumber=CameraNumber==3?1:CameraNumber;
    numLights = 5;
    numObjects = 1;
    numTeaPots = 5;

    obj[0].createTessellatedWallOBJ(2, 2, 1, 1);
    obj[0].reloadData();

    modelMatrices[0] = Rotate(-90, 1, 0, 0)*Scale(10, 10, 10);

    Mats[0] = bronze;

    tMats[0] = polishedsilver;
    tMats[1] = polishedgold;
    tMats[2] = pewter;
    tMats[3] = redPlastic;
    tMats[4] = greenPlastic;

    for(int i = 0; i < numTeaPots; i++){
        tPotAng[i] = randD(0, 360);
        tPotSpeed[i] = randD(-0.01, 0.01);
        tPotRotateSpeed[i] = randD(-2, 2);
        tPotAxis[i] = vec3(randGLF(0,1), randGLF(0,1), randGLF(0,1));
    }

    for (int i = 0; i < numLights; i++)
    {
        Lts[i] = defaultLight;
        Lts[i].position = vec4(randGLF(-5, 5), randGLF(3, 6), randGLF(-5, 5), 1);
        Lts[i].spotDirection = vec3(randGLF(-1, 1), -1, randGLF(-1, 1));
        Lts[i].spotCutoff = randGLF(20, 40);
        Lts[i].ambient = vec4(0.2, 0.2, 0.2, 1.0); // extra lit, too dark to see teapots
        Lts[i].diffuse = vec4(defaultLight.diffuse[1]/2,
                              defaultLight.diffuse[2]/2,
                              defaultLight.diffuse[3]/2,
                              defaultLight.diffuse[4]);
        Lts[i].specular = vec4(defaultLight.specular[1]/2,
                               defaultLight.specular[2]/2,
                               defaultLight.specular[3]/2,
                               defaultLight.specular[4]);

    }

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);
}

void createLayout7()
{
    CameraNumber=CameraNumber==3?1:CameraNumber;
    numLights = 3;
    numObjects = 101;
    numTeaPots = 0;

    obj[100].createTessellatedWallOBJ(2, 2, 1, 1);
    obj[100].reloadData();

    modelMatrices[100] = Rotate(-90, 1, 0, 0)*Scale(10, 10, 10);

    Mats[100] = grass;

    for (int i = 0; i < numLights; i++)
        Lts[i] = defaultLight;

    Lts[0].position = vec4(2, 3, 4, 1);
    Lts[1].position = vec4(-4, 3, -4, 1);
    Lts[2].position = vec4(1, 3, 4, 1);

    GLfloat theta = 2*PI/100;

    for (int i = 0; i < 100; i++){
        modelMatrices[i] = Translate(theta*cos(5*theta)/2,
                                     (2*PI-theta)/2 + 0.1,
                                     theta*sin(5*theta)/2);

        obj[i].createSphereOBJ(0.05, 20, 20);
        obj[i].setDrawBorder(GL_FALSE);
        obj[i].reloadData();

        int color = randI(1, 3);
        switch(color)
        {
        case 1:
            Mats[i] = redDull;
            break;
        case 2:
            Mats[i] = greenDull;
            break;
        case 3:
            Mats[i] = blueDull;
        }
        theta = 2*i*PI/100;
        twinkleTimes[i] = randF(1, 4);
    }

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);
}

void createLayout8()
{
    CameraNumber=CameraNumber==3?1:CameraNumber;
    numLights = 3;
    numTeaPots = 15;
    numObjects = 1;

    obj[0].createTessellatedWallOBJ(2, 2, 1, 1);
    obj[0].reloadData();

    modelMatrices[0] = Rotate(-90, 1, 0, 0)*Scale(10, 10, 10);

    Mats[0] = grass;

    for (int i = 0; i < numLights; i++){
        Lts[i] = defaultLight;
        Lts[i].attenuation = vec3(1, 0, 0.05);
    }

    Lts[0].position = vec4(2, 3, 4, 1);
    Lts[1].position = vec4(-4, 3, -4, 1);
    Lts[2].position = vec4(1, 3, 4, 1);


    for(int i =0; i < 15; i++){
        int material = randI(1, 5);
        switch(material)
        {
        case 1:
            tMats[i] = bronze;
            break;
        case 2:
            tMats[i] = copper;
            break;
        case 3:
            tMats[i] = gold;
            break;
        case 4:
            tMats[i] = pewter;
            break;
        case 5:
            tMats[i] = silver;
            break;
        }
        tPotMatrices[i] = Translate(randGLF(-10, 10), 0.5, randGLF(-10, 10)) *
                Scale(1, 1, 1) * Rotate(randGLF(0, 360), vec3(0, 1, 0));
    }

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);

}

void createLayout9()
{
    CameraNumber=3;
    ypr2dcamera = YPR2DCamera(0.5);
    ypr2dcamera.PositionCamera(1, 0, 0, 1);

    numLights = 2;
    numTeaPots = 0;
    numObjects = 1;

    obj[0].createTessellatedWallOBJ(2, 2, 1, 1);
    obj[0].reloadData();

    modelMatrices[0] = Rotate(-90, 1, 0, 0)*Scale(10, 10, 10);

    Mats[0] = grass;

    for (int i = 0; i < numLights; i++){
        Lts[i] = defaultLight;
        Lts[i].spotDirection = vec3(0, 0, 1);
        Lts[i].spotCutoff = 25;
        Lts[i].spotExponent = 80;
        Lts[i].attenuation = vec3(1, 0.1, 0.005);
    }

    Lts[0].position = vec4(ypr2dcamera.getPosition() -
                            0.25 * ypr2dcamera.getRightVector() +
                            0.125 * ypr2dcamera.getView(), 1);
    Lts[1].position = vec4(ypr2dcamera.getPosition() +
                            0.25 * ypr2dcamera.getRightVector() +
                            0.125 * ypr2dcamera.getView(), 1);

    LoadLights(Lts, numLights);
    glUniform1i(glGetUniformLocation(program, "numLights"), numLights);
}


void createLayout10()
{
    createLayout9();
}

void init()
{
    for (int i = 0; i < 101; i++)
    {
        obj[i].createSphereOBJ(1, 20, 20);
        obj[i].load(vPosition, vColor, vNormal);
        obj[i].setDrawBorder(GL_FALSE);
        obj[i].setColor(1, 0, 0);

        modelMatrices[i] = Identity();

        Mats[i] = redPlastic;
    }

    for (int i = 0; i < 10; i++)
    {
        Lts[i] = defaultLight;
        Lts[i].position = vec4(2, 3, 4, 1);
    }

    tPot.load(vPosition, vNormal);

    for (int i = 0; i < 15; i++){
        tPotMatrices[i] = Identity();
        tMats[i] = redPlastic;
    }

    cubeObj.load(vPosition, vColor, vNormal);

    GLfloat x[11];
    GLfloat z[11];

    int side = 11;
    int start = side/2*(-1);

    for (int i = 0; i < side; i++)
    {
        GLfloat p = GLfloat((start+i)*2);
        x[i] = p;
        z[i] = p;
    }

    for (int i = 0; i < side; i++)
    {
        for(int j = 0; j < side; j++)
        cubeMatrices[i*side+j] = Translate(x[i], 0.5, z[j]);
    }

    sphcamera.setPosition(10.0, 30, 30);

    yprcamera.setPosition(10, 10, 10);
    yprcamera.setView(-5, -5, -5);
    yprcamera.setUpViaY();

    coords.load(vPosition, vColor);
    glEnable(GL_DEPTH_TEST);

    lightobj.createSphereOBJ(0.2, 7, 7);
    lightobj.load(vPosition, vColor, vNormal);
    lightobj.setColor(1, 1, 0);
    lightobj.setDrawBorder(GL_TRUE);

//  Load the shaders

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "vertexShader.vert"},
        {GL_FRAGMENT_SHADER, "Phong.frag"},
        {GL_NONE, NULL}
    };

    program = LoadShaders(shaders);
    glUseProgram(program);

    GlobalAmbient = vec4(0.2, 0.2, 0.2, 1);
    glUniform4fv(glGetUniformLocation(program, "GlobalAmbient"), 1, GlobalAmbient);
    createLayout1();
}

void reshape(GLint width, GLint height)
{
    winWidth = width;
    winHeight = height;

    glViewport(0, 0, width, height);
    projection = Perspective(60.0f, (float)width/(float)height, 0.0001f, 100.0f);
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
        createLayout1();
        drawAxes = GL_TRUE;
        drawTeaPot = GL_FALSE;
        drawCube   = GL_FALSE;
        drawLight  = GL_TRUE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_FALSE;
        enableFog    = GL_FALSE;
        camera2donly = GL_FALSE;
        break;

    case GLUT_KEY_F2:
        createLayout2();
        drawAxes = GL_TRUE;
        drawTeaPot = GL_FALSE;
        drawCube   = GL_FALSE;
        drawLight  = GL_TRUE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_FALSE;
        enableFog    = GL_FALSE;
        camera2donly = GL_FALSE;
        break;

    case GLUT_KEY_F3:
        createLayout3();
        drawAxes = GL_TRUE;
        drawTeaPot = GL_FALSE;
        drawCube   = GL_FALSE;
        drawLight  = GL_TRUE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_FALSE;
        enableFog    = GL_FALSE;
        camera2donly = GL_FALSE;
        break;

    case GLUT_KEY_F4:
        createLayout4();
        drawAxes = GL_TRUE;
        drawTeaPot = GL_FALSE;
        drawCube   = GL_FALSE;
        drawLight  = GL_TRUE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_FALSE;
        enableFog    = GL_FALSE;
        camera2donly = GL_FALSE;
        break;

    case GLUT_KEY_F5:
        createLayout5();
        drawAxes = GL_TRUE;
        drawTeaPot = GL_FALSE;
        drawCube   = GL_FALSE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_FALSE;
        enableFog    = GL_FALSE;
        camera2donly = GL_FALSE;
        break;

    case GLUT_KEY_F6:
        createLayout6();
        drawAxes = GL_FALSE;
        drawTeaPot = GL_TRUE;
        drawCube   = GL_FALSE;
        updateTeaPot = GL_TRUE;
        updateTree   = GL_FALSE;
        enableFog    = GL_FALSE;
        camera2donly = GL_FALSE;
        break;

    case GLUT_KEY_F7:
        createLayout7();
        drawAxes = GL_FALSE;
        drawTeaPot = GL_FALSE;
        drawCube   = GL_FALSE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_TRUE;
        enableFog    = GL_FALSE;
        camera2donly = GL_FALSE;
        break;

    case GLUT_KEY_F8:
        createLayout8();
        drawAxes = GL_FALSE;
        drawTeaPot = GL_TRUE;
        drawCube   = GL_FALSE;
        drawLight  = GL_TRUE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_FALSE;
        enableFog    = GL_FALSE;
        camera2donly = GL_FALSE;
        break;

    case GLUT_KEY_F9:
        createLayout9();
        drawAxes = GL_FALSE;
        drawTeaPot = GL_FALSE;
        drawCube   = GL_TRUE;
        drawLight  = GL_FALSE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_FALSE;
        enableFog    = GL_FALSE;
        camera2donly = GL_TRUE;
        break;

    case GLUT_KEY_F10:
        createLayout10();
        drawAxes = GL_FALSE;
        drawTeaPot = GL_FALSE;
        drawCube   = GL_TRUE;
        drawLight  = GL_FALSE;
        updateTeaPot = GL_FALSE;
        updateTree   = GL_FALSE;
        enableFog    = GL_TRUE;
        camera2donly = GL_TRUE;
        break;

    case GLUT_KEY_F11:
        drawAxes = GL_TRUE;
        break;

    case GLUT_KEY_F12:
        drawAxes = GL_FALSE;
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
    else if (CameraNumber == 3)
    {
        if (ctrlDown)
        {
            switch (key)
            {
            case GLUT_KEY_LEFT:
                ypr2dcamera.moveRight(-0.2);
                break;

            case GLUT_KEY_RIGHT:
                ypr2dcamera.moveRight(0.2);
                break;
            }
        }
        else{
            switch (key)
            {
            case GLUT_KEY_DOWN:
                ypr2dcamera.moveForward(-0.2);
                break;
            case GLUT_KEY_UP:
                ypr2dcamera.moveForward(0.2);
                break;
            case GLUT_KEY_LEFT:
                ypr2dcamera.addYaw(1);
                break;
            case GLUT_KEY_RIGHT:
                ypr2dcamera.addYaw(-1);
                break;
            }
        }

        Lts[0].position = vec4(ypr2dcamera.getPosition() -
                               0.25 * ypr2dcamera.getRightVector() +
                               0.125 * ypr2dcamera.getView(), 1);
        Lts[1].position = vec4(ypr2dcamera.getPosition() +
                               0.25 * ypr2dcamera.getRightVector() +
                               0.125 * ypr2dcamera.getView(), 1);

        for(int i = 0; i < numLights; i++)
            Lts[i].spotDirection = ypr2dcamera.getView();

        LoadLights(Lts, numLights);
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

    case '1':
        CameraNumber = 1;
        break;

    case '2':
        CameraNumber = 2;
        break;
    /* for testing
    case '3':
        CameraNumber = 3;
        break;
    */
    case 'f':
        if(enableFog)
            fogon = !fogon;
        break;
    case 'd':
        if(enableFog)
        if(maxdistance < 20)
            maxdistance++;
        break;
    case 's':
        if(enableFog)
        if(maxdistance > 1)
            maxdistance--;
        break;
    }

    if(camera2donly)
        CameraNumber = 3;

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
    else if (CameraNumber == 3)
        view = ypr2dcamera.lookAt();

    mat4 axesscale = Scale(20);
    glUniformMatrix4fv(glGetUniformLocation(program, "ProjModelView"),
                       1, GL_TRUE, projection*view*axesscale);

    TurnLightsOff(Lts, 10);
    if (drawAxes)
        coords.draw();
    TurnLightsOn(Lts, 10);

    if (CameraNumber == 1)
        glUniform3fv(glGetUniformLocation(program, "eye"), 1, sphcamera.getPosition());
    else if (CameraNumber == 2)
        glUniform3fv(glGetUniformLocation(program, "eye"), 1, yprcamera.getPosition());
    else if (CameraNumber == 3)
        glUniform3fv(glGetUniformLocation(program, "eye"), 1, ypr2dcamera.getPosition());

    if(enableFog)
    {
        glUniform1i(glGetUniformLocation(program, "fogon"), fogon);
        glUniform1f(glGetUniformLocation(program, "maxdistance"), maxdistance);
    }
    else
    {
        fogon = false;
        glUniform1f(glGetUniformLocation(program, "fogon"), fogon);
    }

    for (int i = 0; i < numObjects; i++)
    {
        LoadMaterial(Mats[i]);

        glUniformMatrix4fv(glGetUniformLocation(program, "ProjModelView"),
                           1, GL_TRUE, projection*view*modelMatrices[i]);

        glUniformMatrix4fv(glGetUniformLocation(program, "ModelView"),
                           1, GL_TRUE, modelMatrices[i]);

        //  M^-1 = 1/det(M) * adj(M)
        //  (M^-1)^T = 1/det(M) * adj(M)^T
        //  GLSL uses column major so sending adj(M) to the shader loads adj(M)^T into the shader.
        mat3 normal = Adjoint(modelMatrices[i]);
        glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"),
                           1, GL_FALSE, normal);  //  Note the False for transposing

        obj[i].draw();
    }

    if(drawTeaPot)
    {
        for (int i = 0; i < numTeaPots; i++)
        {
            LoadMaterial(tMats[i]);

            glUniformMatrix4fv(glGetUniformLocation(program, "ProjModelView"),
            1, GL_TRUE, projection*view*tPotMatrices[i]);

            glUniformMatrix4fv(glGetUniformLocation(program, "ModelView"),
            1, GL_TRUE, tPotMatrices[i]);

            //  M^-1 = 1/det(M) * adj(M)
            //  (M^-1)^T = 1/det(M) * adj(M)^T
            //  GLSL uses column major so sending adj(M) to the shader loads adj(M)^T into the shader.
            mat3 normal = Adjoint(tPotMatrices[i]);
            glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"),
            1, GL_FALSE, normal);  //  Note the False for transposing

            tPot.draw();
        }
    }

    if(drawCube)
    {
        for (int i = 0; i < numCubes; i++)
        {
            LoadMaterial(redPlastic);

            glUniformMatrix4fv(glGetUniformLocation(program, "ProjModelView"),
            1, GL_TRUE, projection*view*cubeMatrices[i]);

            glUniformMatrix4fv(glGetUniformLocation(program, "ModelView"),
            1, GL_TRUE, cubeMatrices[i]);

            //  M^-1 = 1/det(M) * adj(M)
            //  (M^-1)^T = 1/det(M) * adj(M)^T
            //  GLSL uses column major so sending adj(M) to the shader loads adj(M)^T into the shader.
            mat3 normal = Adjoint(cubeMatrices[i]);
            glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"),
            1, GL_FALSE, normal);  //  Note the False for transposing

            cubeObj.draw();
        }
    }

    TurnLightsOff(Lts, 10);

    if(drawLight)
    {
        for (int i = 0; i < numLights; i++)
        {
            glUniformMatrix4fv(glGetUniformLocation(program, "ProjModelView"),
                               1, GL_TRUE, projection*view*Translate(Lts[i].position));
            lightobj.draw();
        }
    }
    TurnLightsOn(Lts, 10);

    glutSwapBuffers();
    printOpenGLErrors();
}


void update()
{
    timer = clock();
    currSec = (double)(timer - startTime)/CLOCKS_PER_SEC;
    diffSec = currSec - prevSec;
    prevSec = currSec;
    sum += diffSec;

    if(updateTree)
    {
        for (int i = 0; i < 100; i++)
            slightTimers[i] += diffSec;
    }


    if(1.0/sum <= TimesPerSecond)
    {
        sum = 0;

        if(updateTeaPot)
        {
            for(int i = 0; i < 5; i++)
            {
                tPotAng[i] += tPotSpeed[i];
                tPotRotateAng[i] += tPotRotateSpeed[i];
                if(tPotAng[i] < -360) tPotAng[i] += 360;
                if(tPotAng[i] > 360)  tPotAng[i] -= 360;
                if(tPotRotateAng[i] < -360) tPotRotateAng[i] += 360;
                if(tPotRotateAng[i] < -360) tPotRotateAng[i] -= 360;
                tPotMatrices[i] = Translate(cos(tPotAng[i])*r[i], 1, sin(tPotAng[i])*r[i]) *
                Scale(1, 1, 1) * Rotate(tPotRotateAng[i], tPotAxis[i]);
            }
        }

        if(updateTree)
        {
            for (int i = 0; i < 100; i++)
            {
                if(slightTimers[i] >= twinkleTimes[i])
                {
                    slightTimers[i] -= twinkleTimes[i];
                    if(Mats[i].emission[0] == 0)
                        Mats[i].emission[0] = 1;
                    else
                        Mats[i].emission[0] = 0;
                }
            }
        }


        glutPostRedisplay();
    }
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
    glutIdleFunc(update);

    glutMainLoop();
}
