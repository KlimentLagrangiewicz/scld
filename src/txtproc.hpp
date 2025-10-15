#ifndef TXTPROC_HPP_
#define TXTPROC_HPP_

#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#endif
#endif

#include <algorithm>
#include <string>
#include <string_view>
#include <cstring>
#include <span>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <unordered_set>

#include "cppdownloader.hpp"

void downloadFromInput(const char **argv, int begin, int end);
void downloadFromInput(std::istream& input, const char **argv, int begin, int end);

void downloadFromFile(const char **argv, const int begin, const int end);
void downloadFromFile(std::istream& input, const char **argv, const int begin, const int end);

std::vector<std::string> getFormats(const std::string_view arg);
std::vector<std::string> getFormats(const char *arg);

#endif