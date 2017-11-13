#include <GLFW/glfw3.h>
#include "gl_core_4_2.c"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include "camera.h"
#include "mesh.h"
#include "shader.h"

using namespace std;

Camera camera;
float previous_xpos = 400.0f;
float previous_ypos = 300.0f;
float delta_time = 0.0f;
float previous_time = 0.0f;

string vertex_shader_filepath = "../shader/solid.vert";
string fragment_shader_filepath = "../shader/solid.frag";
string screenshot_file = "../working/obj_loading/screenshot.png";

void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  // screenshot
  if (key == GLFW_KEY_C && action == GLFW_PRESS) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    GLubyte *data = new GLubyte[width * height * 4];
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // flip vertically (better way to do this?)
    GLubyte *flipped_data = new GLubyte[width * height * 4];
    for (int i = 0; i < height; i++)
      memcpy(&flipped_data[width * i * 4], &data[width * (height - i - 1) * 4],
             width * 4 * sizeof(GLubyte));

    int screenshot = stbi_write_png(screenshot_file.c_str(), 640, 480, 4,
                                    (void *)flipped_data, 0);
    if (screenshot == 0) cout << "Screenshot failed\n";
  }

  float camera_speed = 2.5 * delta_time;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.MoveForward(camera_speed);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.MoveForward(-camera_speed);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.MoveRight(-camera_speed);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.MoveRight(+camera_speed);
}

void CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  float x_offset = xpos - previous_xpos;
  float y_offset = previous_ypos - ypos;
  previous_xpos = xpos;
  previous_ypos = ypos;

  float sensitivity = 0.1f;
  x_offset *= sensitivity;
  y_offset *= sensitivity;

  camera.Pan(x_offset, y_offset);
}

void ScrollCallback(GLFWwindow *window, double xpos, double ypos) {
  camera.Zoom(ypos);
}

void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar *msg, const void *data) {
  cout << "Debug call: " << msg << endl;
}

void ErrorCallback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Invalid arguments.\n";
    return -1;
  }

  glfwSetErrorCallback(ErrorCallback);

  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  GLFWwindow *window = glfwCreateWindow(640, 480, "mission", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwSetKeyCallback(window, KeyCallback);
  glfwSetScrollCallback(window, ScrollCallback);
  glfwSetCursorPosCallback(window, CursorPosCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  glDebugMessageCallback(GlDebugCallback, NULL);
  glEnable(GL_DEPTH_TEST);

  Shader shader(vertex_shader_filepath, fragment_shader_filepath);

  GLuint program;
  GLint uniform_mvp;

  if (!shader.LoadProgram(program)) {
    cout << "Could not load shaders\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  GLuint vertex_array, vertex_buffer, index_buffer;

  Mesh mesh;
  int num_elements = mesh.LoadTinyObj(argv[1]);
  mesh.LoadVertexArray(vertex_array, vertex_buffer, index_buffer);

  while (!glfwWindowShouldClose(window)) {
    float current_time = glfwGetTime();
    delta_time = current_time - previous_time;
    previous_time = current_time;

    float ratio;
    int width, height;
    glm::mat4 model, view, projection, mvp;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = (float)width / (float)height;
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program);
    uniform_mvp = glGetUniformLocation(program, "mvp");

    model = glm::mat4(1.0);
    view = camera.GetViewMatrix();
    projection = camera.GetProjectionMatrix(ratio);
    mvp = projection * view * model;

    glBindVertexArray(vertex_array);
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, (const GLfloat *)&mvp);
    glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, NULL);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertex_array);
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteBuffers(1, &index_buffer);
  glDeleteProgram(program);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}