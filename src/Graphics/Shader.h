#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

typedef unsigned int GLuint;

class Shader
{
public:
  GLuint ID;
  Shader(const std::string& vertexFile, const std::string& fragmentFile);
  void activate();
  void shutdown();
  void setUniform1i(const std::string& name, int value) const;
  void setMat3(const std::string& name, const glm::mat3& mat);
private:
  GLuint CreateAndCompileShader(const std::string& shaderSource, GLuint shaderType);
};

#endif