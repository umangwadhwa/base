#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
  glm::vec3 camera_pos;
  glm::vec3 camera_front;
  glm::vec3 camera_up;

  float yaw;
  float pitch;
  float roll;
  float fov;

 public:
  Camera();
  Camera(glm::vec3, glm::vec3, glm::vec3, float, float, float, float);

  void MoveForward(float);
  void MoveRight(float);
  void Pan(float, float);
  void Zoom(float);

  glm::mat4 GetProjectionMatrix(float);
  glm::mat4 GetViewMatrix();
};
