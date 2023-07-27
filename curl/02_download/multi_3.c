#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define MULTI_PERFORM_HANG_TIMEOUT 60 * 1000
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

static struct timeval tvnow(void)
{
	struct timeval now;
	now.tv_sec = (long)time(NULL);
	now.tv_usec = 0;

	return now;
}

static long tvdiff(struct timeval newer, struct timeval older)
{
	return (newer.tv_sec - older.tv_sec) * 1000 + (newer.tv_usec - older.tv_usec) / 1000;
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

static void easys_clear(CURLM* cm, const int still_running)
{	
	CURLMsg *msg;
	int msgs_left;
	CURL *easy;
	CURLcode res;
	
	while((msg = curl_multi_info_read(cm, &msgs_left)))
	{
		if(msg->msg == CURLMSG_DONE)
		{
			easy = msg->easy_handle;
			res = msg->data.result;
			curl_multi_remove_handle(cm, easy);
			curl_easy_cleanup(easy);
		}
		else if (still_running)
		{
			fprintf("%s::%s %d select error\n", __FILE__, __FUNCTION__, __LINE__);
		}
	}
}

static void multi(CURLM* cm)
{
	struct timeval mp_start;
	mp_start = tvnow();
	int still_running = 0;
	int flag = 0;		//超时标志 1-超时中
	//有没有easy
    curl_multi_perform(cm, &still_running);
    while (still_running) 
	{
		struct timeval timeout;
		fd_set fdread;
		fd_set fdwrite;
		fd_set fdexcep;
		int maxfd = -1;
		long curl_timeo = -1;

		//必须的初始化
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexcep);

		//备用超时值
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		//获取libcurl提供的超时，此超时用作select的超时
		curl_multi_timeout(cm, &curl_timeo);
		if (curl_timeo >= 0) 
		{
			timeout.tv_sec = curl_timeo / 1000;
			if (timeout.tv_sec > 1)
				timeout.tv_sec = 1;
			else
				timeout.tv_usec = (curl_timeo % 1000) * 1000;
		}

		//获取multi会话中的描述符集
		CURLMcode rcm = curl_multi_fdset(cm, &fdread, &fdwrite, &fdexcep, &maxfd);
		check_curlm_code(rcm);

		int rc;
		if (maxfd == -1) 
		{
			//select可能在做其他的事情，100ms之后再select。如果还没，run forever
			struct timeval wait = { 0, 100 * 1000 };
			rc = select(0, NULL, NULL, NULL, &wait);
		}
		else
		{
			rc = select(maxfd + 1, &fdread, NULL, NULL, &timeout);
		}

		if (tvdiff(tvnow(), mp_start) > MULTI_PERFORM_HANG_TIMEOUT) 
		{
			fprintf(stderr, "ABORTING: Since it seems that we would have run forever.\n");
			break;
		}

		//超时也要perform，明确是select还是easy的问题
		switch(rc)
		{
			case -1:
				if (flag == 0)
				{
					mp_start = tvnow();
					flag = 1;
				}
				break;
			case 0:
				curl_multi_perform(cm, &still_running);
				if (flag == 0)
				{
					mp_start = tvnow();
					flag = 1;
				}
				break;	
			default:
				curl_multi_perform(cm, &still_running);
				flag = 0;
				break;
		}
    }
	
	easys_clear(cm, still_running);
}

/*
	关键函数：
		curl_multi_fdset
		select		//只能监控1024个描述符
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
    curl_global_cleanup();

    return 0;
}