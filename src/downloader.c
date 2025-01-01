#include "downloader.h"


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	return fwrite(ptr, size, nmemb, stream);
}

bool download_image(const char* url, const char* outfilename) {
	CURL *curl = curl_easy_init();
	if (curl) {
		FILE *fp = fopen(outfilename, "wb");
		if (fp == NULL) {
			curl_easy_cleanup(curl);
			return false;
		}		
		if (curl_easy_setopt(curl, CURLOPT_URL, url)) {
			curl_easy_cleanup(curl);
			fclose(fp);
			return false;
		}
		if (curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6L)) {
			curl_easy_cleanup(curl);
			fclose(fp);
			return false;
		}
		if (curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)) {
			curl_easy_cleanup(curl);
			fclose(fp);
			return false;
		}
		if (curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L)) {
			curl_easy_cleanup(curl);
			fclose(fp);
			return false;
		}
		if (curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data)) {
			curl_easy_cleanup(curl);
			fclose(fp);
			return false;
		}
		if (curl_easy_setopt(curl, CURLOPT_FILE, fp)) {
			curl_easy_cleanup(curl);
			fclose(fp);
			return false;
		}
		if (curl_easy_perform(curl)) {
			curl_easy_cleanup(curl);
			fclose(fp);
			return false;
		}
		curl_easy_cleanup(curl);
		fclose(fp);
		return true;
	}
	return false;
}
