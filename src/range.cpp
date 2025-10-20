#include "range.hpp"


static inline std::string getURL(std::string_view path, const int i, std::string_view fmt, const int diff) {
	std::string result;
	if (diff <= 0) {
		result.reserve(path.size() + 11 + fmt.size() + 2);
		result.append(path);
		result.append(std::to_string(i));
	} else {
		const auto num_str = std::to_string(i);
		result.reserve(path.size() + diff + num_str.size() + fmt.size() + 2);
		result.append(path);
		result.append(diff, '0');
		result.append(num_str);
	}
	
	result.push_back('.');
	result.append(fmt);
	return result;
}

static inline bool mySwap(int &a, int &b) {
	if (a > b) {
		const int buf = a;
		a = b;
		b = buf;
		return true;
	}
	return false;
}

static inline int getIntWidth(int n) {
	if (n == 0) return 1;
	
	if (n == std::numeric_limits<int>::min()) return 11;
	
	const int sign = n < 0 ? 1 : 0;
	const unsigned un = n < 0 ? -static_cast<unsigned>(n) : n;
	
	if (un < 10u) return sign + 1;
	if (un < 100u) return sign + 2;
	if (un < 1000u) return sign + 3;
	if (un < 10000u) return sign + 4;
	if (un < 100000u) return sign + 5;
	if (un < 1000000u) return sign + 6;
	if (un < 10000000u) return sign + 7;
	if (un < 100000000u) return sign + 8;
	if (un < 1000000000u) return sign + 9;
	return sign + 10;
}


void downloadFromRange(const std::string &path, const std::string &fmt, const std::string &sfirst, const std::string &slast) {
	int first = std::stoi(sfirst), last = std::stoi(slast);
	const int width = mySwap(first, last) ? slast.size() : sfirst.size();
	const int max_output_width = getIntWidth(last);
	tbb::parallel_for(
		tbb::blocked_range<int>(first, last + 1),
		[&width, &fmt, &path, &max_output_width](const tbb::blocked_range<int>& range) {
			for (auto i = range.begin(); i != range.end(); ++i) {
				const std::string url = getURL(path, i, fmt, width - getIntWidth(i));
				fileDownloadSilently(url, i, max_output_width, fmt);
			}
		}
	);
}

void downloadFromRange(const std::string &path, const std::vector<std::string> &fmts, const std::string &sfirst, const std::string &slast) {
	if (fmts.empty()) throw std::runtime_error("fmts vector can't be empty");
	if (fmts.size() < 2) {
		downloadFromRange(path, fmts[0], sfirst, slast);
	} else {
		int first = std::stoi(sfirst), last = std::stoi(slast);
		const int width = mySwap(first, last) ? slast.size() : sfirst.size();
		const int max_output_width = getIntWidth(last);
		tbb::parallel_for(
			tbb::blocked_range<int>(first, last + 1),
			[&width, &fmts, &path, &max_output_width](const tbb::blocked_range<int>& range) {
				for (auto i = range.begin(); i != range.end(); ++i) {
					for (const auto & fmt: fmts) {
						const auto url = getURL(path, i, fmt, width - getIntWidth(i));
						fileDownloadSilently(url, i, max_output_width, fmt);
					}
				}
			}
		);
	}
}
