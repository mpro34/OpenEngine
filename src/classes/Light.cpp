#include "../headers/Light.h"

Light::Light() {
  color = glm::vec3(1.0f, 1.0f, 1.0f); // How much color in each pixel to show
  ambient_intensity = 1.0f;

  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  diffuse_intensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a_intensity, 
          GLfloat x_dir, GLfloat y_dir, GLfloat z_dir, GLfloat d_intensity) {
  color = glm::vec3(red, green, blue);
  ambient_intensity = a_intensity;

  direction = glm::vec3(x_dir, y_dir, z_dir);
  diffuse_intensity = d_intensity;
}

void Light::UseLight(GLfloat ambient_insensity_loc, GLfloat ambient_color_loc,
                  GLfloat diffuse_intensity_location, GLfloat direction_location) {
  glUniform3f(ambient_color_loc, color.x, color.y, color.z);
  glUniform1f(ambient_insensity_loc, ambient_intensity);

  glUniform3f(direction_location, direction.x, direction.y, direction.z);
  glUniform1f(diffuse_intensity_location, diffuse_intensity);
}

Light::~Light() {

}