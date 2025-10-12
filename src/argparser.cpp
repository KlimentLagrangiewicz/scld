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

static inline std::string getName(const std::string &url) {
	if (url.empty()) return "";
	
	auto scheme_pos = url.find("://");
	size_t start_search = 0;
	
	if (scheme_pos != std::string::npos) start_search = scheme_pos + 3;
	
	auto fragment_pos = url.find('#', start_search);
	auto query_pos = url.find('?', start_search);
	
	auto path_end = std::min(
		fragment_pos != std::string::npos ? fragment_pos : url.size(),
		query_pos != std::string::npos ? query_pos : url.size()
	);
	
	auto pos = url.rfind('/', path_end);
	
	if (pos == std::string::npos)
		return start_search == 0 ? url.substr(0, path_end) : url.substr(start_search, path_end - start_search);


	auto name_start = pos + 1;
	if (name_start >= path_end) return "";

	return url.substr(name_start, path_end - name_start);
}

std::vector<std::string> getFilesURLs(const char **argv, const int begin, const int end, size_t &pos) {
	std::unordered_set<std::string> url_set;
	std::unordered_set<std::string> name_set;
	
	std::vector<std::string> vec1;
	vec1.reserve(end - begin);
	
	std::vector<std::string> vec2;
	vec2.reserve(end - begin);
	
	for (int i = begin; i < end; ++i) {
		const char* const argv_i = argv[i];
		
		if (*argv_i != '\0') {
			if (!url_set.contains(argv_i)) {
				const std::string name = getName(argv_i);
				
				if (!name_set.contains(name)) {
					name_set.insert(name);
					vec1.push_back(argv_i);
				} else {
					vec2.push_back(argv_i);
				}
				url_set.insert(argv_i);
			}
		}
	}
	url_set.clear();
	name_set.clear();
	
	vec2.shrink_to_fit();
	
	const auto vec1s = vec1.size();
	
	pos = vec1s;
	
	if (const auto vec2s = vec2.size(); vec1.capacity() < vec1s + vec2s) {
		vec1.resize(vec1s + vec2s);
	}
	
	vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	
	vec2.clear();
	
	vec1.shrink_to_fit();
	return vec1;
}
