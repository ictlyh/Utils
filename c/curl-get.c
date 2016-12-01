/*
 * Demo of getting page via lib crul
 * Build: gcc curl-get.c -o curl-get -lcurl
 * Run: ./curl-get
 */

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct url_data {
  size_t size;
  char *data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
  size_t n = size * nmemb;
  free(data->data);
  data->data = (char *)malloc(sizeof(char) * (n + 1));
  if (!data->data) {
    fprintf(stdout, "Failed to allocate memory\n");
    return 0;
  }
  memcpy(data->data, ptr, n);
  data->data[n] = '\0';
  return n;
}

char *handleURL(const char *url) {
  CURL *curl;
  CURLcode res;
  struct url_data data;
  data.size = 0;
  data.data = NULL;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stdout, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  }
  return data.data;
}

int main() {
  char url[] = "https://www.baidu.com";
  char *data = handleURL(url);
  if (!data) {
    fprintf(stdout, "Curl handle url error\n");
    return 1;
  }
  fprintf(stdout, "Content of page %s:\n%s\n", url, data);
  return 0;
}
