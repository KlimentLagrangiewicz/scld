#ifndef CPPDOWNLOADER_HPP_
#define CPPDOWNLOADER_HPP_

#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#endif
#endif

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <oneapi/tbb.h>

#include "downloader.hpp"

void fileDownload(const std::string &fileURL);
void fileDownloadSilently(const std::string &fileURL);
void downloadFromStringArray(const std::vector<std::string> &array);

#endif