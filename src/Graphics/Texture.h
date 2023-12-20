#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <GL/glew.h>
#include <stb/stb_image.h>

#include "Shader.h"

class Texture
{
  public:
    GLuint ID;
    GLenum type;
    Texture(std::string image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    void texUnit(Shader shader, const char* uniform, GLuint unit);
    void bind();
    void unbind();
    void shutdown();
};

#endif