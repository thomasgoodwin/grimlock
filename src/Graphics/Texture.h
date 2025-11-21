#pragma once
#include <string>
#include <GL/glew.h>
#include "stb_image.h"

class Texture {
public:
  Texture(const std::string& path, bool flipVertically = true);
  ~Texture();

  void bind(GLuint slot = 0) const;
  void unbind() const;

  GLuint getID() const { return m_ID; }
  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }

private:
  GLuint m_ID;
  int m_width, m_height, m_channels;
};