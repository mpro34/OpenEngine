#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "../../libs/glm/gtc/type_ptr.hpp"
#include "../../libs/glm/glm.hpp"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CommonValues.h"

class Shader {
  public:
    Shader();
    void CreateFromString(const char *vertex_code, const char *fragment_code);
    void CreateFromFiles(const char *vertex_loc, const char *fragment_loc);
    void CreateFromFiles(const char *vertex_loc, const char *geometry_loc, const char *fragment_loc);
    
    void Validate();

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
    GLuint GetOmniLightPosLocation();
    GLuint GetFarPlaneLocation();

    void SetDirectionalLight(DirectionalLight *d_light);
    void SetPointLights(PointLight * p_light, unsigned int light_count, unsigned int texture_unit, unsigned int offset);
    void SetSpotLights(SpotLight * s_light, unsigned int light_count, unsigned int texture_unit, unsigned int offset);
    void SetTexture(GLuint texture_unit);
    void SetDirectionalShadowMap(GLuint texture_unit);
    void SetDirectionalLightTransform(glm::mat4 *ltransform);
    void SetLightMatrices(std::vector<glm::mat4> light_matrices);

    void UseShader();
    void ClearShader();

    ~Shader();

  private:
    int point_light_count;
    int spot_light_count;

    GLuint shader_id, uniform_projection, uniform_model, uniform_view,
          uniform_eye_position, uniform_specular_intensity, uniform_shininess,
          uniform_directional_light_transform, uniform_directional_shadow_map,
          uniformTexture,
          uniform_omni_light_pos, uniform_far_plane;

    GLuint uniform_light_matrices[6];

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

    GLuint uniform_spot_light_count;
    struct {
      GLuint uniform_color;
      GLuint uniform_ambient_intensity;
      GLuint uniform_diffuse_intensity;

      GLuint uniform_position;
      GLuint uniform_constant;
      GLuint uniform_linear;
      GLuint uniform_exponent;

      GLuint uniform_direction;
      GLuint uniform_edge;
    } uniformSpotLight[MAX_SPOT_LIGHTS];

    struct {
      GLuint shadow_map;
      GLuint far_plane;
    } uniformOmniShadowMap[MAX_SPOT_LIGHTS + MAX_POINT_LIGHTS];

    void CompileShader(const char *vertex_code, const char *fragment_code);
    void CompileShader(const char *vertex_code, const char *geometry_code, const char *fragment_code);
    void AddShader(GLuint program, const char* shader_code, GLenum shader_type);

    void CompileProgram();
};

// SHADER_H_