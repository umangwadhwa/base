uniform mat4 mvp;
attribute vec3 vertex_color;
attribute vec3 vertex_position;
varying vec3 color;

void main()
{
    gl_Position = mvp * vec4(vertex_position, 1.0);
    color = vertex_color;
}