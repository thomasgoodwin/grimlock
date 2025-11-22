#ifndef VBO_CLASS_H
#define VBO_CLASS_H

typedef unsigned int GLuint;
typedef float GLfloat;
typedef ptrdiff_t GLsizeiptr;

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