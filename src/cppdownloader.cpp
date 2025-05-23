#include "cppdownloader.hpp"


std::string getOutputFileName(const std::string &inputFileName) {
	if (!std::filesystem::exists(inputFileName)) return inputFileName;
	const std::string::size_type dotPosition = inputFileName.rfind('.');
	const std::string base = inputFileName.substr(0, dotPosition) + " (", fmt = ")." + inputFileName.substr(dotPosition + 1);
	size_t i = 1;
	while (std::filesystem::exists(base + std::to_string(i) + fmt)) i++;
	return base + std::to_string(i) + fmt;
}

bool cppDownloadFile(const std::string &fileURL, const std::string &oFileName) {
	return download_file(fileURL.c_str(), oFileName.c_str());
}

std::string getName(const std::string &url) {
	return url.substr(url.rfind('/') + 1);
}

void printError(const std::string &fileURL, const std::string &oFileName) {
	std::cout << "Error during downloading file " << oFileName << " from site: " << fileURL << '\n';
	try {
		if (std::filesystem::remove(oFileName))
			std::cout << "Artefact file " << oFileName << " deleted\n";
	} catch (const std::filesystem::filesystem_error& err) {
		std::cout << "Filesystem error: " << err.what() << '\n';
	}
}

void fileDownload(const std::string &fileURL) {
	const std::string oFileName = getOutputFileName(getName(fileURL));
	if (!cppDownloadFile(fileURL, oFileName)) {
		static tbb::spin_mutex mtx;
		tbb::spin_mutex::scoped_lock lock(mtx);
		printError(fileURL, oFileName);
	}
}

void downloadFromStringArray(const std::vector<std::string> &array) {
	tbb::parallel_for_each(array.begin(), array.end(), fileDownload);
}
