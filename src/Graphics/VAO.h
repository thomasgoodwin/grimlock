#ifndef VAO_CLASS_H
#define VAO_CLASS_H
#include "VBO.h"

typedef unsigned int GLenum;

class VAO
{
  public:
    GLuint ID;
    VAO();
    void linkAttribute(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset); 
    void bind();
    void unbind();
    void shutdown();
};

#endif