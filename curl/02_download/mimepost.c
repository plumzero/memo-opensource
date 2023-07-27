#include <stdio.h>
#include <s>
#include <curl/curl.h>

int main ()
{
  //easy handler的句柄  
  CURL* curl = NULL;  
  CURLcode res = CURLE_OK;  
  //HTTP报文头  
  struct curl_slist* headers = NULL;  
  char tmp_str[256] = { 0 };  
  
  //构建HTTP报文头  
  snprintf(tmp_str, sizeof(tmp_str), "User-Agent: %s", req->user_agent_);  
  headers = curl_slist_append(headers, tmp_str);  
  snprintf(tmp_str, sizeof(tmp_str), "Accept-Language: %s", req->language_);  
  headers = curl_slist_append(headers, tmp_str);  
  snprintf(tmp_str, sizeof(tmp_str), "X-FORWORDED-FOR: %s", req->ip_.c_str());  
  headers = curl_slist_append(headers, tmp_str);  
  
  /*这个函数只能用一次,如果这个函数在curl_easy_init函数调用时还没调用， 
  它讲由libcurl库自动调用，所以多线程下最好在主线程中调用一次该函数以防止在线程 
  中curl_easy_init时多次调用*/  
  curl_global_init(CURL_GLOBAL_ALL);  
    
  //初始化easy handler句柄  
  curl = curl_easy_init();  
  if (curl) {  
    //设置post请求的url地址  
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.10.137");  
    //设置HTTP头  
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);  
    //设置发送超时时间  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);  
  
    //执行单条请求  
    res = curl_easy_perform(curl);  
    if (res != CURLE_OK) {  
      //curl_easy_strerror进行出错打印  
      LOG(WARNING) << "curl_easy_perform() failed:" << curl_easy_strerror(res);  
    }  
      
    curl_slist_free_all(headers);  
      
    //这个调用用来结束一个会话.与curl_easy_init配合着用  
    curl_easy_cleanup(curl);  
      
    //在结束libcurl使用的时候，用来对curl_global_init做的工作清理。类似于close的函数  
    curl_global_cleanup();  
}