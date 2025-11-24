#include <GL/glew.h>
#include "VAO.h"

VAO::VAO()
{
  glGenVertexArrays(1, &ID);
  bind();
}
void VAO::linkAttribute(VBO& vbo, GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset)
{
  glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(index);
}
void VAO::bind()
{
  glBindVertexArray(ID);
}
void VAO::unbind()
{
  glBindVertexArray(0);
}
void VAO::shutdown()
{
  glDeleteVertexArrays(1, &ID);
}