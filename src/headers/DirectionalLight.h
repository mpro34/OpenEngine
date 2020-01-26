#pragma once

#include "Light.h"

class DirectionalLight : public Light {
  public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
                    GLfloat a_intensity, GLfloat d_intensity,
                    GLfloat x_dir, GLfloat y_dir, GLfloat z_dir);

    void UseLight(GLfloat ambient_insensity_loc, GLfloat ambient_color_loc,
                  GLfloat diffuse_intensity_location, GLfloat direction_location);

    ~DirectionalLight();
  
  private:
    glm::vec3 direction;

};

// DIRECTIONAL_LIGHT_