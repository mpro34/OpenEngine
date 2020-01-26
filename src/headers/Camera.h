#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../libs/glm/gtc/matrix_transform.hpp"

class Camera {
  public:
    Camera();
    Camera(glm::vec3 start_position, glm::vec3 start_up, GLfloat start_yaw, GLfloat start_pitch, GLfloat start_move_speed, GLfloat start_turn_speed);

    void KeyControl(bool *keys, GLfloat delta_time);
    void MouseControl(GLfloat x_change, GLfloat y_change);

    glm::vec3 GetCameraPosition();

    glm::mat4 CalcViewMatrix();

    ~Camera();

  private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movement_speed;
    GLfloat turn_speed;

    void Update();

};

// CAMERA_H_