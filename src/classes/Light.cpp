#include "../headers/Light.h"

Light::Light() {
  color = glm::vec3(1.0f, 1.0f, 1.0f); // How much color in each pixel to show
  ambient_intensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a_intensity) {
  color = glm::vec3(red, green, blue);
  ambient_intensity = a_intensity;
}

void Light::UseLight(GLfloat ambient_insensity_loc, GLfloat ambient_color_loc) {
  glUniform3f(ambient_color_loc, color.x, color.y, color.z);
  glUniform1f(ambient_insensity_loc, ambient_intensity);
}

Light::~Light() {

}