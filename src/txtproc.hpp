#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <omp.h>


#include "downloader.h"

extern std::string get_name(std::string url);

int get_lines_num(std::string file_name);
std::vector<std::string> get_lines(std::string file_name);
void download_array_files(std::vector<std::string> array);
void download_from_file(std::string file_name);