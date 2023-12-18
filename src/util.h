#ifndef UTIL
#define UTIL
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

#ifndef ENGDIR
#define ENGDIR "../"
#endif

std::string GetFileContents(const std::string& filename)
{
  std::ifstream file;
  std::string enginePath = ENGDIR + filename;
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


#endif