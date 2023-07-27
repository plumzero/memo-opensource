#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#  include <unistd.h>
#endif
#include <curl/multi.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <sys/poll.h>
#include <event2/event.h>
#include <fcntl.h>
#include <sys/stat.h>

//								HAVE AN ERROR！！！！！！！！！！！！！！！！！！

/* 宏 */
#define NUM_FIVE 5
#define _DEBUG 1
/* 全局变量 */
CURLM *g_curlm;
struct event_base *g_evbase;
struct event *g_evtimer;
typedef struct _Download_Des_Info
{
	int sockfd;				//描述符		为了分明，在这里将sockfd定为int，而不是curl_socket_t
	FILE *pFile;			//对应文件流指针
	struct event*	event;	//所处事件
	char url[1024];			//下载源url
} DDInfo;

DDInfo g_info[NUM_FIVE];
char *g_paths[NUM_FIVE];
char *g_files[NUM_FIVE];


static char *g_url_list[NUM_FIVE] = {
	"http://nginx.org/download/nginx-1.14.0.tar.gz",
	"http://nginx.org/download/nginx-1.12.2.tar.gz",
	"http://nginx.org/download/nginx-1.10.3.tar.gz",
	"http://nginx.org/download/nginx-1.8.1.tar.gz",
	"http://nginx.org/download/nginx-1.6.3.tar.gz",
};
/* 普通函数 */
int multi_opt();
int create_fds();
int create_easys();
int create_events();
int download_dir_file();
int init_DDInfo();
/* 回调函数 */
static int multi_socket_callback(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp);
static void event_callback(int fd, short event, void *arg);
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
static void timer_callback(int fd, short kind, void *userp);
int multi_timer_callback(CURLM *multi, long timeout_ms, void *userp);

int main()
{	
	/* 初始化 */
	memset(&g_info, 0, sizeof(DDInfo) * NUM_FIVE);
	
	struct event_config *cfg = event_config_new();
	event_config_avoid_method(cfg, "epoll");
	g_evbase = event_base_new_with_config(cfg);  
	event_config_free(cfg);
	
	curl_global_init(CURL_GLOBAL_ALL);
	g_curlm = curl_multi_init();
	g_evtimer = evtimer_new(g_evbase, timer_callback, NULL);
	
	multi_opt();
	download_dir_file();
	init_DDInfo();
	begin_unsysnc();
	
	event_base_dispatch(g_evbase);
	
	curl_multi_cleanup(g_curlm);
	curl_global_cleanup();

	return 0;
}

//创建 multi 会话	成功返回0
int multi_opt()
{	
	printf("execute %s::%s %d\n", __FILE__, __FUNCTION__, __LINE__);
	curl_multi_setopt(g_curlm, CURLMOPT_TIMERFUNCTION, multi_timer_callback);
	curl_multi_setopt(g_curlm, CURLMOPT_TIMERDATA, NULL);
	curl_multi_setopt(g_curlm, CURLMOPT_SOCKETFUNCTION, multi_socket_callback);
	curl_multi_setopt(g_curlm, CURLMOPT_SOCKETDATA, NULL);
	
	return 0;
}

//成功返回0
int init_DDInfo()
{	
	printf("execute %s::%s %d\n", __FILE__, __FUNCTION__, __LINE__);
	for(int i = 0; i < NUM_FIVE; i++)
	{
		FILE *pFile = fopen(g_paths[i], "wb+");
		if (pFile == NULL)
		{
			fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
			exit(-1);
		}

		g_info[i].pFile = pFile;
		strcpy(g_info[i].url, g_url_list[i]);
		
		///
		CURL *curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, pi->url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, pi->pFile);
	}

	return 0;
}

//调用此函数，开始异步 
void begin_unsysnc()
{
	printf("execute %s::%s %d\n", __FILE__, __FUNCTION__, __LINE__);
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, pi->url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, pi->pFile);
	
	int rescode;
	if((rescode = curl_multi_add_handle(g_curlm, curl)) != CURLE_OK)
	{
		fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, curl_multi_strerror(rescode));
		exit(1);
	}
	return;
}

//libcurl回调函数：libcurl与fd联系起来	将sockfd与event关联，将sockfd与multi关联
static int multi_socket_callback(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
{
	// NUM_FIVE个描述符，执行NUM_FIVE次    回调会自动创建描述符s，不需要自己创建     sockfd与event关联
	int kind = (what & CURL_POLL_IN ? EV_READ : 0) | (what & CURL_POLL_OUT ? EV_WRITE : 0) | EV_PERSIST;

	printf("%s::%s %d sockfd=%d what=%d kind=%d\n", __FILE__, __FUNCTION__, __LINE__, s, what, kind);
	struct event* fd_event = event_new(g_evbase, s, kind, event_callback, NULL);	//参数暂传为NULL
	if (fd_event == NULL)
	{
		fprintf(stderr, "%s::%s %d failed\n", __FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}
	event_add(fd_event, NULL);
	
	curl_multi_assign(g_curlm, s, NULL);

	return 0;
}

//文件描述符事件回调
static void event_callback(int fd, short event, void *arg)
{
	int action, rescode, still_running;
	
	action = (event & EV_READ ? CURL_CSELECT_IN : 0) | (event & EV_WRITE ? CURL_CSELECT_OUT : 0);
//	printf("execute %s::%s %d fd=%d action=%d\n", __FILE__, __FUNCTION__, __LINE__, fd, action);
	
	if((rescode = curl_multi_socket_action(g_curlm, fd, action, &still_running)) != CURLE_OK)
	{
		fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, curl_multi_strerror(rescode));
			return -1;
	}
	if (!still_running)	//在multi中运行的easy数	0 - transfers complete/done
	{
		int msgs_in_queue;
		CURLMsg *m;
		m = curl_multi_info_read(g_curlm, &msgs_in_queue);
		if (msgs_in_queue)
		{
			fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, curl_multi_strerror(m->data.result));
			return -1;
		}
	}
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	printf("execute %s::%s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return fwrite(ptr, size, nmemb, (FILE*)userdata);
}

//写的目录及文件名
int download_dir_file()
{
	// printf("execute %s::%s %d\n", __FILE__, __FUNCTION__, __LINE__);
	for (int i = 0; i < NUM_FIVE; i++)
	{		
		char tmp_chars[1024];
		strcpy(tmp_chars, g_url_list[i]);
		char *file_name = strrchr(tmp_chars, 47) + 1;
		//g_files[i] = file_name;
		
		char *pstr = malloc(1024);
		memset(pstr, 0, 1024);
		size_t len = strlen("/home/lb/curl/downdir/");
		memcpy(pstr, "/home/lb/curl/downdir/", len);
		g_paths[i] = memmove(pstr + len, file_name, strlen(file_name)) - len;
	
		// printf("---%s---%d\n", g_paths[i], len);
	}
	return 0;
}

int multi_timer_callback(CURLM *multi, long timeout_ms, void *userp)
{	
	struct timeval timeout;
	timeout.tv_sec = timeout_ms / 1000;
	timeout.tv_usec = (timeout_ms % 1000) * 1000;
	printf("%s::%s %d %ld\n", __FILE__, __FUNCTION__, __LINE__, timeout_ms);
	
	evtimer_add(g_evtimer, &timeout);
	
	return 0;
}

//定时器回调此函数
static void timer_callback(int fd, short kind, void *userp)
{
	int rescode, still_running;
	if ((rescode = curl_multi_socket_action(g_curlm, CURL_SOCKET_TIMEOUT, 0, &still_running)) != CURLE_OK)
	{
		fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, curl_multi_strerror(rescode));
		return -1;
	}
	printf("execute %s::%s %d still_running=%d\n", __FILE__, __FUNCTION__, __LINE__, still_running);
	
	CURLMsg *msg;
	int msgs_left;
	CURL *easy;
	char *eff_url;
	while((msg = curl_multi_info_read(g_curlm, &msgs_left)))
	{
		if (msg->msg == CURLMSG_DONE)
		{
			easy = msg->easy_handle;
			curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
			fprintf(stdout, "DONE: %s\n", eff_url);
			curl_multi_remove_handle(g_curlm, easy);
			curl_easy_cleanup(easy);
		}
	}
}

void Free()
{
	for (int i = 0; i < NUM_FIVE; i++)
	{
		if (g_paths[i] != NULL)
		{
			free(g_paths[i]);
			g_paths[i] = NULL;
		}
	}
}