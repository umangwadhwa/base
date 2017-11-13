#include <gl_core_4_2.h>
#include <fstream>
#include <iostream>

class Shader {
  std::string vert_shader_string;
  std::string frag_shader_string;

  std::string ReadShader(std::string);

 public:
  Shader(std::string, std::string);
  bool LoadProgram(GLuint&);
};
