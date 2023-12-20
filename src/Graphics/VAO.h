#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <GL/glew.h>
#include "VBO.h"

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