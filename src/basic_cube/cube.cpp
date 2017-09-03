#include <gl_core_3_2.h>
#include "gl_core_3_2.c"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <iostream>
#include <fstream>

using namespace std;


const struct
{
    float x, y, z;
    float r, g, b;
} vertices[] =
{
    { -0.25, -0.25,  0.25, 0.0, 0.0, 0.0 },
    {  0.25, -0.25,  0.25, 1.0, 0.0, 0.0 },
    {  0.25,  0.25,  0.25, 1.0, 1.0, 0.0 },
    { -0.25,  0.25,  0.25, 0.0, 1.0, 0.0 },
    { -0.25, -0.25, -0.25, 0.0, 1.0, 1.0 },
    {  0.25, -0.25, -0.25, 0.0, 0.0, 1.0 },
    {  0.25,  0.25, -0.25, 1.0, 0.0, 1.0 },
    { -0.25,  0.25, -0.25, 1.0, 1.0, 1.0 },
};

const GLuint indices[] =
{
    0, 1, 2,
    2, 3, 0,
    1, 5, 6,
    6, 2, 1,
    7, 6, 5,
    5, 4, 7,
    4, 0, 3,
    3, 7, 4,   
    4, 5, 1,
    1, 0, 4,
    3, 2, 6,
    6, 7, 3,
};

string vertex_shader_filepath = "../src/basic_cube/cube.vert";
string fragment_shader_filepath = "../src/basic_cube/cube.frag";


string ReadShader(string filename)
{
    ifstream file(filename);

    if(file.is_open())
    {
        string contents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        return contents;
    }
    else 
    {
        cout<<"Unable to open shader file "<<filename<<endl;
        return "";
    }
}

void ErrorCallback(int error, const char * description)
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(640, 480, "mission", NULL, NULL);
    if(!window) 
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyCallback);

    glfwSwapInterval(1);

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    if(ogl_ext_KHR_debug)
        glDebugMessageCallback(GlDebugCallback, NULL);

    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS);

    GLuint vao, vertex_buffer, cube_elements, vertex_shader, fragment_shader, program;
    GLint mvp_location, vertex_position_location, vertex_color_location;

    string vertex_shader_string = ReadShader(vertex_shader_filepath);
    string fragment_shader_string = ReadShader(fragment_shader_filepath);
    const char * vertex_shader_text = vertex_shader_string.c_str();
    const char * fragment_shader_text = fragment_shader_string.c_str();

    if(vertex_shader_text == "" || fragment_shader_text == "")
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &cube_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    mvp_location = glGetUniformLocation(program, "mvp");
    vertex_position_location = glGetAttribLocation(program, "vertex_position");
    vertex_color_location = glGetAttribLocation(program, "vertex_color");

    glEnableVertexAttribArray(vertex_position_location);
    glVertexAttribPointer(vertex_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *)0);
    glEnableVertexAttribArray(vertex_color_location);
    glVertexAttribPointer(vertex_color_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *)(sizeof(float)*3));

    while(!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glm::mat4 m, v, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        m = glm::rotate(m, (float)glfwGetTime(), glm::vec3(0, 1, 0)); //model
        v = glm::lookAt(glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //view
        p = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f); //projection
        mvp = p * v * m;

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);
        
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, NULL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    //cout<<"Compiled against GLFW "<<GLFW_VERSION_MAJOR<<"."<<GLFW_VERSION_MINOR<<"."<<GLFW_VERSION_REVISION<<endl;
    //int major, minor, revision;
    //glfwGetVersion(&major, &minor, &revision);
    //cout<<"Running against GLFW "<<major<<"."<<minor<<"."<<revision<<endl;

    glDeleteProgram(program);
    glDeleteBuffers(1, &cube_elements);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}