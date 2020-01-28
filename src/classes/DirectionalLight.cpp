#include "../headers/DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
  // By running the super constructor with the Light() syntax, we only need to set direction here.
  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  light_proj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadow_width, GLfloat shadow_height,
                                  GLfloat red, GLfloat green, GLfloat blue, 
                                  GLfloat a_intensity, GLfloat d_intensity,
                                  GLfloat x_dir, GLfloat y_dir, GLfloat z_dir) : Light(shadow_width, shadow_height, red, green, blue, a_intensity, d_intensity) {
  // Calls the super constructor first
  direction = glm::vec3(x_dir, y_dir, z_dir);
  light_proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLight(GLfloat ambient_insensity_loc, GLfloat ambient_color_loc,
                                GLfloat diffuse_intensity_location, GLfloat direction_location) {
  glUniform3f(ambient_color_loc, color.x, color.y, color.z);
  glUniform1f(ambient_insensity_loc, ambient_intensity);

  glUniform3f(direction_location, direction.x, direction.y, direction.z);
  glUniform1f(diffuse_intensity_location, diffuse_intensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform() {

  return light_proj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight() {

}