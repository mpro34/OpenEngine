#pragma once

#include <GL/glew.h>
#include "../../libs/glm/glm.hpp"
#include "../../libs/glm/gtc/matrix_transform.hpp"

#include "ShadowMap.h"

class Light {
  public:
    Light();
    Light(GLfloat shadow_width, GLfloat shadow_height,
          GLfloat red, GLfloat green, GLfloat blue, 
          GLfloat a_intensity, GLfloat d_intensity);

    ShadowMap* GetShadowMap() { return shadow_map; }

    ~Light();

  protected: // Accessible by subclasses, but private otherwise
    glm::vec3 color;
    GLfloat ambient_intensity;

    glm::vec3 direction;
    GLfloat diffuse_intensity;

    glm::mat4 light_proj;

    ShadowMap *shadow_map;
};

// LIGHT_H_