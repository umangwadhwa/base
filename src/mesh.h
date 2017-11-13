#include <gl_core_4_2.h>
#include <iostream>
#include <vector>

class Mesh {
  std::vector<float> vertices;
  std::vector<uint32_t> indices;

 public:
  int LoadTinyObj(std::string);
  void LoadVertexArray(GLuint&, GLuint&, GLuint&);
};
