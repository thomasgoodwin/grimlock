#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "../util.h"

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
{
  GLuint vertexShader = CreateAndCompileShader(vertexFile, GL_VERTEX_SHADER);
  GLuint fragmentShader = CreateAndCompileShader(fragmentFile, GL_FRAGMENT_SHADER);
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  int success;
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (success != 1)
  {
    GLint logLength = 0;
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
      std::vector<GLchar> infoLog(logLength);
      glGetProgramInfoLog(ID, logLength, nullptr, infoLog.data());
      std::cerr << "Program link error:\n" << infoLog.data() << std::endl;
    }
  }
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

void Shader::setUniform1i(const std::string& name, int value) const
{
  GLint location = glGetUniformLocation(ID, name.c_str());
  if (location == -1) {
    //std::cerr << "Warning: uniform '" << name << "' not found in shader. ID: " << ID << "\n";
  }
  else {
    glUniform1i(location, value);
  }
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat)
{
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()),
    1, GL_FALSE, glm::value_ptr(mat));
}

GLuint Shader::CreateAndCompileShader(const std::string& shaderFile, GLuint shaderType)
{
  std::string shaderScript = getFileContents(shaderFile);
  const char* shaderSource = shaderScript.c_str();
  GLuint shader = glCreateShader(shaderType);

  glShaderSource(shader, 1, &shaderSource, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success != GL_TRUE)
  {
    char infoLog[2048];
    glGetShaderInfoLog(shader, 2048, nullptr, infoLog);
    std::cerr << "gl shader compilation error: " << infoLog << std::endl;
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}
