#ifndef ARGPARSER_HPP_
#define ARGPARSER_HPP_

#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#endif
#endif

#include <string_view>
#include <unordered_set>
#include <string>
#include <vector>
#include <algorithm>


std::vector<std::string> getFormats(const std::string_view arg);
std::vector<std::string> getFormats(const char *arg);
std::vector<std::string> getFilesURLs(const char **argv, const int begin, const int end);

#endif