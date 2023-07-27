
1. 执行.c文件,会调用.php文件

2. 执行.html文件,会调用.php文件

3. 1 和 2 的执行是等价的

4. 如果 2 不能执行也正常,因为没有全测,但 1 肯定是可以执行的

5. 较方便的办法，不用自己写代码，只需输入命令，系统生成代码即可。几个简单的示例如下:

双向认证调试(-v)，代码写入very2.c中
```sh
	curl -v https://192.168.10.130:445 --cacert sm2.ca.crt.pem --cert sm2.client.crt.pem --key sm2.client.pvk.pem --libcurl very2.c
```
发送不安全(-k)GET请求，获取内容及响应头信息(-i)，代码写入get.c中
```sh
	curl -X GET -k -i www.baidu.com --libcurl get.c
```
发送GET请求，超时返回(--connect-timeout)，代码写入timeout.c中
```sh
	curl -X GET --connect-timeout 5 www.google.com --libcurl timeout.c
```
发送POST请求及参数，代码写入post.c中（注意参数不等同于表单）
```sh
	curl -X POST --url api.juheapi.com/japi/toh --data "key=4be7e7f0f6b180cc3677ddb0b94abfcb&month=10&day=1&v=1.0" --libcurl post.c
```
发送POST请求及表单，代码写入formadd.c中
```sh
	curl -X POST --url 192.168.10.137/dir/3.html --form username=automan --form userage=10000 --libcurl formadd.c
```
下载文件
```sh
	curl -o nginx.tar.gz http://nginx.org/download/nginx-1.14.0.tar.gz --libcurl down.c
```
注意，如果执行过程需要调用回调(如下载、上传、过程回调)，还需要自己加回调。

还没发现文件上传到HTTP服务器的好命令，上传到ftp服务器应该容易（未测）。