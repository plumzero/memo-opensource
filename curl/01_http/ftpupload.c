#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

/**
	演示非匿名用户上传文件到ftp服务器（redhat7.5）

	程序运行前的准备：
		1.搭建ftp服务器 采用常用的vsftpd包作演示 XXX.rpm
		2.在默认配置基础上修改主配置文件项 /etc/vsftpd/vsftpd.conf （提前备份）
			anonymous_enable=NO
			chroot_local_user=YES
			allow_writeable_chroot=YES
		3.重启服务 systemctl restart vsftpd
		4.确保21端口开放	iptables -I INPUT -p tcp --dport 21 -j ACCEPT
		5.新建登录名及密码	/var/ftp/pub为默认ftp服务器目录
			useradd -s /sbin/nologin -d /var/ftp/pub -g ftp ftplb
			chmod 777 /var/ftp/pub
			passwd ftplb
	
	最简单的上传文件到ftp示例
 */
 
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  curl_off_t nread;
  size_t retcode = fread(ptr, size, nmemb, stream);
  /** 每次读取文件的16384个字节，即nread<=16384 */
  nread = (curl_off_t)retcode;

  return retcode;
}

static int usage()
{
	// printf(" TLS client/server both verify with callback.\n");
	printf("--user name to be logined\n");
	printf("--pwd password to be confirmed with the user above\n");
	printf("--file file name to be uploaded\n");
	printf("--url the ftp address to receive the file(like:ftp://192.168.10.137:21)\n");
	printf("--help print this message.\n");
	printf("-v Enable verbose.\n");
	exit(-1);
}

int main(int argc, char *argv[])
{
	argc--;
	argv++;
	
	char *user = NULL, *pwd = NULL, *file = NULL, *url = NULL;
	
	int verbose = 0;
	
	// ./ftpupload --user ftplb --pwd 1234 --file ./upload.c --url 192.168.10.137 --v
	while(argc > 0)
	{
		if (strcmp(*argv, "--user") == 0)
		{
			if(--argc<1)
				break;
			user = *(++argv);
		}
		else if(strcmp(*argv, "--pwd") == 0)
		{
			if(--argc<1)
				break;
			pwd = *(++argv);
		}
		else if(strcmp(*argv, "--file") == 0)
		{
			if (--argc<1)
				break;
			file = *(++argv);
		}
		else if(strcmp(*argv, "--url") == 0)
		{
			if(--argc<1)
				break;
			url = *(++argv);
		}
		else if (strcmp(*argv, "--help") == 0)
		{
			usage();
			exit(-1);
		}
		else if (strcmp(*argv, "--v") == 0)
		{
			verbose = 1;
			if(--argc<1)
				break;
			continue;
		}
		else
			break;
		argc--;
		argv++;
	}
	
	CURL *curl;
	CURLcode res;
	FILE *pFile;
	curl_off_t fsize;
	char userpwd[128];
	char ftpurl[256];
	
	pFile = fopen(file, "rb");
	if (pFile == NULL)
	{
		fprintf(stderr, "Couldn't open '%s': %s\n", file, strerror(errno));
		return -1;
	}
	fseek(pFile, 0L, SEEK_END);
	fsize = (curl_off_t)ftell(pFile);
	fseek(pFile, 0L, SEEK_SET);
	
	char *fname = strrchr(file, 47) + 1;
	snprintf(userpwd, sizeof(userpwd), "%s:%s", user, pwd);
	snprintf(ftpurl, sizeof(ftpurl), "ftp://%s:%u/%s", url, 21, fname);
	
	printf("line: %d  ftpurl: %s\n", __LINE__, ftpurl);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl) 
	{
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_READDATA, pFile);

		/** 开启上传 */
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		
		/** 登录设置 */
		curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
		curl_easy_setopt(curl, CURLOPT_URL, ftpurl);
		
		/** 下面两种写法均可，注意第二个参数与第三个参数类型要对应，建议使用第一种 */
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
		//curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)fsize);
		
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: (%d)%s\n", res, curl_easy_strerror(res));
		
		curl_easy_cleanup(curl);
	}
	
	fclose(pFile);
	curl_global_cleanup();
	return 0;
}
