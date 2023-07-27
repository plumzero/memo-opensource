
https://github.com/cesanta/mongoose

说明:
  网卡与虚拟地址等价，理解为网卡更好一些
  网卡 = 虚拟地址 = 接口网卡 = interface

/**
 * 宏
 */ 
// http 回应到达，被结构化为 struct http_message 
#define MG_EV_HTTP_REPLY 101   /* struct http_message * */

struct http_message {
  struct mg_str message; /* 整个消息: 请求行 + 包头 + 包体 */
  struct mg_str body;    /* 包体. 0-length for requests with no body */
  /* 请求行或响应行内容 */
  struct mg_str method; /* 请求方法 */
  struct mg_str uri;    /* 资源 */
  struct mg_str proto;  /* 协议版本 */
  /* 对于响应类型消息，分别表示状态码及状态信息 */
  int resp_code;
  struct mg_str resp_status_msg;
  /*
   * Query-string part of the URI. For example, for HTTP request
   *    GET /foo/bar?param1=val1&param2=val2
   *    |    uri    |     query_string     |
   *
   * Note that question mark character doesn't belong neither to the uri,
   * nor to the query_string
   */
  struct mg_str query_string;
  /* Headers */
  struct mg_str header_names[MG_MAX_HTTP_HEADERS];
  struct mg_str header_values[MG_MAX_HTTP_HEADERS];
};

struct mg_bind_opts {
  void *user_data;           /* Initial value for connection's user_data */
  unsigned int flags;        /* 可以对 struct mg_connection::flags 进行设置 */
  const char **error_string; /* Placeholder for the error string */
  struct mg_iface *iface;    /* Interface instance */
#if MG_ENABLE_SSL
  const char *ssl_cert;
  const char *ssl_key;
  const char *ssl_ca_cert;
  const char *ssl_cipher_suites;
#endif
};

/**
 * 活跃连接结构体
 */
struct mg_connection {
  struct mg_connection *next, *prev; /* mg_mgr::active_connections linkage */
  struct mg_connection *listener;    /* Set only for accept()-ed connections */
  struct mg_mgr *mgr;                /* Pointer to containing manager */

  sock_t sock;             /* 与对端通信的本地 socket */
  int err;
  union socket_address sa; /* 对端通信地址 */
  size_t recv_mbuf_limit;  /* 本地接收缓冲限制 */
  struct mbuf recv_mbuf;   /* 本地已经接收数据 */
  struct mbuf send_mbuf;   /* 本地预备发送数据 */
  time_t last_io_time;     /* Timestamp of the last socket IO */
  double ev_timer_time;    /* Timestamp of the future MG_EV_TIMER */

  mg_event_handler_t proto_handler; /* Protocol-specific event handler */
  void *proto_data;                 /* Protocol-specific data */
  void (*proto_data_destructor)(void *proto_data);
  mg_event_handler_t handler; /* 事件处理句柄 */
  void *user_data;            /* 用户自有参数 */

  void *mgr_data;          /* 事件管理器参数 */
  struct mg_iface *iface;  /* 对应网卡 */
  unsigned long flags;     /* 连接属性: 监听，协议类型，长短连接，是否http */
};

/**
 * 事件管理器，用于管理本地与所有远程的 http 连接
 * ifaces 中使用 struct mg_iface_vtable 保存每张网卡对应的操作方法，这些方法由 mg_ifaces 保存。
 * 对于常用的几种网卡，框架有在 mongoose.c 中对操作方法进行了定义，并设置为名称为 
 * mg_default_iface_vtable 的缺省操作方法（一共 5 种）
 */
struct mg_mgr {
  struct mg_connection *active_connections; /* 活跃连接列表 */
  void *user_data;                          /* 用户个性数据 */
  int num_ifaces;                           /* 虚拟地址数量 */
  int num_calls;
  struct mg_iface **ifaces;                 /* 虚拟地址列表 */
  const char *nameserver;                   /* DNS 解析地址 */
};

/**
 * 初始化事件管理器。主要是设备的初始化，如网卡。
 * @param mgr
 * @param user_data   一般传入 NULL
 */
void mg_mgr_init(struct mg_mgr *mgr, void *user_data);

/**
 * （多作为客户端，）创建一条与远端的 http 连接，并执行请求。根据响应结果执行相应回调。
 * @param mgr
 * @param MG_CB          事件回调
 * @param url            远端 url
 * @param extra_headers  http 请求头，格式如 "User-Agent: my-app\r\n"
 * @param post_data      为空时代表 GET 请求，否则是 POST 请求。如果是表单提交，请求头需要设置 'Content-Type'
 * @return 失败返回 NULL
 * @examples
 *   nc1 = mg_connect_http(mgr, ev_handler_1, "http://www.google.com", NULL, NULL);
 *   nc2 = mg_connect_http(mgr, ev_handler_1, "https://github.com", NULL, NULL);
 *   nc3 = mg_connect_http(mgr, ev_handler_1, "my_server:8000/form_submit/",
 *                         "Content-Type: application/x-www-form-urlencoded\r\n",
 *                         "var_1=value_1&var_2=value_2");
 * @illu
 *   如果需要设置一些连接选项，如:
 *     1. 提供给服务器的客户端证书;
 *     2. 设置对服务端身份进行验证时的 ca 证书;
 *     3. 本端所支持的密码套件;
 *     4. 对服务端证书的 CN 项(服务器名称)进行验证等
 *   时，通过结构体 struct mg_connect_opts 对选项进行设置，并调用 mg_connect_http_opt 函数。
 */
struct mg_connection *mg_connect_http(
    struct mg_mgr *mgr,
    MG_CB(mg_event_handler_t event_handler, void *user_data), const char *url,
    const char *extra_headers, const char *post_data);

/**
 * 循环调用此函数，检测所有连接中可执行 I/O 操作数，并返回此数量。
 * 一旦连接中有 IO-ready，此函数将触发对应连接事件处理句柄。
 * @param mgr
 * @param milli   最大超时等待，毫秒计
 *  
 * @illu
 *   每个网卡上绑定有多个套接字，mg_mgr_poll 会调用每个网卡上对应的轮询函数，对每个网卡上的套接字动作进行判断，
 *   并触发每条活跃连接上的动作。
 */
int mg_mgr_poll(struct mg_mgr *mgr, int milli);

/**
 * 反初始化事件管理器。关闭并释放所有活跃连接。
 */
void mg_mgr_free(struct mg_mgr *mgr);

/**
 * 创建一个监听连接。如果在本地监听时，可以像这样传入 ":8000"，如果需要绑定到一张接口网卡，则
 * 需要指定其 ip，如 "1.2.3.4:8000"。
 * 缺省时为 tcp 连接，创建 udp 连接时需要指定前缀，如 "udp://:8000"
 * @param mgr
 * @param address    监听地址。格式为 [PROTO://][IP_ADDRESS]:PORT 其中 PROTO 可取 'tcp' 或 'udp'
 * @param MG_CB
 * @param opts       struct mg_bind_opts 仅供 mg_bind_opt 函数使用。
 */
struct mg_connection *mg_bind_opt(struct mg_mgr *mgr, const char *address,
                                  MG_CB(mg_event_handler_t handler, void *user_data),
                                  struct mg_bind_opts opts);

/**
 * 为指定活跃连接绑定 http 事件处理句柄。
 * 用户自定义事件处理句柄: 
 *   void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
 * 用户自定义事件处理句柄将会收到以下额外事件:
 *   MG_EV_HTTP_REQUEST: 
 *     本端接收到 http 请求。可以将请求数据(ev_data)结构化为 http_message 进行处理；
 *   MG_EV_HTTP_REPLY: 
 *     接收到对本端之前所发送请求的回应。可以将回应数据(ev_data)结构化为 http_message 进行处理；
 *   MG_EV_HTTP_CHUNK: 
 *     http chunk 编码块到达。将 ev_data 结构化为 http_message, http_message::body 会包含不完整的，需要重组的，
 *     需要重组的 http 包体。随着新块的到达，包体逐渐增加，这可能会消耗可观内存，因此随着新块的不断到达，事件
 *     处理句柄可以这样处理包体: 
 *        设置 mg_connection::flags 的 MG_F_DELETE_CHUNK, 以通知 Mongoose 删除处理过的包体。当最后一个零块到达
 *        时，Mongoose 发送一个 MG_EV_HTTP_REPLY 事件告知是否已经删除内存编码块。
 *   MG_EV_WEBSOCKET_HANDSHAKE_REQUEST:
 *     服务器已经接收到本端发送的 WebSocket 握手请求。ev_data 包含解析后的 http 请求。
 *   MG_EV_WEBSOCKET_HANDSHAKE_DONE:
 *     服务器已经完成 WebSocket 握手。ev_data 是一个 struct http_message, 包含了客户端的请求(server mode)
 *     或者服务端的回应(client mode)。如果是 clinet mode 时，处理句柄可以检测 resp_code, 此情形下应为 101。
 *     将 server mode 解释为响应端，将 clinet mode 解释为请求端，更好一些。
 *   MG_EV_WEBSOCKET_FRAME:
 *      新的 WebSocket 帧到达，ev_data 是 struct websocket_message *
 *   
 *   其他，略
 *
 * 关于分块编码的应用场景及使用方法，参考
 *   HTTP 之chunked 编码
 *   https://blog.csdn.net/u012175637/article/details/82467130
 */
void mg_set_protocol_http_websocket(struct mg_connection *nc);

/**
 * 获取 http 表单变量。
 * 从 buf 中获取名称为 name 的表单变量，存入 dst 中。
 * @return 获取的表单变量的长度。如果未找到，则返回 0。
 */
int mg_get_http_var(const struct mg_str *buf, const char *name, char *dst,
                    size_t dst_len);

/**
 * 服务器根据 opts 提供给定的 http 请求。
 * 示例代码段:
 * static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *   struct http_message *hm = (struct http_message *) ev_data;
 *   struct mg_serve_http_opts opts = { .document_root = "/var/www" };  // C99
 *
 *   switch (ev) {
 *     case MG_EV_HTTP_REQUEST:
 *       mg_serve_http(nc, hm, opts);
 *       break;
 *     default:
 *       break;
 *   }
 * }
 */ 
void mg_serve_http(struct mg_connection *nc, struct http_message *hm,
                   struct mg_serve_http_opts opts);

/**
 * 将缓冲 buf 中 len 长度的数据通过 http 分块编码的形式发往客户端。
 * mg_send_http_chunk(nc, "foo", 3) 会将 '3\r\nfoo\r\n' 追加到 nc->send_mbuf 中，输出到 IO 缓存。
 * 注意: 
 *   调用此函数前，需要设置 http 属性 "Transfer-Encoding: chunked"
 *   在响应的最后要再发一个空编码块，告知客户端发送完毕。
 *     mg_printf_http_chunk(nc, "%s", "my response!");
 *     mg_send_http_chunk(nc, "", 0);                 // 通知客户端发送完毕
 */
void mg_send_http_chunk(struct mg_connection *nc, const char *buf, size_t len);
/**
 * 发送可打印形式的 http 块。
 * 功能与 mg_send_http_chunk() 相似，只是多了可变参数，以及只能发送可视化字符。
 */
void mg_printf_http_chunk(struct mg_connection *nc, const char *fmt, ...);

/**
 * 向连接的另一端发送 printf 风格的格式化数据。
 */
int mg_printf(struct mg_connection *, const char *fmt, ...);

/**
 * 功能类似于 strcmp() 函数。
 */
int mg_vcmp(const struct mg_str *str2, const char *str1);