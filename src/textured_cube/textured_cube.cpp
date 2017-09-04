#include <gl_core_3_2.h>
#include "gl_core_3_2.c"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <iostream>
#include <fstream>

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

string vertex_shader_filepath = "../src/textured_cube/textured_cube.vert";
string fragment_shader_filepath = "../src/textured_cube/textured_cube.frag";


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

void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    //screenshot
    if(key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        GLubyte * data = new GLubyte[width*height*4];
        glReadBuffer(GL_FRONT);
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        //flip vertically (better way to do this?)
        GLubyte * flipped_data = new GLubyte[width*height*4];
        for(int i = 0; i < height; i++)
            memcpy(&flipped_data[width*i*4], &data[width*(height-i-1)*4], width*4*sizeof(GLubyte));
        
        int screenshot = stbi_write_png("../src/textured_cube/screenshot.png", 640, 480, 4, (void *)flipped_data, 0);
        if(screenshot == 0)
            cout<<"Screenshot failed\n";
    }
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
    
    glfwSetKeyCallback(window, KeyCallback);

    glfwMakeContextCurrent(window);
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

    int tex_width, tex_height, tex_channels;
    unsigned char * data = stbi_load("../src/textured_cube/wall.jpg", &tex_width, &tex_height, &tex_channels, 4);

    if(data == NULL)
    {
        cout<<stbi_failure_reason()<<endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }


    GLuint vao, vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, texture_location;

    string vertex_shader_string = ReadShader(vertex_shader_filepath);
    string fragment_shader_string = ReadShader(fragment_shader_filepath);
    const char * vertex_texture_shader_text = vertex_shader_string.c_str();
    const char * fragment_texture_shader_text = fragment_shader_string.c_str();

    if(vertex_texture_shader_text == "" || fragment_texture_shader_text == "")
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    mvp_location = glGetUniformLocation(program, "mvp");
    texture_location = glGetUniformLocation(program, "texture_sample");

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *)(sizeof(float)*3));

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
        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    //cout<<"Compiled against GLFW "<<GLFW_VERSION_MAJOR<<"."<<GLFW_VERSION_MINOR<<"."<<GLFW_VERSION_REVISION<<endl;
    //int major, minor, revision;
    //glfwGetVersion(&major, &minor, &revision);
    //cout<<"Running against GLFW "<<major<<"."<<minor<<"."<<revision<<endl;

    stbi_image_free(data);

    glDeleteProgram(program);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &tex);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}