#version 420

uniform mat4 mvp;
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 0) out vec3 color;

void main() {
  gl_Position = mvp * vec4(vertex_position, 1.0);
  color = vertex_color;
}