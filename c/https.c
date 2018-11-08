/**
 *	curl demo with https
 *
 *	Build: gcc https.c -lcurl
 *	Run: ./a.out URL
 */
#include <stdio.h>
#include <curl/curl.h>
 
int main(int argc, char** argv)
{
	CURL *curl;
	CURLcode res;
    const char* url = argv[1];
    char errbuff[CURL_ERROR_SIZE];
	if (CURLE_OK == curl_global_init(CURL_GLOBAL_DEFAULT))
	{
		printf("curl_global_init done\n");
	}
	else
	{
		printf("curl_global_init fail\n");
	}

	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		printf("curl_easy_setopt CURLOPT_URL %s done\n", url);

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		printf("curl_easy_setopt CURLOPT_CONNECTTIMEOUT done\n");

		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		printf("curl_easy_setopt CURLOPT_TIMEOUT done\n");

		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuff);
		printf("curl_easy_setopt CURLOPT_ERRORBUFFER done\n");

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		printf("curl_easy_setopt CURLOPT_SSL_VERIFYPEER done\n");

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		printf("curl_easy_setopt CURLOPT_SSL_VERIFYHOST done\n");

		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		printf("curl_easy_perform done\n");
		/* Check for errors */ 
		if(res != CURLE_OK)
		{
			printf("curl_easy_perform() failed(%d): %s\n", res, curl_easy_strerror(res));
			printf("detail error msg:%s\n", errbuff);
		}

		/* always cleanup */ 
		curl_easy_cleanup(curl);
		printf("curl_easy_cleanup done\n");
	}
	else
	{
		printf("curl is null\n");
	}

	curl_global_cleanup();
	printf("curl_global_cleanup done\n");

	return 0;
}
