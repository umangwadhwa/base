#include "shader.h"

Shader::Shader(std::string vert_shader_file, std::string frag_shader_file) {
  vert_shader_string = ReadShader(vert_shader_file);
  frag_shader_string = ReadShader(frag_shader_file);
}

std::string Shader::ReadShader(std::string filepath) {
  std::ifstream file(filepath);

  if (!file.is_open()) {
    return "";
  }

  std::string contents((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
  file.close();
  return contents;
}

bool Shader::LoadProgram(GLuint& program) {
  if (vert_shader_string.empty() || frag_shader_string.empty()) {
    return false;
  }

  const char* vert_shader_c_str = vert_shader_string.c_str();
  const char* frag_shader_c_str = frag_shader_string.c_str();

  GLuint vert_shader, frag_shader;

  vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vert_shader_c_str, nullptr);
  glCompileShader(vert_shader);

  frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &frag_shader_c_str, nullptr);
  glCompileShader(frag_shader);

  program = glCreateProgram();
  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);
  glLinkProgram(program);

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  return true;
}
