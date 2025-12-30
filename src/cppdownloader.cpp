#include "cppdownloader.hpp"


static std::string getOutputFileName(const std::string &inputFileName) {
	if (!std::filesystem::exists(inputFileName)) return inputFileName;
	const auto dotPosition = inputFileName.rfind('.');
	const auto base = inputFileName.substr(0, dotPosition) + " (", fmt = ")." + inputFileName.substr(dotPosition + 1);
	size_t i = 1;
	while (std::filesystem::exists(base + std::to_string(i) + fmt)) ++i;
	return base + std::to_string(i) + fmt;
}


static std::string getOutputFileName(const int n, const int maxWidth, const std::string& suffixus) {
	const auto n_str = std::to_string(n);
	const auto n_size = n_str.size();
	const auto max_width = static_cast<std::string::size_type>(maxWidth);
	
	const auto base_size = (n_size > max_width ? n_size : max_width) + suffixus.size();
	
	std::string base_name;
	base_name.reserve(base_size);
	
	if (n_size > max_width) {
		base_name.append(n_str);
	} else {
		base_name.append(max_width - n_size, '0');
		base_name.append(n_str);
	}
	
	base_name.append(suffixus);
	
	if (!std::filesystem::exists(base_name)) return base_name;
	
	const auto prefix_size = n_size > max_width ? n_size : max_width;
	std::string prefix;
	prefix.reserve(prefix_size + 2);
	prefix.append(base_name, 0, prefix_size);
	prefix.append(" (");
	
	const auto suffix_size = suffixus.size() + 2;
	const auto total_base_size = prefix.size() + suffix_size;
	
	std::array<char, 21> num_buf{};
	
	for (std::string::size_type i = 1; i < std::numeric_limits<std::string::size_type>::max(); ++i) {
		std::string candidate;
		candidate.reserve(total_base_size + 14);
		candidate.append(prefix);
		
		const auto [ptr, ec] = std::to_chars(num_buf.data(), num_buf.data() + num_buf.size(), i);
		if (ec == std::errc()) {
			candidate.append(num_buf.data(), ptr);
		} else {
			candidate.append(std::to_string(i));
		}
		
		candidate.append(")");
		candidate.append(suffixus);
		
		if (!std::filesystem::exists(candidate)) return candidate;
	}
	
	return base_name;
}

static inline bool cppDownloadFile(const std::string &fileURL, const std::string &oFileName) {
	return download_file(fileURL, oFileName);
}

std::string getName(const std::string &url) {
	if (url.empty()) return "";
	
	auto scheme_pos = url.find("://");
	size_t start_search = 0;
	
	if (scheme_pos != std::string::npos) start_search = scheme_pos + 3;
	
	auto fragment_pos = url.find('#', start_search);
	auto query_pos = url.find('?', start_search);
	
	auto path_end = std::min(
		fragment_pos != std::string::npos ? fragment_pos : url.size(),
		query_pos != std::string::npos ? query_pos : url.size()
	);
	
	auto pos = url.rfind('/', path_end);
	
	if (pos == std::string::npos)
		return start_search == 0 ? url.substr(0, path_end) : url.substr(start_search, path_end - start_search);
	
	auto name_start = pos + 1;
	if (name_start >= path_end) return "";
	
	return url.substr(name_start, path_end - name_start);
}

static void printError(const std::string &fileURL, const std::string &oFileName) {
	std::cout << "Error during downloading file " << oFileName << " from site: " << fileURL << '\n';
	try {
		if (std::filesystem::remove(oFileName)) std::cout << "Artefact file " << oFileName << " deleted\n";
	} catch (const std::filesystem::filesystem_error& err) {
		std::cout << "Filesystem error: " << err.what() << '\n';
	}
}

void fileDownload(const std::string &fileURL) {
	const auto oFileName = getOutputFileName(getName(fileURL));
	if (cppDownloadFile(fileURL, oFileName)) {
		static tbb::spin_mutex mtx;
		tbb::spin_mutex::scoped_lock lock(mtx);
		printError(fileURL, oFileName);
	}
}

void fileDownload(const std::string &fileURL, const int n, const int maxWidth, const std::string & suffixus) {
	const auto oFileName = getOutputFileName(n, maxWidth, suffixus);
	if (cppDownloadFile(fileURL, oFileName)) {
		static tbb::spin_mutex mtx;
		tbb::spin_mutex::scoped_lock lock(mtx);
		printError(fileURL, oFileName);
	}
}

void fileDownloadSilently(const std::string &fileURL) {
	const auto oFileName = getOutputFileName(getName(fileURL));
	if (cppDownloadFile(fileURL, oFileName)) 
		std::filesystem::remove(oFileName);
}

void fileDownloadSilently(const std::string &fileURL, const int n, const int maxWidth, const std::string & suffixus) {
	const auto oFileName = getOutputFileName(n, maxWidth, suffixus);
	
	if (cppDownloadFile(fileURL, oFileName))
		std::filesystem::remove(oFileName);

}

void downloadFromStringArray(const std::span<const std::string> array) {
	tbb::parallel_for_each(
		array.begin(),
		array.end(),
		[](const auto & i) {
			fileDownload(i);
		}
	);
}

void downloadFromStringArraySerialy(const std::span<const std::string> array) {
	for (const auto &i: array) fileDownload(i);
}
