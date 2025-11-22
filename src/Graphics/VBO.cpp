#include <GL/glew.h>
#include "VBO.h"

VBO::VBO()
{

}

VBO::VBO(GLfloat* verticies, GLsizeiptr size)
{
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
}

void VBO::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::shutdown()
{
  glDeleteBuffers(1, &ID);
}