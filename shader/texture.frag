#version 420

uniform sampler2D texture_sample;
layout (location = 0) in vec2 uv;
out vec4 fragColor;

void main() {
  fragColor = texture2D(texture_sample, uv);
}