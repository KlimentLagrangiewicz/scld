#include "argparser.hpp"

std::vector<std::string> getFormats(const std::string_view arg) {
	std::vector<std::string> res;
	res.reserve(std::count(arg.begin(), arg.end(), ',') + 1);
	std::unordered_set<std::string> fmt_set;
	
	std::string added_str;
	for (const auto &c: arg) {
		if (c == ',') {
			if (!added_str.empty()) {
				if (!fmt_set.contains(added_str)) {
					res.push_back(added_str);
					fmt_set.insert(added_str);
				}
				added_str.clear();
			}
		} else {
			added_str += c;
		}
	}
	
	if (!added_str.empty()) {
		if (!fmt_set.contains(added_str)) {
			res.push_back(added_str);
			fmt_set.insert(added_str);
		}
		added_str.clear();
	}
	
	res.shrink_to_fit();
	fmt_set.clear();
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
	std::unordered_set<std::string> fmt_set;
	
	std::string added_str;
	for (std::size_t i = 0; i < s; ++i) {
		const auto &c = arg[i];
		if (c == ',') {
			if (!added_str.empty()) {
				if (!fmt_set.contains(added_str)) {
					res.push_back(added_str);
					fmt_set.insert(added_str);
				}
				added_str.clear();
			}
		} else {
			added_str += c;
		}
	}
	
	if (!added_str.empty()) {
		if (!fmt_set.contains(added_str)) {
			res.push_back(added_str);
			fmt_set.insert(added_str);
		}
		added_str.clear();
	}
	
	res.shrink_to_fit();
	fmt_set.clear();
	return res;
}

std::vector<std::string> getFilesURLs(const char **argv, const int begin, const int end) {
	std::unordered_set<std::string> url_set;
	std::vector<std::string> res;
	res.reserve(end - begin);
	
	for (int i = begin; i < end; ++i) {
		const char* const argv_i = argv[i];
		if (!url_set.contains(argv_i)) {
			url_set.insert(argv_i);
			res.push_back(argv_i);
		}
	}
	url_set.clear();
	res.shrink_to_fit();
	return res;
}