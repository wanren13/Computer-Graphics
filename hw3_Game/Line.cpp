#include "Line.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

Line::Line()
{
    GLushort indices[] =
    {
        0, 1
    };

    GLfloat  vertices[] =
    {
        -1.5, 0.0, 0.0, 1.0,
         1.5, 0.0, 0.0, 1.0
    };

    GLfloat  colors[] =
    {
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0
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

Line::~Line()
{

}

void Line::draw()
{
    glLineWidth(5.0);
    glBindVertexArray(vboptr);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, NULL);
}

void Line::setT(const mat4& t)
{
    transformation = t;
}

void Line::setT(GLfloat x, GLfloat y, GLfloat scale, GLfloat ang)
{
    transformation = Translate(x, y, 0.0) * Scale(scale) * RotateZ(ang);
}

mat4 Line::getT() const
{
    return transformation;
}
