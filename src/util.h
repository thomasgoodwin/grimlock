#ifndef UTIL_H
#define UTIL_H
#include <string>

#ifndef DEBUG
#define DEBUG false
#endif

std::string getFileContents(const std::string& filename);
uint64_t generate_uuid();

#endif