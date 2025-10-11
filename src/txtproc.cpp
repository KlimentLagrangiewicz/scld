#include "txtproc.hpp"

static size_t getNumLines(const std::string &file_name) {
	std::ifstream fl(file_name, std::ios_base::in);
	if (!fl.is_open()) throw std::runtime_error("Error in opening \"" + file_name + "\" file.\n");
	
	if (fl.peek() == std::ifstream::traits_type::eof()) return 0;	
	const size_t res = 1 + std::count(std::istreambuf_iterator<char>(fl), std::istreambuf_iterator<char>(), '\n');
	fl.close();
	return res;
}

static inline void removeDuplicates(std::vector<std::string>& vec) {
	std::unordered_set<std::string> set;
	for (const auto& s : vec) {
		set.insert(std::move(s));
	}
	vec.assign(set.begin(), set.end());
	
	set.clear();
}

static std::vector<std::string> getStringVec(const std::string &file_name) {
	size_t s = getNumLines(file_name);
	if (s == 0) throw std::runtime_error("File \"" + file_name + "\" is empty.\n");
	
	std::vector<std::string> strVec;
	strVec.reserve(s);
	std::ifstream fl(file_name, std::ios_base::in);
	if (!fl.is_open()) throw std::runtime_error("Error in opening \"" + file_name + "\" file.\n");
	
	while (!fl.eof()) {
		std::string str;
		std::getline(fl, str);
		if (!str.empty()) strVec.push_back(str);
	}
	fl.close();
	
	strVec.shrink_to_fit();
	return strVec;
}

void downloadFromFile(const std::string &file_name) {
	std::vector<std::string> vec = getStringVec(file_name);
	
	removeDuplicates(vec);
	
	downloadFromStringArray(vec);
	
	vec.clear();
}