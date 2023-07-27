/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Upload to a file:// URL
 * </DESC>
 */
#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

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
	/** 程序 用户名(--user)  密码(--pwd)  上传文件名(--file) 目标(--url) */
	argc--;
	argv++;
	
	char user[50];
	char pwd[100];
	char file[200];
	char url[200];
	
	int verbose = 0;
	
	// ./fileupload --user ftplb --pwd 1234 --file ./upload.c --url 192.168.10.137 --v
	while(argc > 0)
	{
		if (strcmp(*argv, "--user") == 0)
		{
			strcpy(user, *argv);
		}
		else if(strcmp(*argv, "--pwd") == 0)
		{
			strcpy(pwd, *argv);
		}
		else if(strcmp(*argv, "--file") == 0)
		{
			strcpy(file, *argv);
		}
		else if(strcmp(*argv, "--url") == 0)
		{
			strcpy(url, *argv);
		}
		else if (strcmp(*argv, "--help") == 0)
		{
			usage();
		}
		else if (strcmp(*argv, "--v") == 0)
		{
			verbose = 1;
		}
		else
			break;
		argc--;
		argv++;
	}
	
	CURLcode res;
	long filesize ;
	FILE* pFile;
	char userpwd[128];
	char ftpurl[256];
	char errbuf[CURL_ERROR_SIZE];
	
	// pFile = fopen(file, "r");
	pFile = fopen("./upload.txt", "r");
	if (pFile == NULL)
	{
		goto err;
	}
	fseek(pFile, 0L, SEEK_END);
	filesize = ftell(pFile);
	fseek(pFile, 0L, SEEK_SET);
	
	curl_global_init(CURL_GLOBAL_ALL);
	CURL*  curl = curl_easy_init();
	if (curl == NULL)
	{
		fprintf(stderr, "%s %d failed: %s\n", __FUNCTION__, __LINE__, strerror(errno));
		goto err;
	}
	
	char *fname = strrchr(file, 47) + 1;
	
	// snprintf(userpwd, sizeof(userpwd), "%s:%s", user, pwd);
	// snprintf(ftpurl, sizeof(ftpurl), "ftp://%s:%u/%s", url, 21, fname);
	
	snprintf(userpwd, sizeof(userpwd), "%s:%s", "ftplb", "1234");
	snprintf(ftpurl, sizeof(ftpurl), "ftp://%s:%u/%s", "192.168.10.137", 21, "upload.txt");
	
	printf("userpwd: %s  ftpurl: %s\n", userpwd, ftpurl);
	
	curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
	curl_easy_setopt(curl, CURLOPT_URL, ftpurl);
	curl_easy_setopt(curl, CURLOPT_READDATA, pFile);  
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);  
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, filesize);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10); 
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);	
	if (verbose)
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		// size_t len = strlen(errbuf);
		// fprintf(stderr, "\nlibcurl: (%d) ", res);
		// if(len)
		  // fprintf(stderr, "%s%s", errbuf,
				  // ((errbuf[len - 1] != '\n') ? "\n" : ""));
		// else
			fprintf(stderr, "%s %d failed: (%d)%s\n", __FUNCTION__, __LINE__, res, curl_easy_strerror(res));
			// fprintf(stderr, "%s\n", curl_easy_strerror(res));
		goto err;
	}
	else
	{
		double speed_upload, total_time;
		curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
		fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n", speed_upload, total_time);
    }
	
err:
	if (pFile)
		fclose(pFile);
	if (curl)
		curl_easy_cleanup(curl);

	curl_global_cleanup();
	
	return 0;
}
