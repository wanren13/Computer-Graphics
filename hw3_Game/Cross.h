#ifndef CROSS_H_INCLUDED
#define CROSS_H_INCLUDED

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "vec_mat.h"

class Cross
{
public:
    Cross();
    ~Cross();

    void setT(const mat4& t);
    void setT(GLfloat x, GLfloat y, GLfloat scale, GLfloat ang);
    mat4 getT() const;

    void draw();
private:
    GLuint vboptr;
    GLuint bufptr;

    mat4 transformation;
};

#endif // CROSS_H_INCLUDED
