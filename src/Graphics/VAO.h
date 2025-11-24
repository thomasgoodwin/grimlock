#ifndef VAO_CLASS_H
#define VAO_CLASS_H
#include "VBO.h"

typedef unsigned int GLenum;

class VAO
{
public:
  GLuint ID;
  VAO();
  void linkAttribute(VBO& vbo, GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset);
  void bind();
  void unbind();
  void shutdown();
};

#endif