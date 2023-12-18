#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <GL/glew.h>
#include "VBO.h"

class VAO
{
  public:
    GLuint ID;
    VAO();
    void linkVBO(VBO VBO, GLuint layout); 
    void bind();
    void unbind();
    void shutdown();
};

#endif