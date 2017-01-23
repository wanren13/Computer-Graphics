#include "Cross.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

Cross::Cross()
{
//  Layout of the Cross
//
//     0   2
//      \ /
//       X
//      / \
//     3   1

    GLushort indices[] =
    {
        0, 1,
        2, 3
    };

    GLfloat  vertices[] =
    {
        -0.45,  0.45, 0.0, 1.0,
         0.45, -0.45, 0.0, 1.0,
         0.45,  0.45, 0.0, 1.0,
        -0.45, -0.45, 0.0, 1.0
    };

    GLfloat  colors[] =
    {
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0
    };

    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors),
                 NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
                    vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors),
                    colors);

    glVertexAttribPointer(0, 4, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT,
                          GL_TRUE, 0, BUFFER_OFFSET(sizeof(vertices)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    transformation = Translate(0.0, 0.0, 0.0) * Scale(1.0) * RotateZ(0.0);
}


Cross::~Cross()
{

}

void Cross::draw()
{
    glLineWidth(8.0);
    glBindVertexArray(vboptr);
    glDrawElements(GL_LINES, 4, GL_UNSIGNED_SHORT, NULL);
}

void Cross::setT(const mat4& t)
{
    transformation = t;
}

void Cross::setT(GLfloat x, GLfloat y, GLfloat scale, GLfloat ang)
{
    transformation = Translate(x, y, 0.0) * Scale(scale) * RotateZ(ang);
}

mat4 Cross::getT() const
{
    return transformation;
}
