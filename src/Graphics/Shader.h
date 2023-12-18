#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <GL/glew.h>

class Shader
{
  public:
    GLuint ID;
    Shader(const std::string &vertexFile, const std::string &fragmentFile);
  
    void activate();
    void shutdown();
  
  private:
    GLuint CreateAndCompileShader(const std::string &shaderSource, GLuint shaderType);
};

#endif