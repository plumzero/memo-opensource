
#include "http_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <map>

static const char data[] =
    // "POST /joyent/http-parser HTTP/1.1\r\n"
    // "Host: github.com\r\n"
    // "DNT: 1\r\n"
    // "Accept-Encoding: gzip, deflate, sdch\r\n"
    // "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
    // "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) "
        // "AppleWebKit/537.36 (KHTML, like Gecko) "
        // "Chrome/39.0.2171.65 Safari/537.36\r\n"
    // "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,"
        // "image/webp,*/*;q=0.8\r\n"
    // "Referer: https://github.com/joyent/http-parser\r\n"
    // "Connection: keep-alive\r\n"
    // "Transfer-Encoding: chunked\r\n"
    // "Cache-Control: max-age=0\r\n\r\nb\r\nhello world\r\n0\r\n";
    
    // "GET /favicon.ico HTTP/1.1\r\n"
         // "Host: 0.0.0.0=5000\r\n"
         // "User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9) Gecko/2008061015 Firefox/3.0\r\n"
         // "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
         // "Accept-Language: en-us,en;q=0.5\r\n"
         // "Accept-Encoding: gzip,deflate\r\n"
         // "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
         // "Keep-Alive: 300\r\n"
         // "Connection: keep-alive\r\n"
         // "\r\n";
         
    "HTTP/1.1 200 OK\r\n"
         "Date: Tue, 04 Aug 2009 07:59:32 GMT\r\n"
         "Server: Apache\r\n"
         "X-Powered-By: Servlet/2.5 JSP/2.1\r\n"
         "Content-Type: text/xml; charset=utf-8\r\n"
         "Connection: close\r\n"
         "\r\n"
         "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
         "<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\">\n"
         "  <SOAP-ENV:Body>\n"
         "    <SOAP-ENV:Fault>\n"
         "       <faultcode>SOAP-ENV:Client</faultcode>\n"
         "       <faultstring>Client Error</faultstring>\n"
         "    </SOAP-ENV:Fault>\n"
         "  </SOAP-ENV:Body>\n"
         "</SOAP-ENV:Envelope>";
    
static const size_t data_len = sizeof(data) - 1;

static int on_message_begin_cb(http_parser* p)
{
    printf("%s: http_major=%d, http_minor=%d, status_code=%d, method=%d, http_errno=%d\n",
            __FUNCTION__, p->http_major, p->http_minor, p->status_code, p->method, p->http_errno);
    
    return 0;
}
static int on_headers_complete_cb(http_parser* p)
{
    printf("%s: http_major=%d, http_minor=%d, status_code=%d, method=%d, http_errno=%d\n",
            __FUNCTION__, p->http_major, p->http_minor, p->status_code, p->method, p->http_errno);
    
    return 0;
}
static int on_message_complete_cb(http_parser* p)
{
    printf("%s: http_major=%d, http_minor=%d, status_code=%d, method=%d, http_errno=%d\n",
            __FUNCTION__, p->http_major, p->http_minor, p->status_code, p->method, p->http_errno);
    
    return 0;
}
static int on_chunk_header_cb(http_parser* p)
{
    printf("%s: http_major=%d, http_minor=%d, status_code=%d, method=%d, http_errno=%d\n",
            __FUNCTION__, p->http_major, p->http_minor, p->status_code, p->method, p->http_errno);
    
    return 0;
}
static int on_chunk_complete_cb(http_parser* p)
{
    printf("%s: http_major=%d, http_minor=%d, status_code=%d, method=%d, http_errno=%d\n",
            __FUNCTION__, p->http_major, p->http_minor, p->status_code, p->method, p->http_errno);
    
    return 0;
}

static int on_url_cb(http_parser* p, const char *at, size_t length)
{
    printf("%s: length=%lu, at=%s\n", __FUNCTION__, length, std::string(at, at + length).c_str());
    
    return 0;
}
static int on_status_cb(http_parser* p, const char *at, size_t length)
{
    printf("%s: length=%lu, at=%s\n", __FUNCTION__, length, std::string(at, at + length).c_str());
    
    return 0;
}
static int on_header_field_cb(http_parser* p, const char *at, size_t length)
{
    printf("%s: length=%lu, at=%s\n", __FUNCTION__, length, std::string(at, at + length).c_str());
    
    return 0;
}
static int on_header_value_cb(http_parser* p, const char *at, size_t length)
{
    printf("%s: length=%lu, at=%s\n", __FUNCTION__, length, std::string(at, at + length).c_str());
    
    return 0;
}
static int on_body_cb(http_parser* p, const char *at, size_t length)
{
    printf("%s: length=%lu, at=%s\n", __FUNCTION__, length, std::string(at, at + length).c_str());
    
    return 0;
}


static http_parser_settings settings = {
    .on_message_begin = on_message_begin_cb,            /** useless */
    .on_url = on_url_cb,                                /** useful  */
    .on_status = on_status_cb,                          /** useful  */
    .on_header_field = on_header_field_cb,              /** useful  */
    .on_header_value = on_header_value_cb,              /** useful  */
    .on_headers_complete = on_headers_complete_cb,      /** useful  */
    .on_body = on_body_cb,                              /** useful  */
    .on_message_complete = on_message_complete_cb,      /** useless */
    .on_chunk_header = on_chunk_header_cb,              /** useless */
    .on_chunk_complete = on_chunk_complete_cb
};

int main()
{
    struct http_parser parser;

    http_parser_init(&parser, HTTP_RESPONSE);
    
    size_t parsed = http_parser_execute(&parser, &settings, data, data_len);
    
    printf("parsed=%lu, data_len=%lu\n", parsed, data_len);
    assert(parsed == data_len);
    
    printf("========== test success ==========\n");
    
    return 0;
}
