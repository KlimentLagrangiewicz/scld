#ifndef RANGE_HPP_
#define RANGE_HPP_

#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#endif
#endif

#include <string>
#include <vector>
#include <oneapi/tbb.h>
#include <stdexcept>

#include "cppdownloader.hpp"


void downloadFromRange(const std::string &path, const std::string &fmt, const std::string &sfirst, const std::string &slast);
void downloadFromRange(const std::string &path, const std::vector<std::string> &fmts, const std::string &sfirst, const std::string &slast);

#endif