#include "argparser.hpp"


std::vector<std::string> getFormats(const std::string & arg) {
	std::vector<std::string> res;
	res.reserve(std::count(arg.begin(), arg.end(), ',') + 1);
	
	std::string added_str;
	for (const auto &c: arg) {
		if (c == ',') {
			if (!added_str.empty()) {
				res.push_back(added_str);
				added_str.clear();
			}
		} else {
			added_str += c;
		}
	}
	
	if (!added_str.empty()) {
		res.push_back(added_str);
		added_str.clear();
	}
	
	res.shrink_to_fit();	
	return res;
}

static inline std::size_t strlen(const char *str) {
	std::size_t i = 0;
	while (*str != '\0') {
		++str;
		++i;
	}
	return i;
}

std::vector<std::string> getFormats(const char* const arg) {
	const auto s = strlen(arg);
	
	std::vector<std::string> res;
	res.reserve(std::count(arg, arg + s, ',') + 1);
	
	std::string added_str;
	for (std::size_t i = 0; i < s; ++i) {
		const auto &c = arg[i];
		if (c == ',') {
			if (!added_str.empty()) {
				res.push_back(added_str);
				added_str.clear();
			}
		} else {
			added_str += c;
		}
	}
	
	if (!added_str.empty()) {
		res.push_back(added_str);
		added_str.clear();
	}
	
	res.shrink_to_fit();
	return res;
}