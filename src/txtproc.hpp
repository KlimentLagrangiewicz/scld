#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "cppdownloader.hpp"


size_t getNumLines(const std::string &file_name);
std::vector<std::string> getStringVec(const std::string &file_name);
void downloadFromFile(const std::string &file_name);