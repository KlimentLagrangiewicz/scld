#include <string>
#include <ranges>
#include <tbb/parallel_for_each.h>

#include "cppdownloader.hpp"


std::string to_format(const int number, const int diff = 0);
std::string getURL(const std::string &path, const int i, const std::string &fmt, const int diff = 0);
bool mySwap(int &a, int &b);
void downloadFromRange(const std::string &path, const std::string &fmt, const std::string &sfirst, const std::string &slast);