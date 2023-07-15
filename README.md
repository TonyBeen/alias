**utils：**

    .
    ├── include
    │   ├── alloc.h
    │   ├── any.hpp
    │   ├── bitmap.h
    │   ├── buffer.h
    │   ├── condition.h
    │   ├── convert.h
    │   ├── debug.h
    │   ├── errors.h
    │   ├── exception.h
    │   ├── file.h
    │   ├── function_traits.h
    │   ├── gcc.hpp
    │   ├── hash.h
    │   ├── map.h
    │   ├── map_node.h
    │   ├── mutex.h
    │   ├── rbtree_api.h
    │   ├── rbtree_base.h
    │   ├── refcount.h
    │   ├── sharedbuffer.h
    │   ├── singleton.h
    │   ├── string8.h
    │   ├── sysdef.h
    │   ├── thread.h
    │   ├── timer.h
    │   ├── types.hpp
    │   └── utils.h
    ├── Makefile
    ├── Readme.md
    └── src
        ├── alloc.cpp
        ├── bitmap.cpp
        ├── buffer.cpp
        ├── condition.cpp
        ├── convert.cpp
        ├── file.cpp
        ├── hash.cpp
        ├── map_node.cpp
        ├── mutex.cpp
        ├── rbtree_api.cpp
        ├── rbtree_base.cpp
        ├── sharedbuffer.cpp
        ├── string8.cpp
        ├── thread.cpp
        ├── timer.cpp
        └── utils.cpp

    utils包含线程、互斥锁、条件变量、信号量、字符串、缓存、函数堆栈、单例、定时器、异常、红黑树Map、共享计数。
    依赖：libpthread

**log:**

    .
    ├── callstack.cpp
    ├── callstack.h
    ├── logcat.cc
    ├── log.cpp
    ├── log_event.h
    ├── log_format.cpp
    ├── log_format.h
    ├── log.h
    ├── log_level.h
    ├── log_main.cpp
    ├── log_main.h
    ├── log_write.cpp
    ├── log_write.h
    ├── Makefile
    ├── readme.md
    └── testlog.cc

    日志类
    只需包含log.h即可使用
    用法：定义一个字符串宏LOG_TAG; 使用LOGI输出; 具体可参考testlog.cc
    依赖: libpthread libunwind
    样式：
    日志输出时间(到毫秒) 父进程 当前线程 日志级别 日志tag: 具体日志信息
    07-11 15:01:26.751  9040  9040 [I] Test: xxxxxxxxxxxxxxx
    07-11 15:01:26.751  9040  9041 [I] Test: xxxxxxxxxxxxxxx
    callstack类用于获取当前线程的堆栈
    log独立于其他模块, 可以直接使用make install安装，提供sudo权限把动态库移动到/usr/lib/下

**crypto:**

    .
    ├── aes.cpp
    ├── aes.h
    ├── base64.cpp
    ├── base64.h
    ├── crypto.h
    ├── Makefile
    ├── md5.cpp
    ├── md5.h
    ├── rsa.cpp
    ├── rsa.h
    ├── sha.cpp
    ├── sha.h
    └── test
        ├── for_base64_test.txt
        ├── for_md5_test.jpg
        ├── private.key
        ├── public.key
        ├── test_aes.cc
        ├── test_base64.cc
        ├── test_md5.cc
        ├── test_rsa.cc
        └── test_sha.cc
    
    加解密类
    包含对应的头文件，编译时加上 -leularcrypto
    依赖于openssl
