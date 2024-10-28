#include "txtproc.hpp"

int get_lines_num(std::string file_name) {
	std::fstream fl;
	fl.open(file_name, std::ios_base::in);
	if (!fl.is_open()) {
		std::cout << "Error in opening \"" << file_name << "\" file.\n";
		std::exit(1);
	}
	if (fl.peek() == std::ifstream::traits_type::eof()) {
		std::cout << "File \"" << file_name << "\" is empty.\n";
		std::exit(1);
	}
	int res = 1 + std::count(std::istreambuf_iterator<char>(fl), std::istreambuf_iterator<char>(), '\n');
	fl.close();
	return res;
}

std::vector<std::string> get_lines(std::string file_name) {
	std::vector<std::string> vec;
	vec.reserve(get_lines_num(file_name));
	std::fstream fl;
	fl.open(file_name, std::ios_base::in);
	if (!fl.is_open()) {
		std::cout << "Error in opening \"" << file_name << "\" file.\n";
		std::exit(1);
	}
	if (fl.peek() == std::ifstream::traits_type::eof()) {
		std::cout << "File \"" << file_name << "\" is empty.\n";
	}
	while (!fl.eof()) {
		std::string str;
		std::getline(fl, str);
		if (str.size()) vec.push_back(str);
	}
	return vec;
}


void download_array_files(std::vector<std::string> array) {
	#pragma omp parallel for firstprivate(array) schedule(dynamic)
	for (std::string url: array) {
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

void download_from_file(std::string file_name) {
	std::vector<std::string> vec = get_lines(file_name);
	download_array_files(vec);
	vec.clear();
}