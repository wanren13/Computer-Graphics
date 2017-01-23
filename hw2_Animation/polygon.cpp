#include <iostream>               // for debugging
#include "polygon.h"
#include <math.h>

#define BUFFER_OFFSET(x) ((const void*) (x))
#define PI 3.1415926535897932385

GLfloat polygon::MAX_TRANSLATEX = 10.0;
GLfloat polygon::MIN_TRANSLATEX = -10.0;
GLfloat polygon::MAX_TRANSLATEY = 10.0;
GLfloat polygon::MIN_TRANSLATEY = -10.0;

polygon::polygon():
sides(4), radius(1.0), red(1.0), green(1.0), blue(1.0)
{
    init();
    load();
}


polygon::polygon(GLint n, GLfloat rad):
sides(n), radius(rad), red(1.0), green(1.0), blue(1.0)
{
    init();
    load();
}


polygon::polygon(GLint n, GLfloat rad, GLfloat r, GLfloat g, GLfloat b):
sides(n), radius(rad), red(r), green(g), blue(b)
{
    init();
    load();
}


polygon::~polygon()
{
    glDeleteVertexArrays(1, &vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
    delete [] points;
    delete [] indices;
    delete [] colors;
}


inline void polygon::init()
{
    points  = new GLfloat[(sides+1)*4];
    indices = new GLushort[sides*3];
    colors  = new GLfloat[(sides+1)*3];

    setVertexIndices();
    setVertexLocations();
    setVertexColors();

    rotation        = 0.0;
    scale           = 1.0;
    translateX      = 0.0;
    translateY      = 0.0;
    rotationSpeed   = 0.0;
    scaleSpeed      = 1.0;
    translateXSpeed = 0.0;
    translateYSpeed = 0.0;
}


void polygon::load()
{
    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glGenBuffers(1, &eboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLushort)*sides*3, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(sides+1)*7,
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*(sides+1)*4,
                    points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(sides+1)*4,
                    sizeof(GLfloat)*(sides+1)*3, colors);

    setPipelinePositions(0, 1);

    glVertexAttribPointer(vPosition, 4, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vColor, 3, GL_FLOAT,
                          GL_TRUE, 0, BUFFER_OFFSET(sizeof(GLfloat)*(sides+1)*4));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

void polygon::reloadData()
{
    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLushort)*sides*3, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(sides+1)*7,
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*(sides+1)*4,
                    points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(sides+1)*4,
                    sizeof(GLfloat)*(sides+1)*3, colors);

    glVertexAttribPointer(vPosition, 4, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vColor, 3, GL_FLOAT,
                          GL_TRUE, 0, BUFFER_OFFSET(sizeof(GLfloat)*(sides+1)*4));
}


void polygon::draw()
{
    glBindVertexArray(vboptr);
    glDrawElements(GL_TRIANGLES, sides*3, GL_UNSIGNED_SHORT, NULL);
}

void polygon::updateTransformations()
{
    if(scale >= MAX_SCALE || scale <= MIN_SCALE)
        scaleSpeed = 2 - scaleSpeed;

    if(translateX >= MAX_TRANSLATEX)
        translateXSpeed = translateXSpeed<0?translateXSpeed:-1*translateXSpeed;

    if(translateX <= MIN_TRANSLATEX)
        translateXSpeed = translateXSpeed>0?translateXSpeed:-1*translateXSpeed;

    if(translateY >= MAX_TRANSLATEY)
        translateYSpeed = translateYSpeed<0?translateYSpeed:-1*translateYSpeed;

    if(translateY <= MIN_TRANSLATEY)
        translateYSpeed = translateYSpeed>0?translateYSpeed:-1*translateYSpeed;

    rotation   += rotationSpeed;
    if(rotation >= 360 || rotation <= 360)
        rotation = rotation - 360*((int)rotation/360);

    translateX += translateXSpeed;
    translateY += translateYSpeed;
    scale      *= scaleSpeed;
}

mat4 polygon::getTransformationMatrix()
{
    mat4 rotatemat = RotateZ(rotation);
    mat4 scalemat  = Scale(scale);
    mat4 transmat  = Translate(translateX, translateY, 0);

    return transmat * scalemat * rotatemat;
}

void polygon::setVertexIndices()
{
    for(int i=0; i<sides; i++)
    {
        indices[i*3] = 0;
        indices[i*3+1] = i+1;
        indices[i*3+2] = i+2;
    }

    indices[sides*3-1] = 1;
}

void polygon::setVertexLocations()
{
    // center point
    points[0] = 0.0;
    points[1] = 0.0;
    points[2] = 0.0;
    points[3] = 1.0;

    // rest of points
    GLfloat ang = 2*PI/sides;

    for(int i=1; i<sides+1; i++)
    {
        points[4*i]   = radius * cos(ang*i);
        points[4*i+1] = radius * sin(ang*i);
        points[4*i+2] = 0;
        points[4*i+3] = 1;
    }
}

void polygon::setVertexColors()
{
    for (int i=0; i<sides+1; i++)
    {
        colors[i*3] = red;
        colors[i*3+1] = green;
        colors[i*3+2] = blue;
    }
}

//////////////////
// Setters      //
//////////////////

// Sets the sides of the polygon
void polygon::setSides(GLint n)
{
    if(sides != n)
    {
        sides = n;

        delete [] points;
        delete [] indices;
        delete [] colors;

        points  = new GLfloat[(sides+1)*4];
        indices = new GLushort[sides*3];
        colors  = new GLfloat[(sides+1)*3];

        setVertexIndices();
        setVertexLocations();
        setVertexColors();
        reloadData();
    }
}

// Sets the radius of the polygon,
void polygon::setRadius(GLfloat rad)
{
    if(radius != rad)
    {
        radius = rad;

        setVertexLocations();
        reloadData();
    }
}

// Sets the color of the polygon
void polygon::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;

    setVertexColors();
    reloadData();
}

// Sets all of the polygon attributes
void polygon::setAll(GLint n, GLfloat rad, GLfloat r, GLfloat g, GLfloat b)
{
    if(sides != n)
    {
        delete [] points;
        delete [] indices;
        delete [] colors;

        sides = n;

        points  = new GLfloat[(sides+1)*4];
        indices = new GLushort[sides*3];
        colors  = new GLfloat[(sides+1)*3];
        setVertexIndices();
    }

    radius = rad;
    red = r;
    green = g;
    blue = b;

    setVertexLocations();
    setVertexColors();
    reloadData();

    bool debug;

    debug = false;
//    debug = true;

    if(debug)
    {
        std::cout << "Indices :" << std::endl;

        int c = 0;

        for(int i = 0; i < sides; i++)
            std::cout << c++ << ": (" << indices[i*3] << ", " << indices[i*3+1] << ", " << indices[i*3+2] << ")" << std::endl;

        std::cout << std::endl;

        c = 0;

        std::cout << "Points :" << std::endl;

        for(int i = 0; i < sides+1; i++)
            std::cout << c++ << ": (" << points[i*4] << ", " << points[i*4+1] << ", " << points[i*4+2] << ", " << points[i*4+3] << ")" << std::endl;

        std::cout << std::endl;

        c = 0;

        std::cout << "Colors :" << std::endl;

        for(int i = 0; i < sides+1; i++)
            std::cout << c++ << ": (" << colors[i*3] << ", " << colors[i*3+1] << ", " << colors[i*3+2] << ")" << std::endl;

        std::cout << std::endl;
    }
}

void polygon::setPipelinePositions(GLint v, GLint c)
{
    vPosition = v;
    vColor = c;
}


void polygon::setRotation(GLfloat r)
{
    rotation = r;
}


void polygon::setScale(GLfloat s)
{
    scale = s;
}


void polygon::setTranslate(GLfloat x, GLfloat y)
{
    translateX = x;
    translateY = y;
}

void polygon::setRotationSpeed(GLfloat r)
{
    rotationSpeed = r;
}

void polygon::setScaleSpeed(GLfloat s)
{
    scaleSpeed = s;
}

void polygon::setTranslateSpeed(GLfloat x, GLfloat y)
{
    translateXSpeed = x;
    translateYSpeed = y;
}


//////////////////
// Getters      //
//////////////////

GLint polygon::getSides()
{
    return sides;
}

GLfloat polygon::getRadius()
{
    return radius;
}

GLfloat polygon::getRotation()
{
    return rotation;
}

GLfloat polygon::getScale()
{
    return scale;
}

GLfloat polygon::getTranslateX()
{
    return translateX;
}

GLfloat polygon::getTranslateY()
{
    return translateY;
}

GLfloat polygon::getRotationSpeed()
{
    return rotationSpeed;
}

GLfloat polygon::getScaleSpeed()
{
    return scaleSpeed;
}

GLfloat polygon::getTranslateSpeedX()
{
    return translateXSpeed;
}

GLfloat polygon::getTranslateSpeedY()
{
    return translateYSpeed;
}



