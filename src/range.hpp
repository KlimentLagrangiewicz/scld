#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <filesystem>
#include <omp.h>

#include "downloader.h"


std::string to_format(const int number, int diff);
std::string get_url(std::string path, int i, std::string fmt, int diff);
bool my_swap(int &a, int &b);
std::string get_name(std::string url);
void download_range(std::string path, std::string fmt, std::string sfirst, std::string slast);