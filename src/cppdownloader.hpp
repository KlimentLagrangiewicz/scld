#ifndef CPPDOWNLOADER_HPP_
#define CPPDOWNLOADER_HPP_

#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
		#include <winsock2.h>
	#endif
#endif

#include <string>
#include <span>
#include <filesystem>
#include <oneapi/tbb.h>
#include <charconv>
#include <array>

#include "downloader.hpp"

std::string getName(const std::string &url);
void fileDownload(const std::string &fileURL);
void fileDownload(const std::string &fileURL, const int n, const int maxWidth, const std::string & suffixus);
void fileDownloadSilently(const std::string &fileURL);
void fileDownloadSilently(const std::string &fileURL, const int n, const int maxWidth, const std::string & suffixus);
void downloadFromStringArray(const std::span<const std::string> array);
void downloadFromStringArraySerialy(const std::span<const std::string> array);

#endif