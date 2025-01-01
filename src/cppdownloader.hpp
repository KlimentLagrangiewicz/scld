#include <iostream>
#include <string>
#include <filesystem>

#include "downloader.h"


std::string getOutputFileName(const std::string inputFileName);
bool cppDownloadFile(const std::string fileURL, const std::string oFileName);
std::string getName(const std::string url);
void fileDownload(const std::string fileURL);