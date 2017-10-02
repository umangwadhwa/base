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

struct Vertices
{
    float x, y, z;
    float u, v;
};

Vertices cube_small_vertices[] =
{
    { -0.25, -0.25,  0.25, 0.0, 0.0 },
    {  0.25, -0.25,  0.25, 1.0, 0.0 },
    {  0.25,  0.25,  0.25, 1.0, 1.0 },
    { -0.25,  0.25,  0.25, 0.0, 1.0 },

    { -0.25,  0.25,  0.25, 0.0, 0.0 },
    {  0.25,  0.25,  0.25, 1.0, 0.0 },
    {  0.25,  0.25, -0.25, 1.0, 1.0 },
    { -0.25,  0.25, -0.25, 0.0, 1.0 },

    {  0.25, -0.25, -0.25, 0.0, 0.0 },
    { -0.25, -0.25, -0.25, 1.0, 0.0 },
    { -0.25,  0.25, -0.25, 1.0, 1.0 },
    {  0.25,  0.25, -0.25, 0.0, 1.0 },

    { -0.25, -0.25, -0.25, 0.0, 0.0 },
    {  0.25, -0.25, -0.25, 1.0, 0.0 },
    {  0.25, -0.25,  0.25, 1.0, 1.0 },
    { -0.25, -0.25,  0.25, 0.0, 1.0 },

    { -0.25, -0.25, -0.25, 0.0, 0.0 },
    { -0.25, -0.25,  0.25, 1.0, 0.0 },
    { -0.25,  0.25,  0.25, 1.0, 1.0 },
    { -0.25,  0.25, -0.25, 0.0, 1.0 },

    {  0.25, -0.25,  0.25, 0.0, 0.0 },
    {  0.25, -0.25, -0.25, 1.0, 0.0 },
    {  0.25,  0.25, -0.25, 1.0, 1.0 },
    {  0.25,  0.25,  0.25, 0.0, 1.0 },
};

const uint32_t cube_small_indices[] =
{
     0,  1,  2,  2,  3,  0,
     4,  5,  6,  6,  7,  4,
     8,  9, 10, 10, 11,  8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20,
};

Vertices cube_medium_vertices[] =
{
    { -0.5, -0.5,  0.5, 0.0, 0.0 },
    {  0.5, -0.5,  0.5, 1.0, 0.0 },
    {  0.5,  0.5,  0.5, 1.0, 1.0 },
    { -0.5,  0.5,  0.5, 0.0, 1.0 },

    { -0.5,  0.5,  0.5, 0.0, 0.0 },
    {  0.5,  0.5,  0.5, 1.0, 0.0 },
    {  0.5,  0.5, -0.5, 1.0, 1.0 },
    { -0.5,  0.5, -0.5, 0.0, 1.0 },

    {  0.5, -0.5, -0.5, 0.0, 0.0 },
    { -0.5, -0.5, -0.5, 1.0, 0.0 },
    { -0.5,  0.5, -0.5, 1.0, 1.0 },
    {  0.5,  0.5, -0.5, 0.0, 1.0 },

    { -0.5, -0.5, -0.5, 0.0, 0.0 },
    {  0.5, -0.5, -0.5, 1.0, 0.0 },
    {  0.5, -0.5,  0.5, 1.0, 1.0 },
    { -0.5, -0.5,  0.5, 0.0, 1.0 },

    { -0.5, -0.5, -0.5, 0.0, 0.0 },
    { -0.5, -0.5,  0.5, 1.0, 0.0 },
    { -0.5,  0.5,  0.5, 1.0, 1.0 },
    { -0.5,  0.5, -0.5, 0.0, 1.0 },

    {  0.5, -0.5,  0.5, 0.0, 0.0 },
    {  0.5, -0.5, -0.5, 1.0, 0.0 },
    {  0.5,  0.5, -0.5, 1.0, 1.0 },
    {  0.5,  0.5,  0.5, 0.0, 1.0 },
};

const uint32_t cube_medium_indices[] =
{
     0,  1,  2,  2,  3,  0,
     4,  5,  6,  6,  7,  4,
     8,  9, 10, 10, 11,  8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20,
};

Vertices cube_large_vertices[] =
{
    { -0.75, -0.75,  0.75, 0.0, 0.0 },
    {  0.75, -0.75,  0.75, 1.0, 0.0 },
    {  0.75,  0.75,  0.75, 1.0, 1.0 },
    { -0.75,  0.75,  0.75, 0.0, 1.0 },

    { -0.75,  0.75,  0.75, 0.0, 0.0 },
    {  0.75,  0.75,  0.75, 1.0, 0.0 },
    {  0.75,  0.75, -0.75, 1.0, 1.0 },
    { -0.75,  0.75, -0.75, 0.0, 1.0 },

    {  0.75, -0.75, -0.75, 0.0, 0.0 },
    { -0.75, -0.75, -0.75, 1.0, 0.0 },
    { -0.75,  0.75, -0.75, 1.0, 1.0 },
    {  0.75,  0.75, -0.75, 0.0, 1.0 },

    { -0.75, -0.75, -0.75, 0.0, 0.0 },
    {  0.75, -0.75, -0.75, 1.0, 0.0 },
    {  0.75, -0.75,  0.75, 1.0, 1.0 },
    { -0.75, -0.75,  0.75, 0.0, 1.0 },

    { -0.75, -0.75, -0.75, 0.0, 0.0 },
    { -0.75, -0.75,  0.75, 1.0, 0.0 },
    { -0.75,  0.75,  0.75, 1.0, 1.0 },
    { -0.75,  0.75, -0.75, 0.0, 1.0 },

    {  0.75, -0.75,  0.75, 0.0, 0.0 },
    {  0.75, -0.75, -0.75, 1.0, 0.0 },
    {  0.75,  0.75, -0.75, 1.0, 1.0 },
    {  0.75,  0.75,  0.75, 0.0, 1.0 },
};

const uint32_t cube_large_indices[] =
{
     0,  1,  2,  2,  3,  0,
     4,  5,  6,  6,  7,  4,
     8,  9, 10, 10, 11,  8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20,
};

glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float previous_xpos = 400.0f;
float previous_ypos = 300.0f;
float fov = 45.0f;

float delta_time = 0.0f;
float previous_time = 0.0f;

string vertex_shader_filepath = "../src/camera_controls/textured_cube.vert";
string fragment_shader_filepath = "../src/camera_controls/textured_cube.frag";
string screenshot_file = "../src/camera_controls/screenshot.png";
string texture_filepath[] = 
{
    "../src/camera_controls/wood.jpg",
    "../src/camera_controls/bricks.jpg",
    "../src/camera_controls/crate.jpg",
};

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

void LoadProgram(GLuint &program, const char * vertex_shader_text, const char * fragment_shader_text)
{
    GLuint vertex_shader, fragment_shader;
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
}

void LoadVertexArray(GLuint &vertex_array, GLuint &vertex_buffer, GLuint &index_buffer, 
    Vertices * vertices, size_t vertices_size, const uint32_t * indices, size_t indices_size)
{
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);  

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertices), (void *)(sizeof(float)*3));
    
    glBindVertexArray(0);
}

bool LoadTexture(GLuint &texture, string filename)
{
    int tex_width, tex_height, tex_channels;
    unsigned char * data = stbi_load(filename.c_str(), &tex_width, &tex_height, &tex_channels, 4);

    if(data == NULL)
    {
        cout<<stbi_failure_reason()<<endl;
        return false;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return true;
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    //screenshot
    if(key == GLFW_KEY_C && action == GLFW_PRESS)
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
        
        int screenshot = stbi_write_png(screenshot_file.c_str(), 640, 480, 4, (void *)flipped_data, 0);
        if(screenshot == 0)
            cout<<"Screenshot failed\n";
    }

    float camera_speed = 2.5 * delta_time;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += camera_speed * camera_front;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= camera_speed * camera_front;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
}

void CursorPosCallback(GLFWwindow * window, double xpos, double ypos) 
{
    float x_offset = xpos - previous_xpos;
    float y_offset = previous_ypos - ypos;
    previous_xpos = xpos;
    previous_ypos = ypos;

    float sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(front);
}

void ScrollCallback(GLFWwindow * window, double xpos, double ypos) 
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= ypos;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * msg, const void * data)
{
    cout<<"Debug call: "<<msg<<endl;
}

void ErrorCallback(int error, const char * description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char ** argv) 
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
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    GLuint program;
    GLint mvp_location;

    LoadProgram(program, vertex_texture_shader_text, fragment_texture_shader_text);
    glUseProgram(program);
    mvp_location = glGetUniformLocation(program, "mvp");

    GLuint vertex_array[3], vertex_buffer[3], index_buffer[3], texture[3];

    for(int i = 0; i < 3; i++)
        if(!LoadTexture(texture[i], texture_filepath[i]))
        {
            glfwDestroyWindow(window);
            glfwTerminate();
            return -1;
        }

    LoadVertexArray(vertex_array[0], vertex_buffer[0], index_buffer[0], 
        cube_small_vertices, sizeof(cube_small_vertices), cube_small_indices, sizeof(cube_small_indices));
    LoadVertexArray(vertex_array[1], vertex_buffer[1], index_buffer[1], 
        cube_medium_vertices, sizeof(cube_medium_vertices), cube_medium_indices, sizeof(cube_medium_indices));
    LoadVertexArray(vertex_array[2], vertex_buffer[2], index_buffer[2], 
        cube_large_vertices, sizeof(cube_large_vertices), cube_large_indices, sizeof(cube_large_indices));

    while(!glfwWindowShouldClose(window))
    {
        float current_time = glfwGetTime();
        delta_time = current_time - previous_time;
        previous_time = current_time;

        float ratio;
        int width, height;
        glm::mat4 model, view, projection, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
        projection = glm::perspective(glm::radians(fov), (float)width/(float)height, 0.1f, 100.0f);
        
        //small cube
        glBindVertexArray(vertex_array[0]);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        model = translate(glm::mat4(1.0), glm::vec3(-4.5, 0.0, 0.0));
        for(int i = 0; i < 5; i++)
        {
            model = glm::translate(model, glm::vec3(1.5, 0.0, 0.0));
            mvp = projection * view * model;
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);
            glDrawElements(GL_TRIANGLES, sizeof(cube_small_indices)/sizeof(GLuint), GL_UNSIGNED_INT, NULL);
        }
        
        //medium cube
        glBindVertexArray(vertex_array[1]);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        model = translate(glm::mat4(1.0), glm::vec3(-7.5, 2.5, 0.0));
        for(int i = 0; i < 5; i++)
        {
            model = glm::translate(model, glm::vec3(2.5, 0.0, 0.0));
            mvp = projection * view * model;
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);
            glDrawElements(GL_TRIANGLES, sizeof(cube_medium_indices)/sizeof(GLuint), GL_UNSIGNED_INT, NULL);
        }

        //large cube
        glBindVertexArray(vertex_array[2]);
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        model = translate(glm::mat4(1.0), glm::vec3(-10.5, 5.0, 0.0));
        for(int i = 0; i < 5; i++)
        {
            model = glm::translate(model, glm::vec3(3.5, 0.0, 0.0));
            mvp = projection * view * model;
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);
            glDrawElements(GL_TRIANGLES, sizeof(cube_large_indices)/sizeof(GLuint), GL_UNSIGNED_INT, NULL);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for(int i = 0; i < 3; i++)
    {
        glDeleteTextures(1, &texture[i]);
        glDeleteVertexArrays(1, &vertex_array[0]);
        glDeleteBuffers(1, &vertex_buffer[0]);
        glDeleteBuffers(1, &index_buffer[0]);
    }

    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}