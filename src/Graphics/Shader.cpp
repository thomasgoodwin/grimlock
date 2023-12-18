#include "Shader.h"
#include "../util.h"

Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile)
{
  GLuint vertexShader = CreateAndCompileShader(vertexFile, GL_VERTEX_SHADER);
  GLuint fragmentShader = CreateAndCompileShader(fragmentFile, GL_FRAGMENT_SHADER);
  ID = glCreateProgram();

  int success;
  glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[512];
    glGetProgramInfoLog(ID, 512, nullptr, infoLog);
    std::cerr << "gl program link error: " << infoLog << std::endl;
  }

  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::activate()
{
  glUseProgram(ID);
}

void Shader::shutdown()
{
  glDeleteProgram(ID);
}

GLuint Shader::CreateAndCompileShader(const std::string &shaderFile, GLuint shaderType)
{
  const char *shaderSource = GetFileContents(shaderFile).c_str();
  GLuint shader = glCreateShader(shaderType);

  glShaderSource(shader, 1, &shaderSource, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "gl shader compilation error: " << infoLog << std::endl;
  }

  return shader;
}
