#include "util.h"
#include <cerrno>
#include <iostream>
#include <sstream>
#include <fstream>

std::string getFileContents(const std::string& filename)
{
  std::ifstream file;
  std::string enginePath = filename;
  file.open(enginePath, std::ios::in);
  std::stringstream contents;
  if (!file.fail())
  {
    contents << file.rdbuf();
  }
  else
  {
    std::cerr << "Failed to read file: " << enginePath << std::endl;
  }
  return contents.str();
}