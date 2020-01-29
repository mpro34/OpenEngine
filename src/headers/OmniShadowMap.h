#pragma once

#include "ShadowMap.h"

class OmniShadowMap : public ShadowMap {
  public:
    OmniShadowMap();

    bool Init(GLuint width, GLuint height);

    void Write();

    void Read(GLenum texture_unit);
  
    ~OmniShadowMap();

  private:

};

// OMNISHADOWMAP_H_