#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <GL/glew.h>

class VBO 
{
  public:
    GLuint ID;
    VBO();
    VBO(GLfloat* verticies, GLsizeiptr size);
    void bind();
    void unbind();
    void shutdown();
};

#endif