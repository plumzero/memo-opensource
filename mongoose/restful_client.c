

#include "mongoose.h"

/* RESTful server host and request URI */

static const char *url[] = {
    "http://10.0.80.121:8090/api/v1/micro/service/NoSource",
    "http://10.0.80.121:8090/api/v1/micro/service/A",
    "http://10.0.80.121:8090/api/v1/micro/service/B",
    "http://10.0.80.121:8090/api/v1/micro/service/C",
    "http://10.0.80.121:8090/api/v1/micro/service/D"
};

// #define _DEBUG

#ifdef _DEBUG
    #define mg_debug(format, ...) printf(format, __VA_ARGS__)
#else
    #define mg_debug(format, ...) do {} while(0)
#endif

static sig_atomic_t s_received_signal = 0;

typedef struct mg_mgr_client {
    int conn_total_count;            // 连接总请求统计
    int conn_success_count;          // 连接成功统计
    int conn_failure_count;          // 连接失败统计
    int conn_timeout_count;          // 连接超时统计
    int conn_close_count;            // 连接成功关闭统计
    int get_success_count;           // 获取资源成功统计
    int get_failure_count;           // 获取资源失败统计
} mg_mgr_client;

typedef struct mg_conn_client {
    unsigned char *buffer;          // 写缓存指针
    size_t buffer_len;              // 写缓存长度
    size_t buffer_pos;              // 写缓存位置
#ifdef _DEBUG
    FILE *pf;
#endif
} mg_conn_client;


typedef size_t(write_cb)(const void *buffer, size_t size, void *user_data);

static void signal_handler(int sig_num)
{
    printf("received signal %d, will exit the program\n", sig_num);
    signal(sig_num, signal_handler);
    s_received_signal = sig_num;
}

/** 暴露于外 */
size_t write_callback(const void *buffer, size_t size, void *user_data)
{
    struct mg_conn_client *pstConnClient = (struct mg_conn_client *)user_data;
    unsigned char *pucNewBuf = NULL;
    
    size_t uiRemBufSize = pstConnClient->buffer_len - pstConnClient->buffer_pos;
    
    if (size > uiRemBufSize) {
        pucNewBuf = realloc(pstConnClient->buffer, pstConnClient->buffer_len + (size - uiRemBufSize));
        if (pucNewBuf == NULL) {
            fprintf(stderr, "callback realloc failed\n");
            size = uiRemBufSize;
        } else {
            pstConnClient->buffer_len += size - uiRemBufSize;
            pstConnClient->buffer = pucNewBuf;
        }
    }
        
#ifdef _DEBUG
        fwrite(buffer, 1, size, pstConnClient->pf);
#endif
    
    memcpy(&pstConnClient->buffer[pstConnClient->buffer_pos], buffer, size);
    pstConnClient->buffer_pos += size;
    
    return size;
}

size_t write_operation(const void* sptr, write_cb *cb, void *user_data)
{
    struct http_message *hm = (struct http_message *)sptr;

    return cb(hm->body.p, hm->body.len, user_data);
}

/**
 *  restful 学习:
 *  ev_handler: 当连接上收到服务端的信息时，restful clinet 会调用 ev_handler 进行处理
 *  客户端可用消息事件
 *    MG_EV_CONNECT         请求连接成功或失败时，客户端获得的信息
 *    MG_EV_HTTP_CHUNK      HTTP chunked-encoding 块到达，http_message::body 中包含不完全包体
 *                          ，通过它组装成完整的 HTTP 包体。接收时会消耗一定内存，可以通过设置
 *                          MG_F_DELETE_CHUNK 标志位释放该部分缓存。
 *                          通常是在收到 MG_EV_HTTP_REPLY 消息事件后，设置此标志位。
 *    MG_EV_HTTP_REPLY      HTTP 请求回应到达。客户端收到一个完整的 http 响应会触发此消息事件。
 *
 *  客户端可用连接标志位
 *    MG_F_SEND_AND_CLOSE
 *      将套接缓冲区数据全部推出，之后关闭这条连接
 *      客户端可以在收到 MG_EV_HTTP_REPLY 后设置此标志位，从而关闭这条连接，实现短连接处理
 *      除 MG_EV_HTTP_REPLY 事件外，最好不要在其他事件中设置此标志位
 */

/**
 * 说明:
 * 1. 客户端短连接机制: 
 *    a. 只要与服务端连接成功，无论获取资源是否成功，只要获得服务端回复后(MG_EV_HTTP_REPLY)，随即请求断开。
 *    b. 客户端使用短连接时，如果服务端发送数据量较大，服务端会将所有数据全部写入服务端内存后（此时客户端处于
 *    等待状态），再慢慢发送。客户端不断收到服务端数据，会慢慢追加写入到客户端内存（此时服务端处于假等状态，
 *    如果服务端断开，则客户端接收数据终止），直到收到 MG_EV_HTTP_REPLY 消息事件。
 *    c. 客户端短连接机制在传输大数据时，会耗费大量时间(1.09M/s)。
 * 2. 客户端长连接机制:
 *    为实现长连接，客户端在接收到 MG_EV_HTTP_REPLY 消息事件后，禁止设置 MG_F_SEND_AND_CLOSE 标志位。
 *    经测试，这种方法不可行，暂时使用短连接吧
 * 3. 连接超时机制:
 *    超时时间内若无法与服务端建立连接，则客户端立即销毁此连接(MG_F_CLOSE_IMMEDIATELY)
 */
 
/**
 * 测试
 * 1. 内存泄漏测试
 *      valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./restful_client 10.0.80.121 8090 0
 *    无泄漏
 * 2. 文件测试
 *    3.8K 文本文件    pass
 *    500K 文本文件    pass
 *    12M  二进制文件  pass
 */
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data, void *user_data)
{
    struct http_message *hm = (struct http_message *) ev_data;
    int iConnectStatus;
    struct mg_mgr_client *pstMgrClient = (struct mg_mgr_client *)nc->mgr->user_data;
    struct mg_conn_client *pstConnClient = (struct mg_conn_client *)user_data;
    size_t siWriteLen;
        
    pstMgrClient->conn_total_count++;

    switch (ev) 
    {
        case MG_EV_CONNECT:
            iConnectStatus = *(int *)ev_data;
            if (iConnectStatus != 0) {
                printf("connect to server failed(reason: %s)\n", strerror(iConnectStatus));
                pstMgrClient->conn_failure_count++;
            } else {
                mg_debug("socket(%d) connect to server success\n", nc->sock);
                pstMgrClient->conn_success_count++;
            }
            break;
        case MG_EV_HTTP_CHUNK:
            mg_debug("chunked-encoding arrived bytes: %d, append up to total bytes: %d\n", 
                                                hm->body.len, nc->recv_mbuf.len);
            break;
        case MG_EV_HTTP_REPLY:
            mg_debug("rescode: %d, total received bytes: %d, body size: %d\n", 
                                                hm->resp_code, nc->recv_mbuf.len, hm->body.len);
            siWriteLen = write_operation(hm, write_callback, pstConnClient);
            if (hm->resp_code == 200) {
                pstMgrClient->get_success_count++;
            } else {
                pstMgrClient->get_failure_count++;
            }
            nc->flags |= MG_F_DELETE_CHUNK;
            nc->flags |= MG_F_SEND_AND_CLOSE;
            s_received_signal = 1;
            break;
        case MG_EV_TIMER:
            printf("%s\n", "connect timeout");
            pstMgrClient->conn_timeout_count++;
            nc->flags |= MG_F_CLOSE_IMMEDIATELY;
            s_received_signal = 1;
        case MG_EV_CLOSE:
            printf("%s\n", "close connection with sever");
            pstMgrClient->conn_close_count++;
            break;
        default:
            break;
    }
}

#define USAGE                      \
    "./restful_server <server ip> <server port> <service id>\n" \
    "  service id options:\n"      \
    "    0 - Url Not Found\n"      \
    "    1 - Transfer-Encoding\n"  \
    "    2 - Content-Length\n"     \
    "    3 - Server Error\n"       \
    "    4 - Time Out\n"

int main(int argc, char *argv[])
{
    int ret = -1;
    struct mg_mgr stMgr;
    struct mg_connection *pstNc = NULL;
    struct mg_mgr_client *pstMgrClient = NULL;
    struct mg_conn_client *pstConnClient = NULL;
    int iOption = 0;
    
    if (argc != 4) {
        printf(USAGE);
        return -1;
    }
    
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    
    iOption = atoi(argv[3]);
    
    pstMgrClient = (struct mg_mgr_client *)calloc(1, sizeof(struct mg_mgr_client));
    if (pstMgrClient == NULL) {
        printf("calloc(struct mg_mgr_client) failed\n");
        goto cleanup;
    }

    pstConnClient = (struct mg_conn_client *)calloc(1, sizeof(struct mg_conn_client));
    if (pstConnClient == NULL) {
        printf("calloc(struct mg_conn_client) failed\n");
        goto cleanup;
    }
    
#ifdef _DEBUG
        pstConnClient->pf = fopen("out.file", "wb");
#endif
    
    mg_mgr_init(&stMgr, pstMgrClient);
    pstNc = mg_connect_http(&stMgr, MG_CB(ev_handler, pstConnClient), url[iOption], NULL, NULL);
    mg_set_timer(pstNc, mg_time() + 2);
    mg_set_protocol_http_websocket(pstNc);

    printf("starting restful client against %s\n", url[iOption]);
    while (s_received_signal == 0) {
        mg_mgr_poll(&stMgr, 1000);
    }
    
#ifdef _DEBUG
        size_t i = 0;
        FILE *pf = NULL;
        size_t n, offset, fsize, len;
        fsize = pstConnClient->buffer_pos;
        mg_debug("===== ===== will write %d bytes ===== =====\n", fsize);
        pf = fopen("buf.file", "wb");
        for (offset = 0; offset < fsize; offset += 1024) {
            len = (fsize - offset > 1024) ? 1024 : (fsize - offset);
            if (fwrite(pstConnClient->buffer + offset, 1, len, pf) != len) {
                printf("fwrite failed\n");
                goto cleanup;
            }
        }
        if (pf) fclose(pf);
#endif
    
    ret = 0;
cleanup:
    printf("%s\n", "free the source");
#ifdef _DEBUG
    if (pstConnClient->pf) fclose(pstConnClient->pf);
#endif
    if (pstConnClient->buffer) free(pstConnClient->buffer);
    if (pstConnClient) free(pstConnClient);
    if (pstMgrClient) free(pstMgrClient);
    
    mg_mgr_free(&stMgr);

    return ret;
}