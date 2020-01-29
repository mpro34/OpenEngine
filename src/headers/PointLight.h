#pragma once

#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>

class PointLight : public Light {
  public:
    PointLight();

    PointLight(GLuint shadow_width, GLuint shadow_height,
              GLfloat near, GLfloat far,
              GLfloat red, GLfloat green, GLfloat blue, 
              GLfloat a_intensity, GLfloat d_intensity,
              GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
              GLfloat con, GLfloat lin, GLfloat exp);

    void UseLight(GLuint ambient_insensity_loc, GLuint ambient_color_loc,
              GLuint diffuse_intensity_location, GLuint position_location,
              GLuint constant_location, GLuint linear_location, GLuint exponent_location);
    
    std::vector<glm::mat4> CalculateLightTransform();
    GLfloat GetFarPlane();

    ~PointLight();

  protected:
    glm::vec3 position;

    GLfloat constant, linear, exponent;

    GLfloat far_plane;
};

// POINTLIGHT_H_