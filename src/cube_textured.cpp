#include <gl_core_3_2.h>
#include "gl_core_3_2.c"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <tinyobj_loader_c.h>
#include <iostream>

using namespace std;

static const struct
{
    float x, y, z;
    float u, v;
} vertices[] =
{
    { -0.25, -0.25,  0.25, 0.0, 0.0 },
    {  0.25, -0.25,  0.25, 1.0, 0.0 },
    {  0.25,  0.25,  0.25, 1.0, 1.0 },

    {  0.25,  0.25,  0.25, 1.0, 1.0 },
    { -0.25,  0.25,  0.25, 0.0, 1.0 },
    { -0.25, -0.25,  0.25, 0.0, 0.0 },

    { -0.25,  0.25,  0.25, 0.0, 0.0 },
    {  0.25,  0.25,  0.25, 1.0, 0.0 },
    {  0.25,  0.25, -0.25, 1.0, 1.0 },

    {  0.25,  0.25, -0.25, 1.0, 1.0 },
    { -0.25,  0.25, -0.25, 0.0, 1.0 },
    { -0.25,  0.25,  0.25, 0.0, 0.0 },

    {  0.25, -0.25, -0.25, 0.0, 0.0 },
    { -0.25, -0.25, -0.25, 1.0, 0.0 },
    { -0.25,  0.25, -0.25, 1.0, 1.0 },

    { -0.25,  0.25, -0.25, 1.0, 1.0 },
    {  0.25,  0.25, -0.25, 0.0, 1.0 },
    {  0.25, -0.25, -0.25, 0.0, 0.0 },

    { -0.25, -0.25, -0.25, 0.0, 0.0 },
    {  0.25, -0.25, -0.25, 1.0, 0.0 },
    {  0.25, -0.25,  0.25, 1.0, 1.0 },

    {  0.25, -0.25,  0.25, 1.0, 1.0 },
    { -0.25, -0.25,  0.25, 0.0, 1.0 },
    { -0.25, -0.25, -0.25, 0.0, 0.0 },

    { -0.25, -0.25, -0.25, 0.0, 0.0 },
    { -0.25, -0.25,  0.25, 1.0, 0.0 },
    { -0.25,  0.25,  0.25, 1.0, 1.0 },

    { -0.25,  0.25,  0.25, 1.0, 1.0 },
    { -0.25,  0.25, -0.25, 0.0, 1.0 },
    { -0.25, -0.25, -0.25, 0.0, 0.0 },

    {  0.25, -0.25,  0.25, 0.0, 0.0 },
    {  0.25, -0.25, -0.25, 1.0, 0.0 },
    {  0.25,  0.25, -0.25, 1.0, 1.0 },

    {  0.25,  0.25, -0.25, 1.0, 1.0 },
    {  0.25,  0.25,  0.25, 0.0, 1.0 },
    {  0.25, -0.25,  0.25, 0.0, 0.0 },
};


static const char * vertex_texture_shader_text =
"uniform mat4 mvp;\n"
"attribute vec3 vertex_position;\n"
"attribute vec2 vertex_uv;\n"
"varying vec2 uv;\n"
"void main()\n"
"{\n"
"    gl_Position =  mvp * vec4(vertex_position, 1.0);\n"
"    uv = vertex_uv;\n"
"}\n";

static const char * fragment_texture_shader_text = 
"varying vec2 uv;\n"
"uniform sampler2D texture_sample;\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture2D(texture_sample, uv);\n"
"}\n";


void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * msg, const void * data)
{
    cout<<"Debug call: "<<msg<<endl;
}


int main() 
{
    glfwSetErrorCallback(ErrorCallback);

    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(640, 480, "mission", NULL, NULL);
    if(!window) 
    {
        glfwTerminate();
        return -1;
    }
    
    glfwSetKeyCallback(window, KeyCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
    {
        glfwDestroyWindow(window);
        return -1;
    }

    /*
    if(ogl_ext_KHR_debug)
        cout<<"KHR_debug supported"<<endl;
    */

    glDebugMessageCallback(GlDebugCallback, NULL);

    glEnable(GL_DEPTH_TEST); 

    int tex_width, tex_height, tex_channels;
    unsigned char * data = stbi_load("../texture/wall.jpg", &tex_width, &tex_height, &tex_channels, 0);

    if(data == NULL)
    {
        cout<<stbi_failure_reason()<<endl;
        return -1;
    }

    cout<<tex_width<<":"<<tex_height<<":"<<tex_channels<<endl;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    if(tex_channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if(tex_channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);


    GLuint vertex_buffer, uv_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, texture_location, vertex_texture_position_location, vertex_texture_uv_location;

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_texture_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_texture_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "mvp");
    vertex_texture_position_location = glGetAttribLocation(program, "vertex_position");
    vertex_texture_uv_location = glGetAttribLocation(program, "vertex_uv");
    texture_location = glGetUniformLocation(program, "texture_sample");

    glEnableVertexAttribArray(vertex_texture_position_location);
    glVertexAttribPointer(vertex_texture_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void *)0);
    glEnableVertexAttribArray(vertex_texture_uv_location);
    glVertexAttribPointer(vertex_texture_uv_location, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void *)(sizeof(float)*3));


    while(!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glm::mat4 m, v, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        m = glm::rotate(m, (float)glfwGetTime(), glm::vec3(0, 1, 0)); //model
        v = glm::lookAt(glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0,1,0)); //view
        p = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f); //projection
        mvp = p * v * m;

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
    cout<<"Compiled against GLFW "<<GLFW_VERSION_MAJOR<<"."<<GLFW_VERSION_MINOR<<"."<<GLFW_VERSION_REVISION<<endl;
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    cout<<"Running against GLFW "<<major<<"."<<minor<<"."<<revision<<endl;
    */

    stbi_image_free(data);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}