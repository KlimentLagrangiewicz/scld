#include <iostream>
#include <string>
#include <filesystem>

#include "downloader.h"
#include "range.hpp"
#include "txtproc.hpp"

#define help_txt "Simple command-line downloader (scld) is elementary command line application for downloading:\n\
1. Homogeneous files from URLs with format like:\
`URL`k.fmt, `URL`(k+1).fmt, `URL`(k+2).fmt, `URL`..., `URL`(N-1).fmt, `URL`N.fmt;\n\
2. From a file contained the addresses of the files line by line;\n\
3. Directly file by providing a full address.\n\
Options:\n\
-r or --range\tAfter that option specify arguments: shared part of URL, file format, first and last indexes;\n\
-t or --txt\tAfter that option specify arguments: filename with full URLs written line by line;\n\
-f or --file\tAfter that option specify full URL to downloading file;\n\
-h or --help\tDisplay main information about the software and specific types of command line options."

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Not enough parameters.\n";
		std::exit(1);
	}
	try {
		std::string flag(*(argv + 1));
		if (flag == "-r" || flag == "--range") {
			if (argc < 5) {
				std::cout << "Not enough parameters.\n";
				std::exit(1);
			}
			std::string path(*(argv + 2)), fmt(*(argv + 3));	
			download_range(path, fmt, std::string(argv[4]), std::string(argv[5]));
		} else if (flag == "-f" || flag == "--file") {
			if (argc < 3) {
				std::cout << "Not enough parameters.\n";
				std::exit(1);
			}
			std::string url(*(argv + 2));
			try {
				bool res = download_image(url.c_str());
				if (!res) {
					try {
						std::string name = get_name(url);
						std::cout << "Error during downloading file " << name << ".\n";
						if (std::filesystem::remove(name))
							std::cout << "File " << name << " deleted.\n";
						else
							std::cout << "File " << name << " not found.\n";
					} catch(const std::filesystem::filesystem_error& err) {
						std::cout << "Filesystem error: " << err.what() << ".\n";
					}
				}
			} catch (...) {
				try {
					std::string name = get_name(url);
					std::cout << "Error during downloading file " << name << ".\n";
					if (std::filesystem::remove(name))
						std::cout << "File " << name << " deleted.\n";
					else
						std::cout << "File " << name << " not found.\n";
				} catch(const std::filesystem::filesystem_error& err) {
					std::cout << "Filesystem error: " << err.what() << ".\n";
				}
			}
		} else if (flag == "-t" || flag == "--txt") {
			if (argc < 3) {
				std::cout << "Not enough parameters.\n";
				std::exit(1);
			}
			std::string file_name(*(argv + 2));
			download_from_file(file_name);
		} else if (flag == "-h" || flag == "--help"){		
			std::cout<< help_txt << '\n';
		} else {
			std::cout << "Unknown option \"" + flag + "\".\n";
		}
	} catch (...) {
		std::cout << "Something went wrong.\n"; 
	}
	return 0;
}
