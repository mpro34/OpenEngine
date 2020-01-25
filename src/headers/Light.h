#pragma once

#include <GL/glew.h>
#include "../../libs/glm/glm.hpp"

class Light {
  public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a_intensity, 
          GLfloat x_dir, GLfloat y_dir, GLfloat z_dir, GLfloat d_intensity);

    void UseLight(GLfloat ambient_insensity_loc, GLfloat ambient_color_loc,
                  GLfloat diffuse_intensity_location, GLfloat direction_location);

    ~Light();

  private:
    glm::vec3 color;
    GLfloat ambient_intensity;

    glm::vec3 direction;
    GLfloat diffuse_intensity;
};

// LIGHT_H_