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
 */
 
// #define LOCAL_FILE      "./gcc-8.1.0.tar.gz"
#define LOCAL_FILE      "./upload.txt"
#define UPLOAD_FILE_AS  "while-uploading.txt"
#define REMOTE_URL      "ftp://192.168.10.137/"  UPLOAD_FILE_AS
#define RENAME_FILE_TO  "renamed-and-fine.txt"

/** 这个回调函数是必须的 */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  curl_off_t nread;
  size_t retcode = fread(ptr, size, nmemb, stream);

  nread = (curl_off_t)retcode;

  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
  return retcode;
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  FILE *hd_src;
  struct stat file_info;
  curl_off_t fsize;

  struct curl_slist *headerlist = NULL;
  static const char buf_1 [] = "RNFR " UPLOAD_FILE_AS;
  static const char buf_2 [] = "RNTO " RENAME_FILE_TO;
  static const char buf_3 [] = "DELE " RENAME_FILE_TO;
  
  fprintf(stdout, "%d buf_1: %s buf_2: %s\n", __LINE__, buf_1, buf_2);

  /* get the file size of the local file */
  if(stat(LOCAL_FILE, &file_info)) {
    printf("Couldnt open '%s': %s\n", LOCAL_FILE, strerror(errno));
    return 1;
  }
  fsize = (curl_off_t)file_info.st_size;

  printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

  /* get a FILE * of the same file */
  hd_src = fopen(LOCAL_FILE, "rb");

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* build a list of commands to pass to libcurl */
    headerlist = curl_slist_append(headerlist, buf_1);
    headerlist = curl_slist_append(headerlist, buf_2);
	headerlist = curl_slist_append(headerlist, buf_3);

    /* we want to use our own read function */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

    /* enable uploading */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	
	/* set user and password */
	curl_easy_setopt(curl, CURLOPT_USERPWD, "ftplb:1234");
	
    /* specify target */
    curl_easy_setopt(curl, CURLOPT_URL, REMOTE_URL);
	
	printf("REMOTE_URL %s\n", REMOTE_URL);

    /* pass in that last of FTP commands to run after the transfer */
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

    /* now specify which file to upload */
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

    /* Set the size of the file to upload (optional).  If you give a *_LARGE
       option you MUST make sure that the type of the passed-in argument is a
       curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
       make sure that to pass in a type 'long' argument. */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)fsize);

    /* Now run off and do what you've been told! */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: (%d)%s\n", res,
              curl_easy_strerror(res));

    /* clean up the FTP commands list */
    curl_slist_free_all(headerlist);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  fclose(hd_src); /* close the local file */

  curl_global_cleanup();
  return 0;
}
