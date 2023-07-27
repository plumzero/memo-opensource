
#include "mongoose.h"

// #define _DEBUG

#ifdef _DEBUG
    #define mg_debug(format, ...) printf(format, __VA_ARGS__)
#else
    #define mg_debug(format, ...) do {} while(0)
#endif

static sig_atomic_t s_received_signal = 0;
static struct mg_serve_http_opts s_http_server_opts;

/// 参考
static char *http_response_header[] = {
    "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n",    // 分块编码传输
    "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n",             // Content-Length 发送
    "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n",   // 服务器内部错误
    "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n",      // 无法找到资源
    "HTTP/1.0 501 Not Implemented\r\nContent-Length: 0\r\n\r\n" // 暂不具有服务请求功能
};

/// 还是用 C++ 容器统计比较好一些，以后再改
/// 只统计失败的
typedef struct mg_mgr_server {
    int conn_accept_count;          // 接受的连接统计
    int conn_request_abnormal_count;// 接收到的异常请求统计
    int conn_handle_success_count;  // 请求处理成功的统计
    int conn_handle_failure_count;  // 请求处理失败的统计
    int conn_close_count;           // 关闭的连接统计
} mg_mgr_server;

typedef struct mg_conn_server {
    
} mg_conn_server;

/**
 *  restful 学习:
 *  ev_handler: 当连接上收到某客户端的信息时，restful server 会调用 ev_handler 进行处理
 *  服务端可用消息位:
 *    MG_EV_ACCEPT           http 连接请求，只有监听套接字会处理该消息
 *    MG_EV_HTTP_REQUEST     http 请求到达
 *    MG_EV_HTTP_REPLY       http 回复到达
 */

/**
 * 说明:
 * 1. 通过 Transfer-Encoding: chunked 方式向客户端发送数据的长度计算
 *    服务端采用多次追加写入到内存完毕后，再慢慢发送的发送数据方式，也就是说可以对同一个连
 *    接(nc)反复调用 mg_send_http_chunk 函数。
 *    例如:
 *    要发送 6697 个字节数据，每次最多追加 1024 个缓冲数据（由程序员决定），则至少需要追加
 *    7 次。每次追加写入之前，需要设置追加写入的消息头，如下
 *          char chunk_size[50];
 *          int n;
 *          n = snprintf(chunk_size, sizeof(chunk_size), "%lX\r\n", (unsigned long) len);
 *          mg_send(nc, chunk_size, n);
 *          mg_send(nc, buf, len);
 *          mg_send(nc, "\r\n", 2);
 *    每个消息头占用 6 个字节，共占用 42 个字节。
 *    另外，还需要加上 http 报头，以 
 *          "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
 *    为例，共 53 个字节。
 *    最后，还要加上通知客户端已经发送完成的 6 个字节。
 *          mg_send_http_chunk(nc, "", 0);
 *    所以，一共需要向客户端发送 6798 个字节。
 * 2. 服务端发送数据的方式
 *    每次追加写入的数据长度可以相同，但实际上每次发送的数据长度不一。也就是说，两者没有必然
 *    联系。所以为了提高效率，可以适当增加要追加的缓冲区的长度，以减少追加写入次数。而发送数
 *    据过程则无法优化。
 *          追加写入: A A A A B
 *          实际发送: a b c d e
 * 3. 哪端主动关闭连接
 *    建议服务端主动关闭连接
 */
/**
 * 测试
 * 1. 内存泄漏测试
 *      valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./restful_server 10.0.80.121 8090
 *    无泄漏
 * 2. 文件测试
 *    对采用 Transfer-Encoding 和 Content-Length 分别进行下列用例测试，均通过，效率相同。
 *    a. 3.8K 文本文件    pass
 *    b. 500K 文本文件    pass
 *    c. 12M  二进制文件  pass
 *
 */
 
static void signal_handler(int sig_num)
{
    printf("received signal %d, will exit the program\n", sig_num);
    signal(sig_num, signal_handler);
    s_received_signal = sig_num;
}

/** Transfer-Encoding 方式 */
int handle_micro_service_A(struct mg_connection *nc, struct http_message *hm)
{
    (void) hm;

    int ret = -1;
    FILE *fp = NULL;
    size_t len, offset, fsize;
    char buf[1024] = { 0 };         // 可适当增加该缓冲区长度
    
    fp = fopen("test.file", "rb");
    if (fp == NULL) {
        printf("fopen('test.file') failed\n");
        goto cleanup;
    }
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    for (offset = 0; offset < fsize; offset += 1024) {
        memset(buf, 0, sizeof(buf));
        len = (fsize - offset > 1024) ? 1024 : (fsize - offset);
        if (fread(buf, 1, len, fp) != len) {
            printf("fread failed\n");
            goto cleanup;
        }
        mg_send_http_chunk(nc, buf, len);
        mg_debug("append up to total %d bytes ...\n", nc->send_mbuf.len);
    }
    mg_send_http_chunk(nc, "", 0);
    nc->flags |= MG_F_DELETE_CHUNK;
    nc->flags |= MG_F_SEND_AND_CLOSE;
    mg_debug("total send %d bytes to client\n", nc->send_mbuf.len);
    
    ret = 0;
cleanup:
    if (fp) fclose(fp);
    
    return ret;
}

/** Content-Length 方式 */
int handle_micro_service_B(struct mg_connection *nc, struct http_message *hm)
{
    (void) hm;
    
    int ret = -1;
    
    FILE *fp = NULL;
    size_t len, offset, fsize;
    char buf[1024] = { 0 };         // 可适当增加该缓冲区长度
    
    fp = fopen("test.file", "rb");
    if (fp == NULL) {
        printf("fopen('test.file') failed\n");
        goto cleanup;
    }
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n", fsize);
    for (offset = 0; offset < fsize; offset += 1024) {
        memset(buf, 0, sizeof(buf));
        len = (fsize - offset > 1024) ? 1024 : (fsize - offset);
        if (fread(buf, 1, len, fp) != len) {
            printf("fread failed\n");
            goto cleanup;
        }
        mg_send_http_chunk(nc, buf, len);
        mg_debug("append up to total %d bytes ...\n", nc->send_mbuf.len);
    }
    mg_send_http_chunk(nc, "", 0);
    nc->flags |= MG_F_SEND_AND_CLOSE;
    mg_debug("total send %d bytes to client\n", nc->send_mbuf.len);
    
    ret = 0;
cleanup:
    if (fp) fclose(fp);
    
    return ret;
}

/** service error */
int handle_micro_service_C(struct mg_connection *nc, struct http_message *hm)
{
    int ret = -1;
    
    mg_debug("server error occured(url: %s)\n", hm->uri.p);
    mg_printf(nc, "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n");
    mg_send_http_chunk(nc, "", 0);
    nc->flags |= MG_F_SEND_AND_CLOSE;
    
    return ret;
}

/** 超时，算作处理失败 */
int handle_micro_service_D(struct mg_connection *nc, struct http_message *hm)
{
    (void) nc;
    (void) hm;

    struct timeval timeout = { 4, 0 };
    select(0, NULL, NULL, NULL, &timeout);
    
    return -1;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data, void *user_data)
{
    struct http_message *hm = (struct http_message *)ev_data;
    struct mg_mgr_server *pstMgrServer = (struct mg_mgr_server *)nc->mgr->user_data;
    struct mg_conn_server *pstConnServer = (struct mg_conn_server *)user_data;
    
    switch (ev)
    {   
        case MG_EV_ACCEPT:
            mg_debug("listening socket(%d) accept a connection\n", nc->sock);
            pstMgrServer->conn_accept_count++;
            break;
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->uri, "/api/v1/micro/service/A") == 0) {
                if (handle_micro_service_A(nc, hm) == 0) {
                    mg_debug("handle micro service %c success\n", 'A');
                    pstMgrServer->conn_handle_success_count++;
                } else {
                    mg_debug("handle micro service %c failed\n", 'A');
                    pstMgrServer->conn_handle_failure_count++;
                }
            } else if (mg_vcmp(&hm->uri, "/api/v1/micro/service/B") == 0) {
                if (handle_micro_service_B(nc, hm) == 0) {
                    mg_debug("handle micro service %c success\n", 'B');
                    pstMgrServer->conn_handle_success_count++;
                } else {
                    mg_debug("handle micro service %c failed\n", 'B');
                    pstMgrServer->conn_handle_failure_count++;
                }
            } else if (mg_vcmp(&hm->uri, "/api/v1/micro/service/C") == 0) {
                if (handle_micro_service_C(nc, hm) == 0) {
                    mg_debug("handle micro service %c success\n", 'C');
                    pstMgrServer->conn_handle_success_count++;
                } else {
                    mg_debug("handle micro service %c failed\n", 'C');
                    pstMgrServer->conn_handle_failure_count++;
                }
            } else if (mg_vcmp(&hm->uri, "/api/v1/micro/service/D") == 0) {
                if (handle_micro_service_D(nc, hm) == 0) {
                    mg_debug("handle micro service %c success\n", 'D');
                    pstMgrServer->conn_handle_success_count++;
                } else {
                    mg_debug("handle micro service %c failed\n", 'D');
                    pstMgrServer->conn_handle_failure_count++;
                }
            } else {
                pstMgrServer->conn_request_abnormal_count++;
                mg_debug("can't find resource(url=%s)\n", hm->uri.p);
                mg_serve_http(nc, hm, s_http_server_opts);
                mg_printf(nc, "%s", "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
                mg_send_http_chunk(nc, "", 0);
                nc->flags |= MG_F_SEND_AND_CLOSE;
            }
            break;      
        case MG_EV_CLOSE:
            printf("close connected socket(%d) with client\n", nc->sock);
            pstMgrServer->conn_close_count++;
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    int ret = -1;
    char szSrvAddr[256] = { 0 };
    const char *pcErrStr = NULL;
    struct mg_mgr stMgr;
    struct mg_connection *pstNc = NULL;
    struct mg_bind_opts stBindOpts;
    struct mg_mgr_server *pstMgrServer = NULL;
    
    if (argc != 3) {
        printf("./%s <server ip> <server port>\n", argv[0]);
        return -1;
    }
    
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    
    ret = snprintf(szSrvAddr, sizeof(szSrvAddr), "%s:%s", argv[1], argv[2]);
    if (ret < 0 || ret >= 256) {
        printf("snprintf error\n");
        return -1;
    }
    
    printf("server: %s\n", szSrvAddr);
    
    pstMgrServer = (struct mg_mgr_server *)calloc(1, sizeof(struct mg_mgr_server));
    if (pstMgrServer == NULL) {
        printf("calloc(struct mg_mgr_server) failed\n");
        goto cleanup;
    }
    
    mg_mgr_init(&stMgr, pstMgrServer);
    
    memset(&stBindOpts, 0, sizeof(stBindOpts));
    stBindOpts.error_string = &pcErrStr;
    pstNc = mg_bind_opt(&stMgr, szSrvAddr, MG_CB(ev_handler, NULL), stBindOpts);
    if (pstNc == NULL) {
        printf("mg_bind_opt: %s\n", *stBindOpts.error_string);
        goto cleanup;
    }
    
    mg_set_protocol_http_websocket(pstNc);
    s_http_server_opts.document_root = argv[0];
    s_http_server_opts.enable_directory_listing = "no";
    
    printf("start restful server on port %s, serving %s\n", argv[2], s_http_server_opts.document_root);
    while (s_received_signal == 0) {
        mg_mgr_poll(&stMgr, 1000);
    }
    
    printf("%s", "==================== statics ====================\n");
    printf("总的成功连接数: %d\n", pstMgrServer->conn_accept_count);
    printf("异常请求连接数: %d\n", pstMgrServer->conn_request_abnormal_count);
    printf("请求处理成功数: %d\n", pstMgrServer->conn_handle_success_count);
    printf("请求处理失败数: %d\n", pstMgrServer->conn_handle_failure_count);
    printf("总的关闭连接数: %d\n", pstMgrServer->conn_close_count);
    printf("%s", "==================== statics ====================\n");
    ret = 0;
cleanup:
    printf("%s\n", "free the source");
    mg_mgr_free(&stMgr);
    if (pstMgrServer) free(pstMgrServer);

    return ret;
}