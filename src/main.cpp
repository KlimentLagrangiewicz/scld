#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#endif
#endif

#include <iostream>
#include <string>
#include <stdexcept>
#include <span>
#include <algorithm>



#include "range.hpp"
#include "txtproc.hpp"
#include "argparser.hpp"

#ifndef SCLD_VERSION
#define SCLD_VERSION "2025.10.14"
#endif

#ifndef help_txt
#define help_txt "Simple command-line downloader (scld) is elementary command line application for downloading:\n\
  1. Homogeneous files from URLs with format like:\
`shared URL part`k.fmt, `shared URL part`(k+1).fmt, `shared URL part`(k+2).fmt, `shared URL part`..., `shared URL part`(N-1).fmt, `shared URL part`N.fmt.\n\
  2. From a file contained the addresses of the files line by line.\n\
  3. Directly file by providing a full address.\n\
Options:\n\
  -h or --help\t\tDisplay this information.\n\
  -r or --range\t\tAfter that option specify arguments: shared part of URL, file format, first and last indexes.\n\
  -t or --txt\t\tAfter that option specify arguments: text filename with full URLs written line by line.\n\
  -f or --file\t\tAfter that option specify full URLs downloading files separated by space.\n\
  -c or --cin\t\tDownloading files from URLs provided via standard input.\n\
  -v or --version\tDisplay version information.\n"
#endif


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
			
			size_t pos;
			const auto &vec = getFilesURLs(const_cast<const char**>(argv), 2, argc, pos);
			
			const auto &vec_s = vec.size();
			
			downloadFromStringArray(std::span<const std::string>(vec.data(), std::min(pos, vec_s)));
			if (pos < vec_s) downloadFromStringArraySerialy(std::span<const std::string>(vec.data() + pos, vec_s));
			
		} else if (flag == "-t" || flag == "--txt") {
			if (argc < 3) throw std::runtime_error("Not enough parameters for \"txt\" option.\n");
			
			downloadFromFile(*(argv + 2));
			
		} else if (flag == "-h" || flag == "--help") {
			std::cout<< help_txt;
		} else if (flag == "-c" || flag == "--cin") {
			downloadFromInput(std::cin);
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
