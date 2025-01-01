#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <omp.h>

#include "cppdownloader.hpp"


int getNumLines(const std::string file_name);
std::vector<std::string> getStringVec(const std::string file_name);
void downloadFromStringArray(const std::vector<std::string> array);
void downloadFromFile(const std::string file_name);