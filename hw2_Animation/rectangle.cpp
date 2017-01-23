#include <iostream>
#include "rectangle.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

/***********************
   Index of vertices

    0 ----------- 3
      | \       |
      |   \     |
      |     \   |
      |       \ |
    1 ----------- 2

**********************/

GLfloat rectangle::MAX_TRANSLATEX = 10.0;
GLfloat rectangle::MIN_TRANSLATEX = -10.0;
GLfloat rectangle::MAX_TRANSLATEY = 10.0;
GLfloat rectangle::MIN_TRANSLATEY = -10.0;

GLushort vertex_indices[] =
{
    0, 1, 2,
    2, 3, 0
};

rectangle::rectangle():
width(1.0), height(1.0), red(1.0), green(1.0), blue(1.0)
{
    init();
    load();
}


rectangle::rectangle(GLfloat w, GLfloat h):
width(w), height(h), red(1.0), green(1.0), blue(1.0)
{
    init();
    load();
}

rectangle::rectangle(GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b):
width(w), height(h), red(r), green(g), blue(b)
{
    init();
    load();
}

rectangle::~rectangle()
{
    glDeleteVertexArrays(1, &vboptr);
    glDeleteBuffers(1, &bufptr);
}


inline void rectangle::init()
{
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


void rectangle::load()
{
    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

    glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors),
                 NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
                    vertices);

    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors),
                    colors);

    setPipelinePositions(0, 1);

    glVertexAttribPointer(vPosition, 4, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));

    glVertexAttribPointer(vColor, 3, GL_FLOAT,
                          GL_TRUE, 0, BUFFER_OFFSET(sizeof(vertices)));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

void rectangle::reloadData()
{
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors),
                NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors),
                colors);
}

void rectangle::draw()
{
    glBindVertexArray(vboptr);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

void rectangle::updateTransformations()
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

mat4 rectangle::getTransformationMatrix()
{
    mat4 rotatemat = RotateZ(rotation);
    mat4 scalemat  = Scale(scale);
    mat4 transmat  = Translate(translateX, translateY, 0);

    return transmat * scalemat * rotatemat;
}

void rectangle::setVertexLocations()
{
    GLfloat w = width / 2;
    GLfloat h = height / 2;

    vertices[0] = -1 * w;             // vertex 0
    vertices[1] = h;

    vertices[4] = -1 * w;             // vertex 1
    vertices[5] = -1 * h;

    vertices[8] = w;                  // vertex 2
    vertices[9] = -1 * h;

    vertices[12] = w;                 // vertex 3
    vertices[13] = h;

    // set d and w
    for (int i=0; i<4; i++)
    {
        vertices[i*4+2] = 0;
        vertices[i*4+3] = 1;
    }
}

void rectangle::setVertexColors()
{
    for (int i=0; i<4; i++)
    {
        colors[i*3] = red;
        colors[i*3+1] = green;
        colors[i*3+2] = blue;
    }
}


//////////////////
// Setters      //
//////////////////

// Sets the width of the rectangle
void rectangle::setWidth(GLfloat w)
{
    width = w;
    setVertexLocations();
    reloadData();
}

// Sets the height of the rectangle,
void rectangle::setHeight(GLfloat h)
{
    height = h;
    setVertexLocations();
    reloadData();
}

// Sets the color of the rectangle
void rectangle::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
    setVertexColors();
    reloadData();
}

// Sets all of the rectangle attributes
void rectangle::setAll(GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b)
{
    width = w;
    height = h;
    red = r;
    green = g;
    blue = b;
    setVertexLocations();
    setVertexColors();
    reloadData();
}

void rectangle::setPipelinePositions(GLint p, GLint c)
{
    vPosition = p;
    vColor = c;
}


void rectangle::setRotation(GLfloat r)
{
    rotation = r;
}


void rectangle::setScale(GLfloat s)
{
    scale = s;
}


void rectangle::setTranslate(GLfloat x, GLfloat y)
{
    translateX = x;
    translateY = y;
}

void rectangle::setRotationSpeed(GLfloat r)
{
    rotationSpeed = r;
}

void rectangle::setScaleSpeed(GLfloat s)
{
    scaleSpeed = s;
}

void rectangle::setTranslateSpeed(GLfloat x, GLfloat y)
{
    translateXSpeed = x;
    translateYSpeed = y;
}


//////////////////
// Getters      //
//////////////////

GLfloat rectangle::getWidth()
{
    return width;
}

GLfloat rectangle::getHeight()
{
    return height;
}

GLfloat rectangle::getRotation()
{
    return rotation;
}

GLfloat rectangle::getScale()
{
    return scale;
}

GLfloat rectangle::getTranslateX()
{
    return translateX;
}

GLfloat rectangle::getTranslateY()
{
    return translateY;
}

GLfloat rectangle::getRotationSpeed()
{
    return rotationSpeed;
}

GLfloat rectangle::getScaleSpeed()
{
    return scaleSpeed;
}

GLfloat rectangle::getTranslateSpeedX()
{
    return translateXSpeed;
}

GLfloat rectangle::getTranslateSpeedY()
{
    return translateYSpeed;
}



