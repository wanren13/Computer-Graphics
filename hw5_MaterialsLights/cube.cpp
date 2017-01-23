#include <GL/glew.h>
#include "cube.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

//  Constructors
cube::cube()
{
    red = 1;
    green = 1;
    blue = 1;
}

cube::cube(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
}

//  Destructor
cube::~cube()
{
}

//  Accessor Functions

void cube::setDrawBorder(GLboolean d)
{
    drawBorder = d;
}

void cube::setDrawFaces(GLboolean d)
{
    drawFaces = d;
}

void cube::setColorCube(GLboolean d)
{
    isColorCube = d;
    reload = GL_TRUE;
    reloadData();
}

void cube::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
    reloadData();
}

void cube::setBorderColor(GLfloat r, GLfloat g, GLfloat b)
{
    bred = r;
    bgreen = g;
    bblue = b;
    reloadData();
}

void cube::reloadData()
{
    reload = GL_TRUE;
    load(vPosition, vColor, vNormal);
}

void cube::load(GLint v, GLint c, GLint n)
{
    vPosition = v;
    vColor = c;
    vNormal = n;

    GLushort indices[] = {0, 1, 2,
                          2, 3, 0,
                          6, 5, 4,
                          4, 7, 6,
                          10, 9, 8,
                          8, 11, 10,
                          12, 13, 14,
                          14, 15, 12,
                          16, 17, 18,
                          18, 19, 16,
                          22, 21, 20,
                          20, 23, 22
                         };

    GLushort border_indices[] = {0, 1, 1, 2, 2, 3, 3, 0,
                                 4, 5, 5, 6, 6, 7, 7, 4,
                                 8, 9, 9, 10, 10, 11, 11, 8,
                                 12, 13, 13, 14, 14, 15, 15, 12,
                                 16, 17, 17, 18, 18, 19, 19, 16,
                                 20, 21, 21, 22, 22, 23, 23, 20
                                };

    GLfloat points[] = {-0.5, 0.5, 0.5, 1,
                        -0.5, -0.5, 0.5, 1,
                        0.5, -0.5, 0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, 0.5, -0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,

                        -0.5, 0.5, 0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, -0.5, 0.5, 1,

                        0.5, -0.5, 0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        -0.5, 0.5, 0.5, 1,
                       };

    GLfloat normals[] = {0, 0, 1,
                         0, 0, 1,
                         0, 0, 1,
                         0, 0, 1,

                         0, 0, -1,
                         0, 0, -1,
                         0, 0, -1,
                         0, 0, -1,

                         0, 1, 0,
                         0, 1, 0,
                         0, 1, 0,
                         0, 1, 0,

                         0, -1, 0,
                         0, -1, 0,
                         0, -1, 0,
                         0, -1, 0,

                         1, 0, 0,
                         1, 0, 0,
                         1, 0, 0,
                         1, 0, 0,

                         -1, 0, 0,
                         -1, 0, 0,
                         -1, 0, 0,
                         -1, 0, 0,
                        };


    GLfloat colors[4*24];
    for (int i = 0; i < 24; i++)
    {
        colors[4*i] = red;
        colors[4*i+1] = green;
        colors[4*i+2] = blue;
        colors[4*i+3] = 1;
    }

    GLfloat color_cube_colors[] = {1, 0, 0, 1,
                                   1, 0, 0, 1,
                                   1, 0, 0, 1,
                                   1, 0, 0, 1,
                                   0, 1, 0, 1,
                                   0, 1, 0, 1,
                                   0, 1, 0, 1,
                                   0, 1, 0, 1,
                                   0, 0, 1, 1,
                                   0, 0, 1, 1,
                                   0, 0, 1, 1,
                                   0, 0, 1, 1,
                                   1, 1, 0, 1,
                                   1, 1, 0, 1,
                                   1, 1, 0, 1,
                                   1, 1, 0, 1,
                                   0, 1, 1, 1,
                                   0, 1, 1, 1,
                                   0, 1, 1, 1,
                                   0, 1, 1, 1,
                                   1, 0, 1, 1,
                                   1, 0, 1, 1,
                                   1, 0, 1, 1,
                                   1, 0, 1, 1,
                                  };

    GLfloat border_colors[4*24];
    for (int i = 0; i < 24; i++)
    {
        border_colors[4*i] = bred;
        border_colors[4*i+1] = bgreen;
        border_colors[4*i+2] = bblue;
        border_colors[4*i+3] = 1;
    }

    if (!reload)
        glGenVertexArrays(1, &vboptr);

    glBindVertexArray(vboptr);

    if (!reload)
    {
        GLuint eboptr;
        glGenBuffers(1, &eboptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    }

    if (!reload)
        glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    if (isColorCube)
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(color_cube_colors), color_cube_colors);
    else
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);

    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);

    if (!reload)
        glGenVertexArrays(1, &vboptr2);

    glBindVertexArray(vboptr2);

    if (!reload)
    {
        GLuint eboptr;
        glGenBuffers(1, &eboptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(border_indices), border_indices, GL_DYNAMIC_DRAW);
    }

    if (!reload)
        glGenBuffers(1, &bufptr2);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(border_colors) + sizeof(normals), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(border_colors), border_colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(border_colors), sizeof(normals), normals);

    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(border_colors)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);

    reload = GL_FALSE;
}

void cube::draw()
{
    if (drawFaces)
    {
        glBindVertexArray(vboptr);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
    }

    if (drawBorder)
    {
        glBindVertexArray(vboptr2);
        glLineWidth(2);
        glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, NULL);
        glLineWidth(1);
    }

}
