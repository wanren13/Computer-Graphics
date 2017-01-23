#ifndef YPR2DCAMERA_H_INCLUDED
#define YPR2DCAMERA_H_INCLUDED

#include <GL/gl.h>
#include "vec_mat.h"

class YPR2DCamera
{
public:
    YPR2DCamera();
    YPR2DCamera(float Y);
    void PositionCamera(float X, float Z, float vX, float vZ);
    void setPosition(float X, float Z);
    void setView(float X, float Z);
    vec3 getPosition();
    vec3 getView();
    vec3 getUpVector();
    vec3 getRightVector();
    mat4 lookAt();
    void addYaw(float num);
    void moveForward(float num);
    void moveRight(float num);

private:
    float fixedy;
    vec3 Position;
    vec3 View;
    vec3 UpVector;
};

#endif // YPR2DCAMERA_H_INCLUDED
