#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "vec_mat.h"

class rectangle
{
public:
    rectangle();
    rectangle(GLfloat w, GLfloat h);
    rectangle(GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b);
    ~rectangle();

    void setWidth(GLfloat w);
    void setHeight(GLfloat h);
    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setAll(GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b);
    void setPipelinePositions(GLint p, GLint c);
    void setRotation(GLfloat r);
    void setScale(GLfloat s);
    void setTranslate(GLfloat x, GLfloat y);

    void setRotationSpeed(GLfloat r);
    void setScaleSpeed(GLfloat s);
    void setTranslateSpeed(GLfloat x, GLfloat y);

    GLfloat getWidth();
    GLfloat getHeight();
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
    GLfloat width;
    GLfloat height;
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
    GLuint bufptr;

    GLint vPosition;
    GLint vColor;

    GLfloat vertices[16];
    GLfloat colors[12];

    static const GLfloat MAX_SCALE = 2.5;
    static const GLfloat MIN_SCALE = 0.2;

    void init();

    void setVertexLocations();
    void setVertexColors();
};

#endif // RECTANGLE_H_INCLUDED
