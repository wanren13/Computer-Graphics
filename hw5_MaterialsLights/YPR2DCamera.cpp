#include "YPR2DCamera.h"

YPR2DCamera::YPR2DCamera()
{
    YPR2DCamera(1);
}

YPR2DCamera::YPR2DCamera(float Y)
{
    fixedy = Y;
    Position.x = 0;
    Position.y = fixedy;
    Position.z = 0;
    View.x = 0;
    View.y = 0;
    View.z = 0;
    UpVector.x = 0;
    UpVector.y = 1;
    UpVector.z = 0;
}


void YPR2DCamera::PositionCamera(float X, float Z, float vX, float vZ)
{
    Position.x = X;
    Position.y = fixedy;
    Position.z = Z;
    View.x = vX;
    View.y = 0;
    View.z = vZ;
    View = normalize(View);

}

void YPR2DCamera::setPosition(float X, float Z)
{
    Position.x = X;
    Position.y = fixedy;
    Position.z = Z;
}

void YPR2DCamera::setView(float X, float Z)
{
    View.x = X;
    View.y = 0;
    View.z = Z;
    View = normalize(View);
}

vec3 YPR2DCamera::getPosition()
{
    return Position;
}

vec3 YPR2DCamera::getView()
{
    return View;
}

vec3 YPR2DCamera::getUpVector()
{
    return UpVector;
}

vec3 YPR2DCamera::getRightVector()
{
    return normalize(cross(View, UpVector));
}

mat4 YPR2DCamera::lookAt()
{
    return LookAt(Position.x, Position.y, Position.z,
                  Position.x + View.x, Position.y + View.y, Position.z + View.z,
                  UpVector.x, UpVector.y, UpVector.z);
}

void YPR2DCamera::addYaw(float num)
{
    mat3 rot = Rotate3(num, UpVector);
    View = rot*View;

    View = normalize(View);
}

void YPR2DCamera::moveForward(float num)
{
    float mod = length(View);
    if (mod < 0.000001f) return;

    Position += View/mod*num;
}

void YPR2DCamera::moveRight(float num)
{
    vec3 mn = cross(View, UpVector);

    float mod = length(mn);
    if (mod < 0.000001f) return;

    Position += mn/mod*num;
}
