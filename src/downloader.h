#include <stdio.h>
#include <stdbool.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
bool download_image(const char* url, const char* outfilename);