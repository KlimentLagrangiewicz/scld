#include "txtproc.hpp"

static inline void trim_whitespace_inplace(std::string& s) {
	static constexpr const char* whitespace = " \t\n\r\f\v\"'";
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
				if (const auto name = getName(line); !name.empty()) {
					url_set.insert(line);
					if (!name_set.contains(name)) {
						name_set.insert(name);
						vec1.push_back(line);
					} else {
						vec2.push_back(line);
					}
				}
			}
		}
	}
	
	
	std::string line;	
	while (std::getline(input, line)) {
		trim_whitespace_inplace(line);
		if (!line.empty()) {
			if (!url_set.contains(line)) {
				if (const auto name = getName(line); !name.empty()) {
					url_set.insert(line);
					if (!name_set.contains(name)) {
						name_set.insert(name);
						vec1.push_back(line);
					} else {
						vec2.push_back(line);
					}
				}
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

static std::vector<std::string> getFilesNames(const char **argv, const int begin, const int end, size_t &sum) {
	
	std::unordered_set<std::string> fn_set;
	std::vector<std::string> fn_vec;
	
	fn_vec.reserve(end - begin);
	
	sum = 0;
	
	for (int i = begin; i < end; ++i) {
		const std::string fn(argv[i]);
		if (!fn.empty()) {
			if (const auto numl = getNumLines(fn); numl > 0 && !fn_set.contains(fn)) {
				fn_set.insert(fn);
				fn_vec.push_back(fn);
				++sum;
			}
		}
	}
	fn_set.clear();
	
	fn_vec.shrink_to_fit();
	return fn_vec;
}

static std::vector<std::string> getFilesNames(std::istream& input, const char **argv, const int begin, const int end, size_t &sum) {
	
	std::unordered_set<std::string> fn_set;
	std::vector<std::string> fn_vec;
	
	fn_vec.reserve(end - begin);
	
	sum = 0;
	
	for (int i = begin; i < end; ++i) {
		const std::string fn(argv[i]);
		if (!fn.empty()) {
			if (const auto numl = getNumLines(fn); numl > 0 && !fn_set.contains(fn)) {
				fn_set.insert(fn);
				fn_vec.push_back(fn);
				++sum;
			}
		}
	}
	
	std::string line;
	while (std::getline(input, line)) {
		if (!line.empty()) {
			if (const auto numl = getNumLines(line); numl > 0 && !fn_set.contains(line)) {
				fn_set.insert(line);
				fn_vec.push_back(line);
				++sum;
			}
		}
	}
	
	fn_set.clear();
	
	fn_vec.shrink_to_fit();
	return fn_vec;
}

static void fileReading(const std::string &file_name, std::vector<std::string>& vec1, std::vector<std::string>& vec2, std::unordered_set<std::string>& url_set, std::unordered_set<std::string>& name_set) {
	std::ifstream fl(file_name, std::ios_base::in);
	if (!fl.is_open()) return;
	
	while (!fl.eof()) {
		std::string str;
		std::getline(fl, str);
		trim_whitespace_inplace(str);
		if (!str.empty()) {
			if (!url_set.contains(str)) {
				url_set.insert(str);
				
				if (const auto name = getName(str); !name.empty()) {
					if (!name_set.contains(name)) {
						name_set.insert(name);
						vec1.push_back(str);
					} else {
						vec2.push_back(str);
					}
				}
			}
		}
	}
	fl.close();
}

static std::vector<std::string> extractURLsFromFiles(const std::span<const std::string> files, size_t reserved, size_t &pos) {
	
	std::vector<std::string> vec1, vec2;
	
	vec1.reserve(reserved);
	vec2.reserve(reserved);
	
	std::unordered_set<std::string> url_set, name_set;
	
	for (const auto &file_name: files) {
		if (!file_name.empty()) {
			fileReading(file_name, vec1, vec2, url_set, name_set);
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

void downloadFromFile(const char **argv, const int begin, const int end) {
	size_t sum, pos;
	const auto fns = getFilesNames(argv, begin, end, sum);
	
	const auto urls = extractURLsFromFiles(fns, sum, pos);
	
	const auto vec_s = urls.size();
	downloadFromStringArray(std::span<const std::string>(urls.data(), std::min(pos, vec_s)));
	if (pos < vec_s) downloadFromStringArraySerialy(std::span<const std::string>(urls.data() + pos, vec_s));
}

void downloadFromFile(std::istream& input, const char **argv, const int begin, const int end) {
	size_t sum, pos;
	const auto fns = getFilesNames(input, argv, begin, end, sum);
	
	const auto urls = extractURLsFromFiles(fns, sum, pos);
	
	const auto vec_s = urls.size();
	downloadFromStringArray(std::span<const std::string>(urls.data(), std::min(pos, vec_s)));
	if (pos < vec_s) downloadFromStringArraySerialy(std::span<const std::string>(urls.data() + pos, vec_s));
}

std::vector<std::string> getFormats(const std::string_view arg) {
	std::vector<std::string> res;
	res.reserve(std::count(arg.begin(), arg.end(), ',') + 1);
	std::unordered_set<std::string> fmt_set;
	
	std::string added_str;
	for (const auto c: arg) {
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
		const auto c = arg[i];
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