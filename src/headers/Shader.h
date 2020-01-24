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

    void UseShader();
    void ClearShader();

    ~Shader();

  private:
    GLuint shader_id, uniform_projection, uniform_model;

    void CompileShader(const char *vertex_code, const char *fragment_code);
    void AddShader(GLuint program, const char* shader_code, GLenum shader_type);
};

// SHADER_H_