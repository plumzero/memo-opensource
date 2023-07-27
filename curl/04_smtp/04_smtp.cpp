
#include <string>

#include <curl/curl.h>

// https://blog.csdn.net/zhangxuechao_/article/details/88365341

int main()
{
    std::string http_uri;
    std::string mail_content;

    CURL *curl;
    CURLcode res = CURLE_OK;
    
    curl_global_init(CURL_GLOBAL_ALL);

    // smtp.qq.com

    curl = curl_easy_init();
    if (! curl) {
        fprintf(stderr, "curl_easy_init failed\n");
        curl_global_cleanup();
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.qq.com");
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "from@163.com");
    // curl_easy_setopt(curl, CURLOPT_MAIL_AUTH, "<secret@cave>");
    struct curl_slist * list = NULL;
    list = curl_slist_append(list, "root@loaclhost");
    list = curl_slist_append(list, "to@qq.com");
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, list);
    curl_easy_setopt(curl, CURLOPT_USERNAME, "to@qq.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "xxx");

    // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    // curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    // curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    // struct curl_slist *headers = NULL;
    // headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    // curl_mime * mime;
    // curl_mimepart * part;
    
    // mime = curl_mime_init(curl);
    // part = curl_mime_addpart(mime);
    // curl_mime_name(part, "target");
    // curl_mime_data(part, "mail", CURL_ZERO_TERMINATED);

    // part = curl_mime_addpart(mime);
    // curl_mime_name(part, "data");
    // curl_mime_data(part, mail_content.c_str(), CURL_ZERO_TERMINATED);
    
    // curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    
    // curl_mime_free(mime);
    curl_slist_free_all(list);
    curl_easy_cleanup(curl);

    curl_global_cleanup();

    return res;
}