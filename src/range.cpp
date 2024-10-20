#include "range.hpp"

std::string to_format(const int number, int diff) {
	std::stringstream res;
	res << std::setw(std::to_string(number).size() + diff) << std::setfill('0') << number;
	return res.str();
}

std::string get_url(std::string path, int i, std::string fmt, int diff = 0) {
	if (diff <= 0) {
		return path + std::to_string(i) + '.' + fmt;
	}
	return path + to_format(i, diff) + '.' + fmt;
}


bool my_swap(int &a, int &b) {
	if (a > b) {
		int  buf = a;
		a = b;
		b = buf;
		return true;
	}
	return false;
}

std::string get_name(std::string url) {
	return url.substr(url.rfind('/') + 1);
}

void download_range(std::string path, std::string fmt, std::string sfirst, std::string slast) {
	int first = stoi(sfirst), last = stoi(slast);
	int width = my_swap(first, last) ? slast.size() : sfirst.size();
	#pragma omp parallel for shared(first, last) firstprivate(width, path, fmt)
	for (int i = first; i <= last; i++) {
		int diff = width - std::to_string(i).size();
		std::string url = get_url(path, i, fmt, diff);
		try {
			bool res = download_image(url.c_str());
			#pragma omp critical
			{
				if (!res) {
					std::string name = get_name(url);
					std::cout << "Error during downloading file " << name << '\n';
					try {
						if (std::filesystem::remove(name))
							std::cout << "File " << name << " deleted.\n";
						else
						std::cout << "File " << name << " not found.\n";
					} catch(const std::filesystem::filesystem_error& err) {
						std::cout << "Filesystem error: " << err.what() << '\n';
					}
				}
			}
		} catch (...) {
			try {
				std::string name = get_name(url);
				std::cout << "Error during downloading file " << name << '\n';
				if (std::filesystem::remove(name))
					std::cout << "File " << name << " deleted.\n";
				else
					std::cout << "File " << name << " not found.\n";
			} catch(const std::filesystem::filesystem_error& err) {
				std::cout << "Filesystem error: " << err.what() << '\n';
			}
		}
	}
}