#include "txtproc.hpp"

static inline void trim_whitespace_inplace(std::string& s) {
	static constexpr const char* whitespace = " \t\n\r\f\v";
	if (s.empty()) return;
	if (const auto first = s.find_first_not_of(whitespace); first == std::string::npos) {
		s.clear();
	} else {
		const auto last = s.find_last_not_of(whitespace);
		const auto len = last - first + 1;
		if (first > 0 || last < s.size() - 1) {
			s.assign(s, first, len);
		}
	}
}

static std::vector<std::string> inputToStrVec(std::istream& input, const char **argv, int begin, int end, size_t &pos) {
	
	std::unordered_set<std::string> url_set;
	std::unordered_set<std::string> name_set;
	
	std::vector<std::string> vec1, vec2;
	
	
	vec1.reserve(end - begin + 41);
	vec2.reserve(end - begin + 21);
	
	for (int i = begin; i < end; ++i) {
		std::string line(argv[i]);
		trim_whitespace_inplace(line);
		
		if (!line.empty()) {
			if (!url_set.contains(line)) {
				const std::string name = getName(line);
				
				if (!name_set.contains(name)) {
					name_set.insert(name);
					vec1.push_back(line);
				} else {
					vec2.push_back(line);
				}
				url_set.insert(line);
			}
		}
	}
	
	
	std::string line;	
	while (std::getline(input, line)) {
		trim_whitespace_inplace(line);
		if (!line.empty()) {
			if (!url_set.contains(line)) {
				const std::string name = getName(line);
				
				if (!name_set.contains(name)) {
					name_set.insert(name);
					vec1.push_back(line);
				} else {
					vec2.push_back(line);
				}
				url_set.insert(line);
			}
		}
	}
	
	url_set.clear();
	name_set.clear();
	
	vec2.shrink_to_fit();
	
	const auto vec1s = vec1.size();
	
	pos = vec1s;
	
	if (const auto vec2s = vec2.size(); vec1.capacity() < vec1s + vec2s) vec1.resize(vec1s + vec2s);
	
	vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	
	vec2.clear();
	
	vec1.shrink_to_fit();
	return vec1;
}

static std::vector<std::string> inputToStrVec(const char **argv, int begin, int end, size_t &pos) {
	
	std::unordered_set<std::string> url_set;
	std::unordered_set<std::string> name_set;
	
	std::vector<std::string> vec1, vec2;
	
	
	vec1.reserve(end - begin);
	vec2.reserve(end - begin);
	
	for (int i = begin; i < end; ++i) {
		std::string line(argv[i]);
		trim_whitespace_inplace(line);
		
		if (!line.empty()) {
			if (!url_set.contains(line)) {
				const std::string name = getName(line);
				
				if (!name_set.contains(name)) {
					name_set.insert(name);
					vec1.push_back(line);
				} else {
					vec2.push_back(line);
				}
				url_set.insert(line);
			}
		}
	}
	url_set.clear();
	name_set.clear();
	
	vec2.shrink_to_fit();
	
	const auto vec1s = vec1.size();
	
	pos = vec1s;
	
	if (const auto vec2s = vec2.size(); vec1.capacity() < vec1s + vec2s) vec1.resize(vec1s + vec2s);
	
	vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	
	vec2.clear();
	
	vec1.shrink_to_fit();
	return vec1;
}


void downloadFromInput(const char **argv, int begin, int end) {
	size_t pos = 0;
	const std::vector<std::string> vec = inputToStrVec(argv, begin, end, pos);
	
	if (vec.empty()) throw std::runtime_error("String vector is empty.\n");
	
	const auto vec_s = vec.size();
	
	downloadFromStringArray(std::span<const std::string>(vec.data(), std::min(pos, vec_s)));
	if (pos < vec_s) downloadFromStringArraySerialy(std::span<const std::string>(vec.data() + pos, vec_s));
}

void downloadFromInput(std::istream& input, const char **argv, int begin, int end) {
	size_t pos = 0;
	const std::vector<std::string> vec = inputToStrVec(input, argv, begin, end, pos);
	
	if (vec.empty()) throw std::runtime_error("String vector is empty.\n");
	
	const auto vec_s = vec.size();
	
	downloadFromStringArray(std::span<const std::string>(vec.data(), std::min(pos, vec_s)));
	if (pos < vec_s) downloadFromStringArraySerialy(std::span<const std::string>(vec.data() + pos, vec_s));
}

static inline size_t getNumLines(const std::string &file_name) {
	std::ifstream fl(file_name, std::ios_base::in);
	if (!fl.is_open()) return 0;
	
	if (fl.peek() == std::ifstream::traits_type::eof()) return 0;
	const size_t res = 1 + std::count(std::istreambuf_iterator<char>(fl), std::istreambuf_iterator<char>(), '\n');
	fl.close();
	return res;
}

static std::vector<std::string> getStringVec(const std::string &file_name, size_t &pos) {
	
	std::ifstream fl(file_name, std::ios_base::in);
	if (!fl.is_open()) {
		pos = 0;
		return {};
	}
	
	size_t s = getNumLines(file_name);
	if (s == 0) {
		pos = 0;
		return {};
	}
	
	std::unordered_set<std::string> url_set;
	std::unordered_set<std::string> name_set;
	
	std::vector<std::string> vec1;
	vec1.reserve(s);
	
	std::vector<std::string> vec2;
	vec2.reserve(s);
	
	while (!fl.eof()) {
		std::string str;
		std::getline(fl, str);
		trim_whitespace_inplace(str);
		if (!str.empty()) {
			if (!url_set.contains(str)) {
				const std::string name = getName(str);
				
				if (!name_set.contains(name)) {
					name_set.insert(name);
					vec1.push_back(str);
				} else {
					vec2.push_back(str);
				}
				url_set.insert(str);
			}
		}
	}
	fl.close();
	
	url_set.clear();
	name_set.clear();
	
	vec2.shrink_to_fit();
	
	const auto vec1s = vec1.size();
	
	pos = vec1s;
	
	if (const auto vec2s = vec2.size(); vec1.capacity() < vec1s + vec2s) vec1.resize(vec1s + vec2s);
	
	vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	
	vec2.clear();
	
	vec1.shrink_to_fit();
	return vec1;
}


static void downloadFromFile(const std::string &file_name) {
	size_t pos = 0;
	const std::vector<std::string> vec = getStringVec(file_name, pos);
	
	if (vec.empty()) return;
	
	const auto vec_s = vec.size();
	
	downloadFromStringArray(std::span<const std::string>(vec.data(), std::min(pos, vec_s)));
	if (pos < vec_s) downloadFromStringArraySerialy(std::span<const std::string>(vec.data() + pos, vec_s));
}

void downloadFromFile(const char **argv, const int begin, const int end) {
	for (int i = begin; i < end; ++i) {
		const char* const argv_i = argv[i];
		if (*argv_i != '\0') {
			downloadFromFile(argv_i);
		}
		
	}
}

void downloadFromFile(std::istream& input, const char **argv, const int begin, const int end) {
	for (int i = begin; i < end; ++i) {
		const char* const argv_i = argv[i];
		if (*argv_i != '\0') {
			downloadFromFile(argv_i);
		}
		
	}
	
	std::string line;
	while (std::getline(input, line)) {
		if (!line.empty()) {
			downloadFromFile(line);
		}
	}

}

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