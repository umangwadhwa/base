#version 330
#extension GL_ARB_separate_shader_objects : require

layout (location = 0) in vec3 color;

void main()
{
    gl_FragColor = vec4(color, 1.0);
}