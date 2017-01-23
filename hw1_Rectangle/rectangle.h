#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include <GL/glew.h>
#include <GL/freeglut.h>

class rectangle
{
public:
    rectangle();
    rectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
    rectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b);
    ~rectangle();

    void setUpperLeft(GLfloat x, GLfloat y);
    void setWidth(GLfloat w);
    void setHeight(GLfloat h);
    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setAll(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b);

    GLfloat getUpperLeftX();
    GLfloat getUpperLeftY();
    GLfloat getWidth();
    GLfloat getHeight();

    void load();
    void reloadData();
    void draw();

private:
    GLfloat ulx;         // upper left x
    GLfloat uly;         // upper left y
    GLfloat width;
    GLfloat height;
    GLfloat red;
    GLfloat green;
    GLfloat blue;

    GLuint vboptr;
    GLuint bufptr;

    GLfloat vertices[16];
    GLfloat colors[12];

    void setVertexLocations();
    void setVertexColors();
};

#endif // RECTANGLE_H_INCLUDED
