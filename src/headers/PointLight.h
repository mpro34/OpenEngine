#pragma once

#include "Light.h"

class PointLight : public Light {
  public:
    PointLight();

    PointLight(GLfloat red, GLfloat green, GLfloat blue, 
              GLfloat a_intensity, GLfloat d_intensity,
              GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
              GLfloat con, GLfloat lin, GLfloat exp);
    void UseLight(GLfloat ambient_insensity_loc, GLfloat ambient_color_loc,
              GLfloat diffuse_intensity_location, GLfloat position_location,
              GLfloat constant_location, GLfloat linear_location, GLfloat exponent_location);
    ~PointLight();

  private:
    glm::vec3 position;

    GLfloat constant, linear, exponent;
};

// POINTLIGHT_H_