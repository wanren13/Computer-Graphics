#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "vec_mat.h"

class polygon
{
public:
    polygon();
    polygon(GLint n);
    polygon(GLint n, GLfloat rad);
    polygon(GLint n, GLfloat rad, GLfloat r, GLfloat g, GLfloat b);
    ~polygon();

    void setSides(GLint w);
    void setRadius(GLfloat rad);
    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setAll(GLint n, GLfloat rad, GLfloat r, GLfloat g, GLfloat b);
    void setPipelinePositions(GLint v, GLint c);

    void setRotation(GLfloat r);
    void setScale(GLfloat s);
    void setTranslate(GLfloat x, GLfloat y);

    void setRotationSpeed(GLfloat r);
    void setScaleSpeed(GLfloat s);
    void setTranslateSpeed(GLfloat x, GLfloat y);

    GLint getSides();
    GLfloat getRadius();

    GLfloat getRotation();
    GLfloat getScale();
    GLfloat getTranslateX();
    GLfloat getTranslateY();

    GLfloat getRotationSpeed();
    GLfloat getScaleSpeed();
    GLfloat getTranslateSpeedX();
    GLfloat getTranslateSpeedY();

    void updateTransformations();
    mat4 getTransformationMatrix();

    void load();
    void reloadData();
    void draw();

    static GLfloat MAX_TRANSLATEX;
    static GLfloat MIN_TRANSLATEX;
    static GLfloat MAX_TRANSLATEY;
    static GLfloat MIN_TRANSLATEY;

private:
    GLint sides;
    GLfloat radius;
    GLfloat red;
    GLfloat green;
    GLfloat blue;

    GLfloat rotation;
    GLfloat scale;
    GLfloat translateX;
    GLfloat translateY;

    GLfloat rotationSpeed;
    GLfloat scaleSpeed;
    GLfloat translateXSpeed;
    GLfloat translateYSpeed;

    GLuint vboptr;
    GLuint eboptr;
    GLuint bufptr;

    GLfloat *points;
    GLushort *indices;
    GLfloat *colors;

    GLint vPosition;
    GLint vColor;

    static const GLfloat MAX_SCALE = 2.5;
    static const GLfloat MIN_SCALE = 0.2;

    GLfloat currDisX;
    GLfloat currDisY;
    GLfloat prevDisX;
    GLfloat prevDisY;

    void init();

    void setVertexIndices();
    void setVertexLocations();
    void setVertexColors();
};

#endif // POLYGON_H_INCLUDED
