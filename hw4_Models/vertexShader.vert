#version 400 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 icolor;

uniform mat4 ProjModelView;

out vec4 color;

void main()
{
    color = icolor;
    gl_Position = ProjModelView * position;
}
