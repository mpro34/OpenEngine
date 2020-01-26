#include "../headers/Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 start_position, glm::vec3 start_up, GLfloat start_yaw, GLfloat start_pitch, GLfloat start_move_speed, GLfloat start_turn_speed) {
  position = start_position;
  world_up = start_up;
  yaw = start_yaw;
  pitch = start_pitch;
  front = glm::vec3(0.0f, 0.0f, -1.0f);

  movement_speed = start_move_speed;
  turn_speed = start_turn_speed;

  Update();
}

void Camera::Update() {
  // Update the camera values 
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front); // Remove magnitude or set magnitude to 1

  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}

void Camera::KeyControl(bool *keys, GLfloat delta_time) {
  // Calculate velocity that the camera should move based on delta time.
  GLfloat velocity = movement_speed * delta_time;

  if (keys[GLFW_KEY_W]) {
    position += front * velocity;
  }

  if (keys[GLFW_KEY_S]) {
    position -= front * velocity;
  }

  if (keys[GLFW_KEY_A]) {
    position -= right * velocity;
  }

  if (keys[GLFW_KEY_D]) {
    position += right * velocity;
  }
}

glm::mat4 Camera::CalcViewMatrix() {
  // Create a view matrix that looks in front of the camera.
  return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::GetCameraPosition() {
  return position;
}

glm::vec3 Camera::GetCameraDirection() {
  return glm::normalize(front);
}

void Camera::MouseControl(GLfloat x_change, GLfloat y_change) {
  // Don't need delta time here, since not dependent on framerate, but amount of mouse movement.
  x_change *= turn_speed;
  y_change *= turn_speed;

  yaw += x_change;
  pitch += y_change;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }

  if (pitch < -89.0f) {
    pitch = -89.0f;
  }
  Update();
}

Camera::~Camera() {

}