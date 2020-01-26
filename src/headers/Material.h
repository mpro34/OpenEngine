#pragma once

#include <GL/glew.h>

class Material {
  public:
    Material();
    Material(GLfloat s_intensity, GLfloat shine);

    void UseMaterial(GLuint specular_intensity_location, GLuint shininess_location);

    ~Material();
  private:
    GLfloat specular_intensity;
    GLfloat shininess;

};

// MATERIAL_H_