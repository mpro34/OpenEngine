#pragma once

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap {
  public:
    ShadowMap();

    virtual bool Init(GLuint width, GLuint height);

    virtual void Write();

    virtual void Read(GLenum texture_unit);

    GLuint GetShadowWidth() { return shadow_width; }
    GLuint GetShadowHeight() { return shadow_height; }

    ~ShadowMap();

  protected:
    GLuint FBO, shadow_map;
    GLuint shadow_width, shadow_height;

};

// SHADOWMAP_H_