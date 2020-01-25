#pragma once

#include <GL/glew.h>
#include "../../libs/glm/glm.hpp"

class Light {
  public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a_intensity);

    void UseLight(GLfloat ambient_insensity_loc, GLfloat ambient_color_loc);

    ~Light();

  private:
    glm::vec3 color;
    GLfloat ambient_intensity;
};

// LIGHT_H_