#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <iostream>

Texture::Texture(const std::string& path, bool flipVertically)
  : m_ID(0), m_width(0), m_height(0), m_channels(0)
{
  stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

  unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 4);
  if (!data) {
    std::cerr << "Failed to load texture: " << path << std::endl;
    return;
  }

  glGenTextures(1, &m_ID);
  glBindTexture(GL_TEXTURE_2D, m_ID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);
}

Texture::~Texture() {
  if (m_ID != 0) {
    glDeleteTextures(1, &m_ID);
  }
}

void Texture::bind(GLuint slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}