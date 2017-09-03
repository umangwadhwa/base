uniform mat4 mvp;
attribute vec3 vertex_position;
attribute vec2 vertex_uv;
varying vec2 uv;

void main()
{
    gl_Position =  mvp * vec4(vertex_position, 1.0);
    uv = vertex_uv;
}