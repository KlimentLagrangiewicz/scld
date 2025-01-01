#include "range.hpp"

std::string to_format(const int number, int diff = 0) {
	std::stringstream res;
	res << std::setw(std::to_string(number).size() + diff) << std::setfill('0') << number;
	return res.str();
}

std::string getURL(const std::string path, const int i, const std::string fmt, int diff = 0){
	if (diff <= 0) {
		return path + std::to_string(i) + '.' + fmt;
	}
	return path + to_format(i, diff) + '.' + fmt;
}


bool mySwap(int &a, int &b) {
	if (a > b) {
		int  buf = a;
		a = b;
		b = buf;
		return true;
	}
	return false;
}

void downloadFromRange(const std::string path, const std::string fmt, const std::string sfirst, const std::string slast) {
	int first = stoi(sfirst), last = stoi(slast);
	int width = mySwap(first, last) ? slast.size() : sfirst.size();
	#pragma omp parallel for shared(first, last) firstprivate(width, path, fmt) schedule(dynamic)
	for (int i = first; i <= last; i++) {
		int diff = width - std::to_string(i).size();
		std::string url = getURL(path, i, fmt, diff);
		fileDownload(url);
	}
}