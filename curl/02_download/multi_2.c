#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <curl/multi.h>

#define MAX_WAIT_MSECS 10*1000 /* Wait max. 10 seconds */

#define CNT 5

static const char *urls[] = {
	"http://nginx.org/download/nginx-1.14.0.tar.gz",
	"http://nginx.org/download/nginx-1.12.2.tar.gz",
	"http://nginx.org/download/nginx-1.10.3.tar.gz",
	"http://nginx.org/download/nginx-1.8.1.tar.gz",
	"http://nginx.org/download/nginx-1.6.3.tar.gz"
};

static size_t cb(char *d, size_t n, size_t l, void *p)
{
  return fwrite(d, n, l, (FILE*)p);
}

static void init(CURLM *cm, int i)
{
	char tmp_chars[1024];
	strcpy(tmp_chars, urls[i]);
	char *file_name = strrchr(tmp_chars, 47) + 1;
	char dd_path[1024];
	strcpy(dd_path, "./");
	strcat(dd_path, file_name);
	
	FILE* pFile = fopen(dd_path, "wb");
	
	CURL *eh = curl_easy_init();
	curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(eh, CURLOPT_WRITEDATA, pFile);
	curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
	curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
	curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[i]);
	// curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
	curl_multi_add_handle(cm, eh);
}

//检验码
static void check_curlm_code(CURLMcode rcm)
{
	if (rcm != CURLM_OK)
	{
		fprintf(stderr, "%s::%s %d failed (errorcode: %d) %s\n", __FILE__, __FUNCTION__, __LINE__, rcm, curl_multi_strerror(rcm));
		exit(1);
	}
}

static void multi(CURLM* cm)
{
	int still_running = 0;
	do {
		/*
			超时时间内等待，如果有感兴趣的事件发生（这里是可读），执行write_cb
			等待socket描述符的动作，可读时调用write_cb
		*/
		int numfds=0;
		int rcm = curl_multi_wait(cm, NULL, 0, MAX_WAIT_MSECS, &numfds);
		check_curlm_code(rcm);
		
		//是否仍有easy运行，如果有继续循环
		curl_multi_perform(cm, &still_running);
	} while(still_running);

	CURLMsg *msg=NULL;
	int msgs_left=0;
	//没有easy说明完成或出现错误，具体原因...
	while ((msg = curl_multi_info_read(cm, &msgs_left))) 
	{
		if (msg->msg == CURLMSG_DONE)
		{
			CURL* eh = msg->easy_handle;
			CURLcode rce = msg->data.result;
			if(rce != CURLE_OK) 
			{
				fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, curl_easy_strerror(rce));
				continue;
			}

			int http_status_code=0;
			char* szUrl = NULL;

			curl_easy_getinfo(eh, CURLINFO_RESPONSE_CODE, &http_status_code);
			curl_easy_getinfo(eh, CURLINFO_PRIVATE, &szUrl);

			if(http_status_code == 200) 
			{
				printf("200 OK for %s\n", szUrl);
			} 
			else 
			{
				fprintf(stderr, "GET of %s returned http status code %d\n", szUrl, http_status_code);
			}

			curl_multi_remove_handle(cm, eh);
			curl_easy_cleanup(eh);
		}
		else 
		{
			fprintf(stderr, "error: after curl_multi_info_read(), CURLMsg=%d\n", msg->msg);
		}
	}	
}

/*
	关键函数
		curl_multi_perform
		curl_multi_wait
 */
int main(void)
{
    curl_global_init(CURL_GLOBAL_ALL);

    CURLM* cm = curl_multi_init();

    for (int i = 0; i < CNT; ++i) 
	{
        init(cm, i);
    }

	multi(cm);
	
    curl_multi_cleanup(cm);

    return 0;
}