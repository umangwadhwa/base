#version 330
#extension GL_ARB_separate_shader_objects : require

uniform mat4 mvp;
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

void main()
{
    gl_Position =  mvp * vec4(vertex, 1.0);
}