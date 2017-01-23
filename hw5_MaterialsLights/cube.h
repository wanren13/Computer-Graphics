#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include "vec_mat.h"

class cube
{
public:
    cube();
    cube(GLfloat r, GLfloat g, GLfloat b);
    ~cube();

    void setDrawBorder(GLboolean d);
    void setDrawFaces(GLboolean d);
    void setColorCube(GLboolean d);

    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setBorderColor(GLfloat r, GLfloat g, GLfloat b);

    void load(GLint v, GLint c, GLint n);
    void reloadData();
    void draw();

private:
    GLfloat red = 1;
    GLfloat green = 1;
    GLfloat blue = 1;
    GLfloat bred = 0;
    GLfloat bgreen = 0;
    GLfloat bblue = 0;

    GLuint vboptr;
    GLuint bufptr;
    GLuint vboptr2;
    GLuint bufptr2;

    GLint vPosition = 0;
    GLint vColor = 1;
    GLint vNormal = 2;

    GLboolean reload = GL_FALSE;
    GLboolean drawFaces = GL_TRUE;
    GLboolean drawBorder = GL_TRUE;
    GLboolean isColorCube = GL_TRUE;
};

#endif // CUBE_H_INCLUDED
