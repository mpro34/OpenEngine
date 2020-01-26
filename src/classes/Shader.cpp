#include "../headers/Shader.h"

Shader::Shader() {
  shader_id = 0;
  uniform_model = 0;
  uniform_projection = 0;

  point_light_count = 0;
  spot_light_count = 0;
}


void Shader::CreateFromString(const char *vertex_code, const char *fragment_code) {
  CompileShader(vertex_code, fragment_code);
}

void Shader::CreateFromFiles(const char *vertex_loc, const char *fragment_loc) {
  std::string vertex_string = ReadFile(vertex_loc);
  std::string fragment_string = ReadFile(fragment_loc);
  const char *vertex_code = vertex_string.c_str();
  const char *fragment_code = fragment_string.c_str();

  CompileShader(vertex_code, fragment_code);
}


std::string Shader::ReadFile(const char *file_loc) {
  std::string content;
  std::ifstream file_stream(file_loc, std::ios::in);

  if (!file_stream.is_open()) {
    printf("Failed to read %s! File doesn't exist.", file_loc);
    return "";
  }

  std::string line = "";
  while (!file_stream.eof()) {
    std::getline(file_stream, line);
    content.append(line + "\n");
  }

  file_stream.close();
  return content;
}


void Shader::CompileShader(const char *vertex_code, const char *fragment_code) {
  // Compile the shaders
  shader_id = glCreateProgram();

  if (!shader_id) {
    printf("Error creating shader program!\n");
    return;
  }

  AddShader(shader_id, vertex_code, GL_VERTEX_SHADER);
  AddShader(shader_id, fragment_code, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glLinkProgram(shader_id);
  glGetProgramiv(shader_id, GL_LINK_STATUS, &result);
  if(!result) {
    glGetProgramInfoLog(shader_id, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }

  glValidateProgram(shader_id);
  glGetProgramiv(shader_id, GL_VALIDATE_STATUS, &result);
  if(!result) {
    glGetProgramInfoLog(shader_id, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
    return;
  }

  uniform_model = glGetUniformLocation(shader_id, "model");
  uniform_projection = glGetUniformLocation(shader_id, "projection");
  uniform_view = glGetUniformLocation(shader_id, "view");
  uniformDirectionalLight.uniform_ambient_intensity = glGetUniformLocation(shader_id, "directional_light.base.ambient_intensity");
  uniformDirectionalLight.uniform_color = glGetUniformLocation(shader_id, "directional_light.base.color");
  uniformDirectionalLight.uniform_direction = glGetUniformLocation(shader_id, "directional_light.direction");
  uniformDirectionalLight.uniform_diffuse_intensity = glGetUniformLocation(shader_id, "directional_light.base.diffuse_intensity");
  uniform_specular_intensity = glGetUniformLocation(shader_id, "material.specular_intensity");
  uniform_shininess = glGetUniformLocation(shader_id, "material.shininess");
  uniform_eye_position = glGetUniformLocation(shader_id, "eye_position");

  uniform_point_light_count = glGetUniformLocation(shader_id, "point_light_count");

  for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
    
    char loc_buff[100] = {'\0'};

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].base.color", i);
    uniformPointLight[i].uniform_color = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].base.ambient_intensity", i);
    uniformPointLight[i].uniform_ambient_intensity = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].base.diffuse_intensity", i);
    uniformPointLight[i].uniform_diffuse_intensity = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].position", i);
    uniformPointLight[i].uniform_position = glGetUniformLocation(shader_id, loc_buff);
    
    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].constant", i);
    uniformPointLight[i].uniform_constant = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].linear", i);
    uniformPointLight[i].uniform_linear = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].exponent", i);
    uniformPointLight[i].uniform_exponent = glGetUniformLocation(shader_id, loc_buff);
  }

  uniform_spot_light_count = glGetUniformLocation(shader_id, "spot_light_count");

  for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {
    
    char loc_buff[100] = {'\0'};

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.base.color", i);
    uniformSpotLight[i].uniform_color = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.base.ambient_intensity", i);
    uniformSpotLight[i].uniform_ambient_intensity = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.base.diffuse_intensity", i);
    uniformSpotLight[i].uniform_diffuse_intensity = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.position", i);
    uniformSpotLight[i].uniform_position = glGetUniformLocation(shader_id, loc_buff);
    
    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.constant", i);
    uniformSpotLight[i].uniform_constant = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.linear", i);
    uniformSpotLight[i].uniform_linear = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.exponent", i);
    uniformSpotLight[i].uniform_exponent = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].direction", i);
    uniformSpotLight[i].uniform_direction = glGetUniformLocation(shader_id, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].edge", i);
    uniformSpotLight[i].uniform_edge = glGetUniformLocation(shader_id, loc_buff);
  }

}


void Shader::AddShader(GLuint program, const char* shader_code, GLenum shader_type) {
  // Add the shader
  GLuint the_shader = glCreateShader(shader_type);

  const GLchar* the_code[1];
  the_code[0] = shader_code;

  GLint code_length[1];
  code_length[0] = strlen(shader_code);

  glShaderSource(the_shader, 1, the_code, code_length);
  glCompileShader(the_shader);

  GLint result = 0;
  GLchar eLog[1024] = { 0 };

  glGetShaderiv(the_shader, GL_COMPILE_STATUS, &result);
  if(!result) {
    glGetShaderInfoLog(the_shader, sizeof(eLog), NULL, eLog);
    printf("Error compiling the %d shader: '%s'\n", shader_type, eLog);
    return;
  }
  glAttachShader(program, the_shader);
}

GLuint Shader::GetProjectionLocation() {
  return uniform_projection;
}


GLuint Shader::GetModelLocation() {
  return uniform_model;
}

GLuint Shader::GetViewLocation() {
  return uniform_view;
}

GLuint Shader::GetAmbientIntensityLocation() {
  return uniformDirectionalLight.uniform_ambient_intensity;
}

GLuint Shader::GetAmbientColorLocation() {
  return uniformDirectionalLight.uniform_color;
}

GLuint Shader::GetDiffuseIntensityLocation() {
  return uniformDirectionalLight.uniform_diffuse_intensity;
}

GLuint Shader::GetDirectionLocation() {
  return uniformDirectionalLight.uniform_direction;
}

GLuint Shader::GetEyePositionLocation() {
  return uniform_eye_position;
}

GLuint Shader::GetSpecularIntensityLocation() {
  return uniform_specular_intensity;
}

GLuint Shader::GetShininessLocation() {
  return uniform_shininess;
}

void Shader::SetDirectionalLight(DirectionalLight *d_light) {
  d_light->UseLight(uniformDirectionalLight.uniform_ambient_intensity, 
                    uniformDirectionalLight.uniform_color, 
                    uniformDirectionalLight.uniform_diffuse_intensity, 
                    uniformDirectionalLight.uniform_direction);
}

void Shader::SetPointLights(PointLight * p_light, unsigned int light_count) {
  // Only use a max light count of MAX_POINT_LIGHTS
  if (light_count > MAX_POINT_LIGHTS) light_count = MAX_POINT_LIGHTS;

  glUniform1i(uniform_point_light_count, light_count);

  for (size_t i = 0; i < light_count; i++) {
    p_light[i].UseLight(uniformPointLight[i].uniform_ambient_intensity,
                        uniformPointLight[i].uniform_color,
                        uniformPointLight[i].uniform_diffuse_intensity, 
                        uniformPointLight[i].uniform_position,
                        uniformPointLight[i].uniform_constant,
                        uniformPointLight[i].uniform_linear,
                        uniformPointLight[i].uniform_exponent);
  }
}

void Shader::SetSpotLights(SpotLight * s_light, unsigned int light_count) {
  // Only use a max light count of MAX_SPOT_LIGHTS
  if (light_count > MAX_SPOT_LIGHTS) light_count = MAX_SPOT_LIGHTS;

  glUniform1i(uniform_spot_light_count, light_count);

  for (size_t i = 0; i < light_count; i++) {
    s_light[i].UseLight(uniformSpotLight[i].uniform_ambient_intensity,
                        uniformSpotLight[i].uniform_color,
                        uniformSpotLight[i].uniform_diffuse_intensity, 
                        uniformSpotLight[i].uniform_position,
                        uniformSpotLight[i].uniform_direction,
                        uniformSpotLight[i].uniform_constant,
                        uniformSpotLight[i].uniform_linear,
                        uniformSpotLight[i].uniform_exponent,
                        uniformSpotLight[i].uniform_edge);
  }
}


void Shader::UseShader() {
  glUseProgram(shader_id);
}


void Shader::ClearShader() {
  if (shader_id != 0) {
    glDeleteProgram(shader_id);
    shader_id = 0;
  }

  uniform_model = 0;
  uniform_projection = 0;
}


Shader::~Shader() {
  ClearShader();
}