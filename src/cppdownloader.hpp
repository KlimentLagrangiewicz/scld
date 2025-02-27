#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <tbb/parallel_for_each.h>
#include <tbb/spin_mutex.h>

extern "C"{
#include "downloader.h"
}

std::string getOutputFileName(const std::string inputFileName);
bool cppDownloadFile(const std::string fileURL, const std::string oFileName);
std::string getName(const std::string url);
void printError(const std::string fileURL, const std::string oFileName);
void fileDownload(const std::string fileURL);
void downloadFromStringArray(const std::vector<std::string> array);