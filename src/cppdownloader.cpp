#include "cppdownloader.hpp"


static std::string getOutputFileName(const std::string &inputFileName) {
	if (!std::filesystem::exists(inputFileName)) return inputFileName;
	const std::string::size_type dotPosition = inputFileName.rfind('.');
	const std::string base = inputFileName.substr(0, dotPosition) + " (", fmt = ")." + inputFileName.substr(dotPosition + 1);
	size_t i = 1;
	while (std::filesystem::exists(base + std::to_string(i) + fmt)) ++i;
	return base + std::to_string(i) + fmt;
}


static std::string getOutputFileName(const int n, const int maxWidth, const std::string & fmt) {
	const std::string n_str = std::to_string(n);
	
	const auto n_size = n_str.size();
	const auto max_width = static_cast<std::string::size_type>(maxWidth);
	
	std::string inputFileName;
	if (n_size > max_width) {
		inputFileName.reserve(n_size + fmt.size() + 1);
		inputFileName.append(n_str);
		inputFileName.push_back('.');
		inputFileName.append(fmt);
	} else {
		inputFileName.reserve(max_width + fmt.size() + 1);
		inputFileName.append(max_width - n_size, '0');
		inputFileName.append(n_str);
		inputFileName.push_back('.');
		inputFileName.append(fmt);
	}
	
	if (!std::filesystem::exists(inputFileName)) return inputFileName;
	
	const auto dotPosition = n_size > max_width ? n_size : max_width;
	
	std::string base;
	base.reserve(dotPosition + 2);	
	base.insert(0, inputFileName, 0, dotPosition);
	base.append(" (");
	
	std::string last_part;
	last_part.reserve(fmt.size() + 2);
	last_part.append(").");
	last_part.append(fmt);
	
	size_t i = 1;
	while (std::filesystem::exists(base + std::to_string(i) + last_part)) ++i;
	return base + std::to_string(i) + last_part;
}


static inline bool cppDownloadFile(const std::string &fileURL, const std::string &oFileName) {
	return download_file(fileURL, oFileName);
}

static inline std::string getName(const std::string &url) {
	return url.substr(url.rfind('/') + 1);
}

static void printError(const std::string &fileURL, const std::string &oFileName) {
	std::cout << "Error during downloading file " << oFileName << " from site: " << fileURL << '\n';
	try {
		if (const bool fexists = std::filesystem::exists(oFileName); fexists && std::filesystem::remove(oFileName))
			std::cout << "Artefact file " << oFileName << " deleted\n";
	} catch (const std::filesystem::filesystem_error& err) {
		std::cout << "Filesystem error: " << err.what() << '\n';
	}
}

static inline void deleteingFile(const std::string &fn) {
	if (std::filesystem::exists(fn)) std::filesystem::remove(fn);
}

void fileDownload(const std::string &fileURL) {
	const std::string &oFileName = getOutputFileName(getName(fileURL));
	if (cppDownloadFile(fileURL, oFileName)) {
		static tbb::spin_mutex mtx;
		tbb::spin_mutex::scoped_lock lock(mtx);
		printError(fileURL, oFileName);
	}
}

void fileDownload(const std::string &fileURL, const int n, const int maxWidth, const std::string & fmt) {
	const std::string &oFileName = getOutputFileName(n, maxWidth, fmt);
	if (cppDownloadFile(fileURL, oFileName)) {
		static tbb::spin_mutex mtx;
		tbb::spin_mutex::scoped_lock lock(mtx);
		printError(fileURL, oFileName);
	}
}

void fileDownloadSilently(const std::string &fileURL) {
	const std::string &oFileName = getOutputFileName(getName(fileURL));
	if (cppDownloadFile(fileURL, oFileName)) {
		//static tbb::spin_mutex mtx;
		//tbb::spin_mutex::scoped_lock lock(mtx);
		deleteingFile(oFileName);
	}
}

void fileDownloadSilently(const std::string &fileURL, const int n, const int maxWidth, const std::string & fmt) {
	const std::string &oFileName = getOutputFileName(n, maxWidth, fmt);
	if (cppDownloadFile(fileURL, oFileName)) {
		//static tbb::spin_mutex mtx;
		//tbb::spin_mutex::scoped_lock lock(mtx);
		deleteingFile(oFileName);
	}
}


void downloadFromStringArray(const std::span<const std::string> array) {
	tbb::parallel_for_each(
		array.begin(),
		array.end(),
		[](const std::string & i) {
			fileDownload(i);
		}
	);
}
