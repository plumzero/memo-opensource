功能：
	均是利用CURLM实现五个独立文件同时下载，但执行方法不同

	编译命令：
		gcc -g -w XXX.c -o XXX -I$PKICM/include -I$CYSEC/include -I$CURL/include -I$CARES/include -L$PKICM/lib -lpkicm -L$CYSEC/lib -lcysec -L$CURL/lib -lcurl -L$CARES/lib -lcares