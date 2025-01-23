#ifndef DOWNLOADER_H_
#define DOWNLOADER_H_

#include <stdio.h>
#include <stdbool.h>
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
bool download_file(const char* const url, const char* const outfilename);

#ifdef __cplusplus
}
#endif

#endif