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
#include <fstream>
#include <vector>
#include <stdexcept>
#include <unordered_set>

#include "cppdownloader.hpp"


void downloadFromFile(const std::string &file_name);

#endif