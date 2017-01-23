#include <iostream>
#include "rectangle.h"
#include "LoadShaders.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

GLint vPosition = 0;
GLint vColor = 1;

/***********************
   Index of vertices

    0 ----------- 3
      | \       |
      |   \     |
      |     \   |
      |       \ |
    1 ----------- 2

**********************/

GLushort vertex_indices[] =
{
    0, 1, 2,
    2, 3, 0
};

rectangle::rectangle():
ulx(0.0), uly(0.0), width(1.0), height(1.0),
red(1.0), green(1.0), blue(1.0)
{
    load();
}


rectangle::rectangle
(GLfloat x, GLfloat y, GLfloat w, GLfloat h):
ulx(x), uly(y), width(w), height(h),
red(1.0), green(1.0), blue(1.0)
{
    load();
}

rectangle::rectangle
(GLfloat x, GLfloat y, GLfloat w, GLfloat h,
 GLfloat r, GLfloat g, GLfloat b):
ulx(x), uly(y), width(w), height(h),
red(r), green(g), blue(b)
{
    load();
}

rectangle::~rectangle()
{
    glDeleteVertexArrays(1, &vboptr);
    glDeleteBuffers(1, &bufptr);
}

// Sets the x and y positions of the upper left corner of the rectangle.
void rectangle::setUpperLeft(GLfloat x, GLfloat y)
{
    ulx = x;
    uly = y;
}

// Sets the width of the rectangle
void rectangle::setWidth(GLfloat w)
{
    width = w;
}

// Sets the height of the rectangle,
void rectangle::setHeight(GLfloat h)
{
    height = h;
}

// Sets the color of the rectangle
void rectangle::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
}

// Sets all of the rectangle attributes
void rectangle::setAll(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b)
{
    ulx = x;
    uly = y;
    width = w;
    height = h;
    red = r;
    green = g;
    blue = b;
}

// Returns the x coordinate of the upper left corner of the rectangle
GLfloat rectangle::getUpperLeftX()
{
    return ulx;
}

// Returns the y coordinate of the upper left corner of the rectangle
GLfloat rectangle::getUpperLeftY()
{
    return uly;
}

// Returns the width of the rectangle
GLfloat rectangle::getWidth()
{
    return width;
}

// Returns the height of the rectangle
GLfloat rectangle::getHeight()
{
    return height;
}

void rectangle::load()
{
    setVertexLocations();
    setVertexColors();

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

    glVertexAttribPointer(vPosition, 4, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));

    glVertexAttribPointer(vColor, 3, GL_FLOAT,
                          GL_TRUE, 0, BUFFER_OFFSET(sizeof(vertices)));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "PassThrough.vert"},
        {GL_FRAGMENT_SHADER, "PassThrough.frag"},
        {GL_NONE, NULL}
    };

    GLuint program = LoadShaders(shaders);
    glUseProgram(program);
}

void rectangle::reloadData()
{
    setVertexLocations();
    setVertexColors();
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors),
                NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors),
                colors);
}

void rectangle::draw()
{
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufptr);
    glBindVertexArray(vboptr);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

void rectangle::setVertexLocations()
{
    vertices[0] = ulx;             // vertex 0
    vertices[1] = uly;

    vertices[4] = ulx;             // vertex 1
    vertices[5] = uly - height;

    vertices[8] = ulx + width;     // vertex 2
    vertices[9] = uly - height;

    vertices[12] = ulx + width;    // vertex 3
    vertices[13] = uly;

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
