#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
  public:
    Shader();
    void CreateFromString(const char *vertex_code, const char *fragment_code);
    void CreateFromFiles(const char *vertex_loc, const char *fragment_loc);
    std::string ReadFile(const char *file_location); 

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    GLuint GetViewLocation();
    GLuint GetAmbientIntensityLocation();
    GLuint GetAmbientColorLocation();
    GLuint GetDiffuseIntensityLocation();
    GLuint GetDirectionLocation();
    GLuint GetEyePositionLocation();
    GLuint GetSpecularIntensityLocation();
    GLuint GetShininessLocation();

    void UseShader();
    void ClearShader();

    ~Shader();

  private:
    GLuint shader_id, uniform_projection, uniform_model, uniform_view,
          uniform_ambient_intensity, uniform_ambient_color, uniform_diffuse_intensity, 
          uniform_direction, uniform_eye_position, uniform_specular_intensity, uniform_shininess;

    void CompileShader(const char *vertex_code, const char *fragment_code);
    void AddShader(GLuint program, const char* shader_code, GLenum shader_type);
};

// SHADER_H_