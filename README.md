**utils：**

    .
    ├── include
    │   ├── Buffer.h
    │   ├── callstack.h
    │   ├── condition.h
    │   ├── debug.h
    │   ├── Errors.h
    │   ├── mutex.h
    │   ├── singlelist.h
    │   ├── string8.h
    │   ├── threaddef.h
    │   ├── thread.h
    │   └── utils.h
    ├── makefile
    ├── Readme.md
    └── src
        ├── Buffer.cpp
        ├── callstack.cpp
        ├── condition.cpp
        ├── mutex.cpp
        ├── string8.cpp
        ├── thread.cpp
        └── utils.cpp
    
    utils包含线程、互斥锁、条件变量、字符串、缓存、函数堆栈。
    dependence：
    linunwind libaliaslog libpthread
    linunwind: github下载;
    libaliaslog: 下面的log库; 

**log:**

    .
    ├── log.cpp
    ├── log_event.h
    ├── log_format.cpp
    ├── log_format.h
    ├── log.h
    ├── log_leavl.h
    ├── log_main.cpp
    ├── log_main.h
    ├── log_write.cpp
    ├── log_write.h
    ├── makefile
    └── testlog.cc

    日志类
    只需包含log.h即可使用
    用法：定义一个字符串宏LOG_TAG; LOGI(const char *fmt,...); 与printf用法一致
    dependecce:
    libpthread
    样式：
    07-11 15:01:26.751  9040  9040 [I] Test: main-0
    07-11 15:01:26.751  9040  9041 [I] Test: thread-00000
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
