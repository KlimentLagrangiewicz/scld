#include "txtproc.hpp"

int getNumLines(const std::string file_name) {
	std::fstream fl;
	fl.open(file_name, std::ios_base::in);
	if (!fl.is_open()) {
		std::cout << "Error in opening \"" << file_name << "\" file.\n";
		std::exit(1);
	}
	if (fl.peek() == std::ifstream::traits_type::eof()) return 0;
	int res = 1 + std::count(std::istreambuf_iterator<char>(fl), std::istreambuf_iterator<char>(), '\n');
	fl.close();
	return res;
}

std::vector<std::string> getStringVec(const std::string file_name) {
	std::vector<std::string> strVec;
	int s = getNumLines(file_name);
	if (s == 0) {
		std::cout << "File \"" << file_name << "\" is empty.\n";
		std::exit(1);
	}
	strVec.reserve(s);
	std::fstream fl;
	fl.open(file_name, std::ios_base::in);
	if (!fl.is_open()) {
		std::cout << "Error in opening \"" << file_name << "\" file.\n";
		std::exit(1);
	}
	while (!fl.eof()) {
		std::string str;
		std::getline(fl, str);
		if (!str.empty()) strVec.push_back(str);
	}
	fl.close();
	return strVec;
}

void downloadFromFile(const std::string file_name) {
	std::vector<std::string> vec = getStringVec(file_name);
	downloadFromStringArray(vec);
	vec.clear();
}