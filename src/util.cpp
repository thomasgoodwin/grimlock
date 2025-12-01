#include "Util.h"
#include <cerrno>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

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

uint64_t generate_uuid() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint64_t> dis(0, 0xFFFFFFFFFFFFFFFF);
  return dis(gen);
}