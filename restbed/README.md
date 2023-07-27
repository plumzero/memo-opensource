
### 说明
- 添加一些测试文件
- 测试过程中可以使用 tcpdump 或 wireshark 进行抓包分析

01.GET 方法测试
- 测试文件
    + getclient.cpp
    + getserver.cpp
- 测试说明
    + 同步请求&响应
    + 长连接测试
- 测试内容
    + GET 方法
    + 客户端发送 Cookie
    + 服务端返回 Content, 客户端收到后取出
    + 也可以通过下列命令单独对 getserver 测试；
      ```shell
        curl -w'\n' -v -X GET http://localhost:1984/resource/fangyi
      ```

02.POST 方法测试
- 测试文件
    + postclient.cpp
    + postserver.cpp
- 测试说明
    + 同步请求&响应
    + 长连接测试
- 测试内容
    + POST 方法
    + 客户端发送 Cookie
    + 服务端返回 Content, 客户端收到后取出
    
03.POST 方法效率测试
- 测试文件
    + efficlient.cpp
    + effiserver.cpp
- 测试说明
    + 同步请求&响应
    + 长连接测试
    + 在客户端处进行结果统计
- 测试内容
    + POST 方法
    + 客户端发送 Cookie
    + 服务端返回 Content, 客户端收到后取出
    + 统计事务数
    
04.客户端多类型 URI 请求及响应测试
- 测试文件
    + multiclient.cpp
    + multiserver.cpp
- 测试说明
    + 同步请求&响应
    + 长连接测试
    + 在客户端处进行结果统计
    + POST 方法
    + 客户端发送 Cookie
    + 服务端返回 Content, 客户端收到后取出

05.服务端开启线程内端口处理
- 测试文件
    + threadclient.cpp
    + threadserver.cpp
- 测试说明
    + 同步请求&响应
    + 长连接测试
    + POST 方法
    + 客户端发送 Cookie
    + 服务端返回 Content, 客户端收到后取出
    
06.客户端向服务端发送 body, 服务端收到后解析测试
- 测试文件
    + bodyclient.cpp
    + bodyserver.cpp
- 测试说明
    + 同步请求&响应
    + 长连接测试
    + POST 方法
    + 客户端发送 body
    + 服务端收到 body 后解析，随后返回一个 body2
    
07.客户端向服务端发送 body(json格式), 服务端收到后解析测试
- 测试文件
    + jsonclient.cpp
    + jsonserver.cpp
- 测试说明
    + 同步请求&响应
    + 长连接测试
    + POST 方法
    + 客户端发送 body
    + 服务端收到 body 后解析，随后返回一个 body2
