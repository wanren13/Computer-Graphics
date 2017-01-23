#include "Circle.h"
#include <math.h>

#define PI 3.1415926535897932385
#define numOfPoints 120
#define BUFFER_OFFSET(x) ((const void*) (x))

Circle::Circle()
{
    GLushort indices[120];

    GLfloat  vertices[480];

    GLfloat  colors[360];

    GLfloat ang = 3*PI/180;

    for(int i=0; i<numOfPoints; i++){
        indices[i] = (GLushort)i;

        vertices[i*4]   = 0.45 * cos(ang*i);
        vertices[i*4+1] = 0.45 * sin(ang*i);
        vertices[i*4+2] = 0;
        vertices[i*4+3] = 1;

        colors[i*3]   = 0.0;
        colors[i*3+1] = 1.0;
        colors[i*3+2] = 0.0;
    }


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

Circle::~Circle()
{

}

void Circle::draw()
{
    glLineWidth(8.0);
    glBindVertexArray(vboptr);
    glDrawElements(GL_LINE_LOOP, 120, GL_UNSIGNED_SHORT, NULL);
}

void Circle::setT(const mat4& t)
{
    transformation = t;
}

void Circle::setT(GLfloat x, GLfloat y, GLfloat scale, GLfloat ang)
{
    transformation = Translate(x, y, 0.0) * Scale(scale) * RotateZ(ang);
}

mat4 Circle::getT() const
{
    return transformation;
}
