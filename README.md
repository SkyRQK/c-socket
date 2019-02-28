# c-socket
c语言实现聊天室 私聊

两个文件 server3.cpp 服务端  danliao.cpp 客户端
common.conf 是配置文件 需要改的地方就是 server_port 改成你的服务端本地ip
本地ip 查看 linux下 ifconfig

下面编译服务端和客户端
客户端解析语句  g++ danliao.cpp common.c -o danliao
服务端解析语句  g++ server3.cpp common.c -o server3 -lpthread

最后先 执行 ./server3 服务端打开
再执行  ./danliao 客户端登入

你可以再起几个客户端 连接不同的配置文件 记得改名字 一个名字只能登一次 


@某人 你好
上面那种@某人后加一个空格在加消息就是私聊了 
输入一个# 就是返回当前在线人数 
某个客户端对应自己的接受文件 chat.log    tail -f chat.log 代码实现 实时查看
