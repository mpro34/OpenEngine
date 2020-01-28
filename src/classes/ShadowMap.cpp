#include "../headers/ShadowMap.h"

ShadowMap::ShadowMap() {
  FBO = 0;
  shadow_map = 0;
}

bool ShadowMap::Init(GLuint width, GLuint height) {
  shadow_width = width; shadow_height = height;

  glGenFramebuffers(1, &FBO);

  glGenTextures(1, &shadow_map);
  glBindTexture(GL_TEXTURE_2D, shadow_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map, 0);

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE) {
    printf("Framebuffer Error: %i\n", status);
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return false;
}

void ShadowMap::Write() {
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void ShadowMap::Read(GLenum texture_unit) {
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, shadow_map);
}

ShadowMap::~ShadowMap() {
  if (FBO) {
    glDeleteFramebuffers(1, &FBO);
  }

  if (shadow_map) {
    glDeleteTextures(1, &shadow_map);
  }
}