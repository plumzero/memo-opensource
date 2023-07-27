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
#include <sys/io.h>

#define THREAD_NUM 30
#define URLMAXSIZE 1024

typedef struct _URL_LIST
{
	char *url_list_ptr[50];
	int url_list_length;
} UL;

typedef struct _threads
{
	pthread_t thread_id;
	int thread_use;		//0-未占用 !0-占用
} TH;

typedef struct _interval
{
	long begin_iter;
	long end_iter;
	int use;	//0-not use
	int ok;		//1-下载完毕
} INTERV;

typedef struct _trans_pos
{
	FILE *_pFile;
	long _pos;
} TransPos;

UL g_ul_st;
TH threads[THREAD_NUM];
int g_download_iter;
char g_full_download_path[1024];
FILE *g_pFile;
pthread_mutex_t g_mutex;
pthread_mutex_t g_mutex_index;
INTERV g_interval[THREAD_NUM];
int g_sliced_flag;
int g_download_status;

int dotz;

static int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow);
static int seek_callback(void *userp, curl_off_t offset, int origin);
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *instream);
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

int download_from_whats(const char *urls_path);
void *thread_func();
void slice_file();

//下载文件长度
static long get_download_file_length(const char *url)
{
	double file_length;
	int res;
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);	//允许跳转
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);		//最多只能跳转3次
	// curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP | CURLRPOTO_HTTPS);	//允许跳转到的协议地址
	// curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);	//只用作HTTP请求中，随着跳转地址不同自动设置header域
	curl_easy_setopt(curl, CURLOPT_HEADER, 1L);			//只需要header头
	curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);			//不需要body
	printf("line = %d, url = %s\n", __LINE__, url);
	if ((res = curl_easy_perform(curl)) != CURLE_OK)
	{
		fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, curl_easy_strerror(res));
		file_length = -1;
	}
	else
	{
		if ((res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_length)) != CURLE_OK)
		{
			fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, curl_easy_strerror(res));
			file_length = -1;
		}
	}
	curl_easy_cleanup(curl);
	return file_length;
}

//传输过程的回调
static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{	
	double tftotal;
	double tfnow;
	
	if (dltotal && !ultotal)
	{
		tftotal = dltotal;
		tfnow = dlnow;
	}
	else if (!dltotal && ultotal)
	{
		tftotal = ultotal;
		tfnow = ulnow;
	}
	
	if (tfnow && tftotal)
	{
		double proportion = tfnow / tftotal;
		int totaldots = 40;
		int dots = round(proportion * totaldots);
		if (dots && dots != dotz)
		{
			int i = 0;
			printf("%3.0f%% [", proportion * 100);
			for (; i < dots; i++)
				printf("*");
			for (; i < totaldots; i++)
				printf(" ");
			printf("]\n");
			fflush(stdout);
			
			dotz = dots;
		}

		return 0;	
	}	
	return -1;
}

//CURLOPT_SEEKFUNCTION选项回调
static int seek_callback(void *userp, curl_off_t offset, int origin)
{
	int res;
	TransPos _tp_out = *((TransPos*)userp);
	FILE *pFile = _tp_out._pFile;
	offset = (curl_off_t)_tp_out._pos;
	origin = SEEK_SET;
	res = fseek(pFile, offset, origin);	//定位文件流指针
	if (res != 0)
	{
		fprintf(stderr, "%s::%s fseek failed: %s\n", __FILE__, __FUNCTION__, curl_easy_strerror(CURL_SEEKFUNC_CANTSEEK));
		return CURL_SEEKFUNC_FAIL;
	}
	
	return CURL_SEEKFUNC_OK;
}

//CURLOPT_READFUNCTION选项回调
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *instream)
{
	FILE *pFile = (FILE*)instream;
	
	size_t retcode = fread(buffer, size, nitems, instream);
	if (retcode != nitems)
	{
		if (feof(pFile))
		{
			fprintf(stderr, "end of file reached!\n");
			return 0;
		}
		ferror("error occured: ");
		return -1;
	}
	
	return retcode;
}

//断点下载
static int resume_download_after_break(CURL *curl, curl_off_t download_from, FILE *pFile, const char *url)	//all para is in
{
	//判断是否需要断点续传
	//获取下载目标文件
	TransPos _tp_in;
	long pos = 0;
	char *file_path = NULL;
	if (file_path != NULL)
	{
		FILE *pF = fopen(file_path, "a+b");
		if (pF != NULL)
		{
			fseek(pF, 0, SEEK_END);
			pos = ftell(pF);
			// fseek(pF, 0, SEEK_SET);
			download_from = (curl_off_t)pos;
			pFile = pF;
			
			double file_cl;
			int res;
			if ((res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_cl)) != CURLE_OK)
			{
				fprintf(stderr, "%s::%s curl_easy_perform failed: %s\n", __FILE__, __FUNCTION__, curl_easy_strerror(res));
				return -1;
			}
			
			char range[200];
			sprintf(range, "%d-%d", pos, file_cl);
			curl_easy_setopt(curl, CURLOPT_RANGE, range);
		}
	}
	
	_tp_in._pos = pos;
	_tp_in._pFile = pFile;
	
	//定位文件流指针项
	// curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, download_from);
//	curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, seek_callback);
//	curl_easy_setopt(curl, CURLOPT_SEEKDATA, &_tp_in);
	//下载进度
	//在multi interface进行数据传输时，在空闲期不会调用此函数，除非设置合适libcurl函数执行传输 -> 意思是另找别的函数
//	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
	//设置CURLOPT_XFERINFOFUNCTION选项回调函数的第一个参数，缺省值为NULL -> 暂时设置为 NULL
//	curl_easy_setopt(curl, CURLOPT_XFERINFODATA, NULL);
	//如果要保证CURLOPT_XFERINFOFUNCTION成功，3参必须设置为0L
//	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
	//下载项
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, pFile);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 4); //wait for 4 seconds to connect to server
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0);       //0 means block always
	
	int res;	
	if ((res = curl_easy_perform(curl)) != CURLE_OK)
	{
		fprintf(stderr, "%s::%s curl_easy_perform failed: %s\n", __FILE__, __FUNCTION__, curl_easy_strerror(res));
		return -1;
	}
}

//CURLOPT_WRITEFUNCTION项回调函数
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	pthread_t thr_id = pthread_self();
	INTERV *p_interv = (INTERV*)userdata;
	size_t written;
	//多线程写同一个文件要加锁
	pthread_mutex_lock(&g_mutex);
	if (size * nmemb <= p_interv->end_iter - p_interv->begin_iter + 1)
	{
		fseek(g_pFile, p_interv->begin_iter, SEEK_SET);
		printf("line = %d, func = %s, thr_id = %u, begin_iter = %ld, tail_iter = %ld, end_iter = %ld\n", 
		__LINE__, __FUNCTION__, thr_id, p_interv->begin_iter , p_interv->begin_iter + nmemb, p_interv->end_iter);
		written = fwrite(ptr, size, nmemb, g_pFile);
		p_interv->begin_iter += size * nmemb;
	}
	else
	{
		fseek(g_pFile, p_interv->begin_iter, SEEK_SET);
		printf("line = %d, func = %s, thr_id = %u, begin_iter = %ld, end_iter = %ld, actually = %ld\n", 
		__LINE__, __FUNCTION__, thr_id, p_interv->begin_iter ,p_interv->end_iter, p_interv->end_iter - p_interv->begin_iter + 1);
		written = fwrite(ptr, size, p_interv->end_iter - p_interv->begin_iter + 1, g_pFile);
		//回收线程
		//把这条线程的 use 设为 0 吗 -> NO
		p_interv->ok = 1;
		int counter = 0;
		for (int i = 0; i < THREAD_NUM; i++)
		{
			if(g_interval[i].ok != 1)
				break;
			counter++;
		}
		if (counter == THREAD_NUM)
		{
			printf("download ok\n");
			//g_sliced_flag = 0;
			//g_download_status = 0;
		}
	}
//	printf("line = %d, func = %s, thr_id = %u, begin_iter = %ld, end_iter = %ld\n", __LINE__, __FUNCTION__, thr_id, p_interv->begin_iter ,p_interv->end_iter);
	pthread_mutex_unlock(&g_mutex);
	
    return written; 
}

static int download_directly(const char *url_path, INTERV *p_interv)
{
	pthread_t thr_id = pthread_self();
	printf("line = %d, func = %s, thr_id = %u\n", __LINE__, __FUNCTION__, thr_id);
	int res;
	
	char range[200];
	sprintf(range, "%ld-%ld", p_interv->begin_iter, p_interv->end_iter);
	
	CURL *curl = curl_easy_init();
	
	curl_easy_setopt(curl, CURLOPT_URL, url_path);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, p_interv);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_RANGE, range);
	
	curl_easy_setopt(curl, CURLOPT_CAINFO, "two.pem");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYSTATUS, 1L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
	curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
	curl_easy_setopt(curl, CURLOPT_SSLCERT, "one.pem");
	curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");
	curl_easy_setopt(curl, CURLOPT_SSLKEY, "key.pem");
	
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
	
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_PROXY, "");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
	
	// curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);		//显示下载进度，需另开一条定时器线程
	// curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
	// curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, NULL);
	
	if ((res = curl_easy_perform(curl)) != CURLE_OK)
	{
		fprintf(stderr, "%s::%s %d %u failed: %s\n", __FILE__, __FUNCTION__, __LINE__, thr_id, curl_easy_strerror(res));
		return -1;
	}
	
	curl_easy_cleanup(curl);
	return 0;
}

int main(int argc, char *argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);
	
	g_download_iter = -1;
	g_sliced_flag = 0;
	g_download_status = 0;
	
	//下载任务
	download_from_whats("/home/lb/testc/url.txt");
	//下载到哪
	//download_to_what("/home/lb/curl");
	//分片
	slice_file();
	//创建线程，开始下载
	create_threads();
	
	sleep (20);
	for (int i = 0; i < g_ul_st.url_list_length; i++)
	{
		if (g_ul_st.url_list_ptr[i])
		{
			free(g_ul_st.url_list_ptr[i]);
			g_ul_st.url_list_ptr[i] = NULL;
		}
			
	}
	curl_global_cleanup();
	
	return 0;
}

//只下载到一个目录下
char* download_to_what(const char* download_path)
{
	//if (!g_download_status)
	//	g_download_iter++;
	g_download_iter = 0;
	//下载目录
	char dir_name[1024];
	download_path != NULL ? strcpy(dir_name, download_path) : strcpy(dir_name, "./");
	if (*(dir_name + strlen(dir_name) - 1) != '/')
	{
		strcat(dir_name, "/");
	}
	//下载文件
	char d_url[1024];
	strcpy(d_url, g_ul_st.url_list_ptr[g_download_iter]);
	char *file_name = strrchr(d_url, 47) + 1;
	//全路径
	strcpy(g_full_download_path, dir_name);
	strcat(g_full_download_path, file_name);
	
	return g_full_download_path;
}

//返回值：下载任务数  
int download_from_whats(const char *urls_path)
{
	char full_path[200];
	urls_path != NULL ? strcpy(full_path, urls_path) : strcpy(full_path, "/home/lb/testc/url.txt");
	
	FILE *pFile = fopen(full_path, "r");
	if (pFile == NULL)
	{
		fprintf(stderr, "%s::%s %d failed: %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(ferror(pFile)));
		return -1;
	}
	
	//初始化专门放一个函数里
	for (int i = 0; i < sizeof(g_ul_st.url_list_ptr) / sizeof(g_ul_st.url_list_ptr[0]); i++)
		g_ul_st.url_list_ptr[i] = NULL;
	g_ul_st.url_list_length = 0;
	
	char url[1024];
	while (fscanf(pFile, "%[^\n]", url) != EOF)
	{
		fgetc(pFile);
		int i, begin;
		for (i = 0; url[i] != 0 && (url[i] == 32 || url[i] == 9); i++);
		begin = i;
		for (; url[i] != 0 && url[i] != 32 && url[i] != 9; i++);
		for (; url[i] != 0; i++)
			url[i] = 0;
		if (url[begin])
		{
			g_ul_st.url_list_ptr[g_ul_st.url_list_length] = malloc(URLMAXSIZE);
			memset(g_ul_st.url_list_ptr[g_ul_st.url_list_length], 0, URLMAXSIZE);
			memcpy(g_ul_st.url_list_ptr[g_ul_st.url_list_length], &url[begin], strlen(&url[begin]));
			printf("%s--%s\n", &url[begin], g_ul_st.url_list_ptr[g_ul_st.url_list_length]);
			g_ul_st.url_list_length++;
		}
	}

	return g_ul_st.url_list_length;
}

//将文件分片
void slice_file()
{
	g_download_iter = 0;
	char *download_url_now = g_ul_st.url_list_ptr[g_download_iter];
	printf("line = %d, g_download_iter = %d, download_url_now = %s\n", __LINE__, g_download_iter, download_url_now);
	long file_length = get_download_file_length(download_url_now);
	long range = file_length / (THREAD_NUM - 1);
	printf("line = %d, file_length=%ld, range=%ld\n", __LINE__, file_length, range);
	long size = 0;
	for (int i = 0; i < THREAD_NUM; i++)
	{
		g_interval[i].begin_iter = size;
		g_interval[i].end_iter = (size + range <= file_length ? size + range - 1 : file_length - 1);
		printf("line = %d, func = %s, begin_iter = %ld, end_iter = %ld\n", __LINE__, __FUNCTION__, g_interval[i].begin_iter, g_interval[i].end_iter);
		g_interval[i].use = 0;
		g_interval[i].ok = 0;
		size += range;
	}
}

//创建多个线程
int create_threads()	//30
{
	int ret;
	for (int i = 0; i < THREAD_NUM; i++)
	{
		ret = pthread_create(&(threads[i].thread_id), NULL, thread_func, NULL);
		if (ret)
		{
			fprintf(stderr, "%s::%s %d failed\n", __FILE__, __FUNCTION__, __LINE__);
			return -1;
		}
		threads[i].thread_use = 0;
		// pthread_detach(threads[i].thread_id);	//脱离
	}
	return 0;
}

void *thread_func(void *args)
{
	int i;

	//此线程正式工作
	//下载路径
	char *download_path_now = download_to_what("/home/lb/curl/");
	pthread_t thr_id = pthread_self();
	//printf("line = %d, download_path_now = %s, thread_id = %u\n", __LINE__, download_path_now, thr_id);
	//下载源
	char *download_url_now = g_ul_st.url_list_ptr[g_download_iter];
	//printf("line = %d, g_download_iter = %d, download_url_now = %s, thread_id = %u\n", __LINE__, g_download_iter ,download_url_now, thr_id);
	//文件分片
	// if (!g_sliced_flag)
	// {
		// slice_file();
		// g_sliced_flag = 1;
		// sleep(10);
	// }
	if (!g_pFile)
	{
		g_pFile = fopen(download_path_now, "wb+");
		if (g_pFile == NULL)
		{
			fprintf(stderr, "%s::%s %d failed\n", __FILE__, __FUNCTION__, __LINE__);
			return NULL;
		}
	}
	//选择分片，加锁，防止两线程选同一分片
	int index;
	pthread_mutex_lock(&g_mutex_index);
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (!g_interval[i].use)
		{
			index = i;
			break;
		}
	}
	g_interval[index].use = 1;
	pthread_mutex_unlock(&g_mutex_index);
	
	download_directly(download_url_now, &g_interval[index]);
	
	return NULL;
}
