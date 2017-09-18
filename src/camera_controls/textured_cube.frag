#version 330
#extension GL_ARB_separate_shader_objects : require

layout (location = 0) in vec2 uv;
uniform sampler2D texture_sample;

void main()
{
    gl_FragColor = texture2D(texture_sample, uv);
}