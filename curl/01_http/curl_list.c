#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

/**
	两个curl_slist是否会覆盖
*/
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	return fwrite(ptr, size, nmemb, userdata);
}

int main(int argc, char *argv[])
{
	CURL *curl;
	CURLcode res;

	struct curl_slist *slist_1 = NULL;
	struct curl_slist *slist_2 = NULL;
	slist_1 = curl_slist_append(slist_1, "Range: bytes=1000000-");
	slist_2 = curl_slist_append(slist_2, "Host: nginx.org:80");
	
	FILE* pfile = fopen("./nginx-1.14.0.tar.gz", "wb");
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "http://nginx.org/download/nginx-1.14.0.tar.gz");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, pfile);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist_1);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist_2);
				
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		else
			fprintf(stdout, "perform OK!\n");
		curl_easy_cleanup(curl);
		curl_slist_free_all(slist_1);
		curl_slist_free_all(slist_2);
	}
	return 0;
}
/**
  规范的 curl_slist 用法：
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
 
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.example.com");
        struct curl_slist *headers = NULL;
 
        //增加HTTP header
        headers = curl_slist_append(headers, "Accept:application/json");
        headers = curl_slist_append(headers, "Content-Type:application/json");
        headers = curl_slist_append(headers, "charset:utf-8");
 
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "post的内容");
 
        static const char *pagefilename = "respone.txt";
        FILE *pagefile;
 
        //写入到文件
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
 
        fopen_s(&pagefile,pagefilename, "wb");
        if (pagefile) 
        {
 
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);        
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            }
            fclose(pagefile);
 
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

 */