#include <gl_core_4_2.h>
#include "gl_core_4_2.c"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <iostream>
#include <fstream>

using namespace std;


string vertex_shader_filepath = "../src/obj_loading/obj_loading.vert";
string fragment_shader_filepath = "../src/obj_loading/obj_loading.frag";
string screenshot_file = "../src/obj_loading/screenshot.png";

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

bool LoadTinyObj(string obj_filepath, vector<float> * vertices, vector<uint32_t> * indices)
{
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string error;

    bool obj_load = tinyobj::LoadObj(&attrib, &shapes, &materials, &error, obj_filepath.c_str());

    if(!obj_load)
    {
        cout<<error<<endl;
        return false;
    }

    uint32_t index_value = 0;

    for(const auto& shape : shapes) 
    {
        for(const auto& index : shape.mesh.indices) 
        {
            indices->push_back(index_value++);

            int vertex_idx = index.vertex_index;
            if(vertex_idx >= 0 && attrib.vertices.size() > (3 * vertex_idx + 2))
            {   
                vertices->push_back(attrib.vertices[3 * vertex_idx + 0]);
                vertices->push_back(attrib.vertices[3 * vertex_idx + 1]);
                vertices->push_back(attrib.vertices[3 * vertex_idx + 2]);
            }
            else
            {
                vertices->push_back(0.0f);
                vertices->push_back(0.0f);
                vertices->push_back(0.0f);
            }

            int texcoord_idx = index.texcoord_index;
            if(texcoord_idx >= 0 && attrib.texcoords.size() > (2 * texcoord_idx + 2))
            {   
                vertices->push_back(attrib.texcoords[2 * texcoord_idx + 0]);
                vertices->push_back(attrib.texcoords[2 * texcoord_idx + 1]);
            }
            else
            {
                vertices->push_back(0.0f);
                vertices->push_back(0.0f);
            }

            int normal_idx = index.normal_index;
            if(normal_idx >= 0 && attrib.normals.size() > (3 * normal_idx + 2))
            {   
                vertices->push_back(attrib.normals[3 * normal_idx + 0]);
                vertices->push_back(attrib.normals[3 * normal_idx + 1]);
                vertices->push_back(attrib.normals[3 * normal_idx + 2]);
            }
            else
            {
                vertices->push_back(0.0f);
                vertices->push_back(0.0f);
                vertices->push_back(0.0f);
            }
        
        }
    }

    
    return true;
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
        
        int screenshot = stbi_write_png(screenshot_file.c_str(), 640, 480, 4, (void *)flipped_data, 0);
        if(screenshot == 0)
            cout<<"Screenshot failed\n";
    }
}

void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * msg, const void * data)
{
    cout<<"Debug call: "<<msg<<endl;
}

int main(int argc, char ** argv) 
{
    if(argc != 2)
    {
        cout<<"Incorrect arguments. Usage: ./obj_loading path/to/obj/file\n";
        return -1;
    }

    glfwSetErrorCallback(ErrorCallback);

    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    vector<float> vertices;
    vector<uint32_t> indices;

    bool load_obj = LoadTinyObj(argv[1], &vertices, &indices);

    if(!load_obj)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    GLuint vertex_array, vertex_buffer, index_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location;

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

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.at(0), GL_STATIC_DRAW);

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices.at(0), GL_STATIC_DRAW);   

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

    int stride = sizeof(float) * vertices.size() / indices.size();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float)*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float)*5));

    while(!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glm::mat4 m, v, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        m = glm::rotate(m, (float)glfwGetTime(), glm::vec3(0, 1, 0)); //model
        v = glm::lookAt(glm::vec3(2, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //view
        p = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f); //projection
        mvp = p * v * m;

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);
        
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteVertexArrays(1, &vertex_array);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}