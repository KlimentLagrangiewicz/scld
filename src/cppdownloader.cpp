#include "cppdownloader.hpp"


std::string getOutputFileName(const std::string inputFileName) {
	if (!std::filesystem::exists(inputFileName)) return inputFileName;
	const std::string::size_type dotPosition = inputFileName.rfind('.');
	std::string base = inputFileName.substr(0, dotPosition), fmt = inputFileName.substr(dotPosition + 1);
	size_t i = 1;
	while (std::filesystem::exists(base + " (" + std::to_string(i) + ")." + fmt)) i++;
	return base + " (" + std::to_string(i) + ")." + fmt;
}

bool cppDownloadFile(const std::string fileURL, const std::string oFileName) {
	return download_file(fileURL.c_str(), oFileName.c_str());
}

std::string getName(const std::string url) {
	return url.substr(url.rfind('/') + 1);
}

void fileDownload(const std::string fileURL) {
	std::string oFileName = getName(fileURL);
	oFileName = getOutputFileName(oFileName);
	if (!cppDownloadFile(fileURL, oFileName)) {
		std::cout << "Error during downloading file " << oFileName << " from site: " << fileURL << '\n';
		try {
			if (std::filesystem::remove(oFileName))
				std::cout << "Artefact file " << oFileName << " deleted\n";
		} catch (const std::filesystem::filesystem_error& err) {
			std::cout << "Filesystem error: " << err.what() << '\n';
		}
	}
}

void downloadFromStringArray(const std::vector<std::string> array) {
	std::for_each(std::execution::par_unseq, array.begin(), array.end(), fileDownload);
}
