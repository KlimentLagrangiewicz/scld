#include <iomanip>
#include <string>
#include <sstream>
#include <ranges>
#include <tbb/parallel_for_each.h>

#include "cppdownloader.hpp"


std::string to_format(const int number);
std::string getURL(const std::string path, const int i, const std::string fmt, int diff);
bool mySwap(int &a, int &b);
std::string getName(const std::string url);
void downloadFromRange(const std::string path, const std::string fmt, const std::string sfirst, const std::string slast);