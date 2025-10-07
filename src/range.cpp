#include "range.hpp"


static inline std::string to_format(const int number, const int diff) {
	return std::string(diff, '0') + std::to_string(number);
}

static inline std::string getURL(const std::string &path, const int i, const std::string &fmt, const int diff) {
	if (diff <= 0) return path + std::to_string(i) + '.' + fmt;
	return path + to_format(i, diff) + '.' + fmt;
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

void downloadFromRange(const std::string &path, const std::string &fmt, const std::string &sfirst, const std::string &slast) {
	int first = std::stoi(sfirst), last = std::stoi(slast);
	const int width = mySwap(first, last) ? slast.size() : sfirst.size();
	tbb::parallel_for(
		tbb::blocked_range<int>(first, last + 1),
		[&width, &fmt, &path](const tbb::blocked_range<int>& range) {
			for (auto i = range.begin(); i != range.end(); ++i) {
				const std::string url = getURL(path, i, fmt, width - std::to_string(i).size());
				fileDownload(url);
			}
		}
	);
}