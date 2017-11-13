#include "camera.h"

Camera::Camera() {
  camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
  camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
  camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
  yaw = -90.0f;
  pitch = 0.0f;
  roll = 0.0f;
  fov = 45.0f;
}

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float y, float p,
               float r, float v) {
  camera_pos = pos;
  camera_front = front;
  camera_up = up;
  yaw = y;
  pitch = p;
  roll = r;
  fov = v;
}

void Camera::MoveForward(float distance) {
  camera_pos += distance * camera_front;
}

void Camera::MoveRight(float distance) {
  camera_pos += distance * glm::normalize(glm::cross(camera_front, camera_up));
}

void Camera::Pan(float x, float y) {
  yaw += x;
  pitch += y;

  glm::vec3 front =
      glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                sin(glm::radians(pitch)),
                sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

  camera_front = glm::normalize(front);
}

void Camera::Zoom(float y) { fov -= y; }

glm::mat4 Camera::GetProjectionMatrix(float ratio) {
  return glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}
