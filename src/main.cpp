#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#endif
#endif

#include <iostream>
#include <string>
#include <stdexcept>


#include "range.hpp"
#include "txtproc.hpp"
#include "argparser.hpp"

#define help_txt "Simple command-line downloader (scld) is elementary command line application for downloading:\n\
1. Homogeneous files from URLs with format like:\
`URL`k.fmt, `URL`(k+1).fmt, `URL`(k+2).fmt, `URL`..., `URL`(N-1).fmt, `URL`N.fmt;\n\
2. From a file contained the addresses of the files line by line;\n\
3. Directly file by providing a full address.\n\
Options:\n\
-r or --range\tAfter that option specify arguments: shared part of URL, file format, first and last indexes;\n\
-t or --txt\tAfter that option specify arguments: filename with full URLs written line by line;\n\
-f or --file\tAfter that option specify full URL to downloading file;\n\
-h or --help\tDisplay main information about the software and specific types of command line options.\n"


int main(int argc, char **argv)
{
	try {
		if (argc < 2) throw std::runtime_error("Not enough parameters.\n");
		const std::string flag(*(argv + 1));
		if (flag == "-r" || flag == "--range") {
			if (argc < 5) throw std::runtime_error("Not enough parameters for \"range\" option.\n");
			
			const std::vector<std::string> fmts = getFormats(*(argv + 3));
			downloadFromRange(*(argv + 2), fmts, std::string(argv[4]), std::string(argv[5]));
		} else if (flag == "-f" || flag == "--file") {
			if (argc < 3) throw std::runtime_error("Not enough parameters for \"file\" option.\n");
			
			const auto &vec = getFilesURLs(const_cast<const char**>(argv), 2, argc);
			downloadFromStringArray(vec);
		} else if (flag == "-t" || flag == "--txt") {
			if (argc < 3) throw std::runtime_error("Not enough parameters for \"txt\" option.\n");
			
			downloadFromFile(*(argv + 2));
		} else if (flag == "-h" || flag == "--help"){
			std::cout<< help_txt;
		} else {
			throw std::runtime_error("Unknown option \"" + flag + "\" \n");
		}
	} catch (const std::exception& e) {
		std::cerr << "Error occurred: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Something went wrong\n"; 
	}
}
