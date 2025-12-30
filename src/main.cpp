#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	
	#include <winsock2.h>
	#include <io.h>
	
	#ifndef isatty
		#define isatty _isatty
	#endif
	
	#ifndef fileno
		#define fileno _fileno
	#endif
#else
	#include <unistd.h>
#endif

#include <iostream>
#include <string>
#include <stdexcept>
#include <span>
#include <algorithm>

#include "range.hpp"
#include "txtproc.hpp"

#ifndef SCLD_VERSION
	#define SCLD_VERSION "v2025.12.30"
#endif

#ifndef help_txt
#define help_txt "Simple command-line downloader (scld) is elementary command line application for downloading:\n\
  1. Homogeneous files from URLs with format like:\
`shared URL part`k.fmt, `shared URL part`(k+1)`suffix`, `shared URL part`(k+2)`suffix`, `shared URL part`..., `shared URL part`(N-1)`suffix`, `shared URL part`N`suffix`.\n\
  2. From a file contained the addresses of the files line by line.\n\
  3. Directly file by providing a full address.\n\
Options:\n\
  -h or --help\t\tDisplay this information.\n\
  -r or --range\t\tAfter that option specify arguments: shared part of URL, shared suffix, first and last indexes.\n\
  -t or --txt\t\tDownloads files from URLs provided as command-line arguments (fstarting from the second one) and standard input.\n\
  -f or --file\t\tDownloads files from URLs listed in text files specified via command-line arguments (from the second onward) and standard input.\n\
  -v or --version\tDisplay version information.\n"
#endif


static inline bool hasStdinData() {
	#if defined(_WIN32) || defined(_WIN64)
		return !_isatty(_fileno(stdin));
	#else
		return !isatty(STDIN_FILENO);
	#endif
}



int main(int argc, char **argv)
{
	try {
		if (argc < 2) throw std::runtime_error("Not enough parameters.\n");
		const std::string flag(*(argv + 1));
		if (flag == "-r" || flag == "--range") {
			if (argc < 5) throw std::runtime_error("Not enough parameters for \"range\" option.\n");
			
			const std::vector<std::string> fmts = getSuffixes(*(argv + 3));
			downloadFromRange(*(argv + 2), fmts, std::string(argv[4]), std::string(argv[5]));
			
		} else if (flag == "-f" || flag == "--file") {
			
			if (hasStdinData()) {
				downloadFromFile(std::cin, const_cast<const char**>(argv), 2, argc);
			} else {
				downloadFromFile(const_cast<const char**>(argv), 2, argc);
			}
			
		} else if (flag == "-t" || flag == "--txt") {
			
			if (hasStdinData()) {
				downloadFromInput(std::cin, const_cast<const char**>(argv), 2, argc);
			} else {
				downloadFromInput(const_cast<const char**>(argv), 2, argc);
			}
			
		} else if (flag == "-h" || flag == "--help") {
			std::cout<< help_txt;
		} else if (flag == "-v" || flag == "--version") {
			std::cout << "scld " << SCLD_VERSION << '\n';
		} else {
			throw std::runtime_error("Unknown option \"" + flag + "\" \n");
		}
	} catch (const std::exception& e) {
		std::cerr << "Error occurred: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Something went wrong\n"; 
	}
}
