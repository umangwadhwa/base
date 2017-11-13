#define TINYOBJLOADER_IMPLEMENTATION
#include "mesh.h"
#include <tinyobjloader/tiny_obj_loader.h>

int Mesh::LoadTinyObj(std::string file) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string error;

  bool loadObj =
      tinyobj::LoadObj(&attrib, &shapes, &materials, &error, file.c_str());
  if (!loadObj) {
    std::cout << error << std::endl;
    return -1;
  }

  uint32_t index_value = 0;

  for (const auto &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {
      indices.push_back(index_value++);

      int vertex_idx = index.vertex_index;
      if (vertex_idx >= 0 && attrib.vertices.size() > (3 * vertex_idx + 2)) {
        vertices.push_back(attrib.vertices[3 * vertex_idx + 0]);
        vertices.push_back(attrib.vertices[3 * vertex_idx + 1]);
        vertices.push_back(attrib.vertices[3 * vertex_idx + 2]);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }

      int texcoord_idx = index.texcoord_index;
      if (texcoord_idx >= 0 &&
          attrib.texcoords.size() > (2 * texcoord_idx + 2)) {
        vertices.push_back(attrib.texcoords[2 * texcoord_idx + 0]);
        vertices.push_back(attrib.texcoords[2 * texcoord_idx + 1]);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }

      int normal_idx = index.normal_index;
      if (normal_idx >= 0 && attrib.normals.size() > (3 * normal_idx + 2)) {
        vertices.push_back(attrib.normals[3 * normal_idx + 0]);
        vertices.push_back(attrib.normals[3 * normal_idx + 1]);
        vertices.push_back(attrib.normals[3 * normal_idx + 2]);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }
    }
  }

  return indices.size();
}

void Mesh::LoadVertexArray(GLuint &vertex_array, GLuint &vertex_buffer,
                           GLuint &index_buffer) {
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  int stride = sizeof(float) * vertices.size() / indices.size();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                        (void *)(sizeof(float) * 3));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
                        (void *)(sizeof(float) * 5));

  glBindVertexArray(0);
}
