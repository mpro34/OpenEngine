#include "../headers/Shader.h"

Shader::Shader() {
  shader_id = 0;
  uniform_model = 0;
  uniform_projection = 0;
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
  uniform_ambient_intensity = glGetUniformLocation(shader_id, "directional_light.ambient_intensity");
  uniform_ambient_color = glGetUniformLocation(shader_id, "directional_light.color");
  uniform_direction = glGetUniformLocation(shader_id, "directional_light.direction");
  uniform_diffuse_intensity = glGetUniformLocation(shader_id, "directional_light.diffuse_intensity");
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
  return uniform_ambient_intensity;
}

GLuint Shader::GetAmbientColorLocation() {
  return uniform_ambient_color;
}

GLuint Shader::GetDiffuseIntensityLocation() {
  return uniform_diffuse_intensity;
}

GLuint Shader::GetDirectionLocation() {
  return uniform_direction;
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