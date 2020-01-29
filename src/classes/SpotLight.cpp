#include "../headers/SpotLight.h"

SpotLight::SpotLight() : PointLight() {
  direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
  edge = 0.0f;
  proc_edge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLuint shadow_width, GLuint shadow_height,
          GLfloat near, GLfloat far,
          GLfloat red, GLfloat green, GLfloat blue, 
          GLfloat a_intensity, GLfloat d_intensity,
          GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
          GLfloat x_dir, GLfloat y_dir, GLfloat z_dir,
          GLfloat con, GLfloat lin, GLfloat exp,
          GLfloat edg) : PointLight(shadow_width, shadow_height, near, far, red, green, blue, a_intensity, d_intensity, x_pos, y_pos, z_pos, con, lin, exp) {
  // Constructor that will be used the most
  direction = glm::normalize(glm::vec3(x_dir, y_dir, z_dir));
  edge = edg;
  proc_edge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambient_insensity_loc, GLuint ambient_color_loc,
          GLuint diffuse_intensity_location, GLuint position_location, GLuint direction_location,
          GLuint constant_location, GLuint linear_location, GLuint exponent_location,
          GLuint edge_location) {
  // Bind the location values below to the shader's uniform values
  glUniform3f(ambient_color_loc, color.x, color.y, color.z);
  glUniform1f(ambient_insensity_loc, ambient_intensity);
  glUniform1f(diffuse_intensity_location, diffuse_intensity);

  glUniform3f(position_location, position.x, position.y, position.z);
  glUniform1f(constant_location, constant);
  glUniform1f(linear_location, linear);
  glUniform1f(exponent_location, exponent); 

  glUniform3f(direction_location, direction.x, direction.y, direction.z);
  glUniform1f(edge_location, proc_edge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir) {
  position = pos;
  direction = dir;
}

SpotLight::~SpotLight() {
  
}