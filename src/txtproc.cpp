#include "txtproc.hpp"

static size_t getNumLines(const std::string &file_name) {
	std::ifstream fl(file_name, std::ios_base::in);
	if (!fl.is_open()) throw std::runtime_error("Error in opening \"" + file_name + "\" file.\n");
	
	if (fl.peek() == std::ifstream::traits_type::eof()) return 0;	
	const size_t res = 1 + std::count(std::istreambuf_iterator<char>(fl), std::istreambuf_iterator<char>(), '\n');
	fl.close();
	return res;
}

static std::vector<std::string> getStringVec(const std::string &file_name, size_t &pos) {
	size_t s = getNumLines(file_name);
	if (s == 0) throw std::runtime_error("File \"" + file_name + "\" is empty.\n");
	
	std::unordered_set<std::string> url_set;
	std::unordered_set<std::string> name_set;
	
	std::vector<std::string> vec1;
	vec1.reserve(s);
	
	std::vector<std::string> vec2;
	vec2.reserve(s);
	
	std::ifstream fl(file_name, std::ios_base::in);
	if (!fl.is_open()) throw std::runtime_error("Error in opening \"" + file_name + "\" file.\n");
	
	while (!fl.eof()) {
		std::string str;
		std::getline(fl, str);
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

void downloadFromFile(const std::string &file_name) {
	size_t pos = 0;
	const std::vector<std::string> vec = getStringVec(file_name, pos);
	
	const auto vec_s = vec.size();
	
	downloadFromStringArray(std::span<const std::string>(vec.data(), std::min(pos, vec_s)));
	if (pos < vec_s) downloadFromStringArraySerialy(std::span<const std::string>(vec.data() + pos, vec_s));
}