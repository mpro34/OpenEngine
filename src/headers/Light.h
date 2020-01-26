#pragma once

#include <GL/glew.h>
#include "../../libs/glm/glm.hpp"

class Light {
  public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a_intensity, GLfloat d_intensity);

    ~Light();

  protected: // Accessible by subclasses, but private otherwise
    glm::vec3 color;
    GLfloat ambient_intensity;

    glm::vec3 direction;
    GLfloat diffuse_intensity;
};

// LIGHT_H_