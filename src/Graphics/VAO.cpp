#include "VAO.h"

VAO::VAO()
{
  glGenVertexArrays(1, &ID);
  bind();
}
void VAO::linkAttribute(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
  VBO.bind();
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  VBO.unbind();
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