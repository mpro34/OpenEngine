#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"

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

    void SetDirectionalLight(DirectionalLight *d_light);
    void SetPointLights(PointLight * p_light, unsigned int light_count);

    void UseShader();
    void ClearShader();

    ~Shader();

  private:
    int point_light_count;

    GLuint shader_id, uniform_projection, uniform_model, uniform_view,
          uniform_eye_position, uniform_specular_intensity, uniform_shininess;

    struct {
      GLuint uniform_color;
      GLuint uniform_ambient_intensity;
      GLuint uniform_diffuse_intensity;

      GLuint uniform_direction;
    } uniformDirectionalLight;

    GLuint uniform_point_light_count;

    struct {
      GLuint uniform_color;
      GLuint uniform_ambient_intensity;
      GLuint uniform_diffuse_intensity;

      GLuint uniform_position;
      GLuint uniform_constant;
      GLuint uniform_linear;
      GLuint uniform_exponent;
    } uniformPointLight[MAX_POINT_LIGHTS];

    void CompileShader(const char *vertex_code, const char *fragment_code);
    void AddShader(GLuint program, const char* shader_code, GLenum shader_type);
};

// SHADER_H_