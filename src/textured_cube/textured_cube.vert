#version 330
#extension GL_ARB_separate_shader_objects : require

uniform mat4 mvp;
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_uv;
layout (location = 0) out vec2 uv;

void main()
{
    gl_Position =  mvp * vec4(vertex_position, 1.0);
    uv = vertex_uv;
}