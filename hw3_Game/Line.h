#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "vec_mat.h"

class Line
{
public:
    Line();
    ~Line();

    void setT(const mat4& t);
    void setT(GLfloat x, GLfloat y, GLfloat scale, GLfloat ang);
    mat4 getT() const;

    void draw();
private:
    GLuint vboptr;
    GLuint bufptr;

    mat4 transformation;
};

#endif // LINE_H_INCLUDED
