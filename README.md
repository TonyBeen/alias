**utils：**

    .
    ├── crypto
    │   ├── aes.cpp
    │   ├── aes.h
    │   ├── base64.cpp
    │   ├── base64.h
    │   ├── crypto.h
    │   ├── Makefile
    │   ├── md5.cpp
    │   ├── md5.h
    │   ├── rsa.cpp
    │   ├── rsa.h
    │   ├── sha.cpp
    │   ├── sha.h
    │   └── test
    │       ├── for_base64_test.txt
    │       ├── for_md5_test.jpg
    │       ├── private.key
    │       ├── public.key
    │       ├── test_aes.cc
    │       ├── test_base64.cc
    │       ├── test_md5.cc
    │       ├── test_rsa.cc
    │       └── test_sha.cc
    ├── log
    │   ├── callstack.cpp
    │   ├── callstack.h
    │   ├── logcat.cc
    │   ├── log.cpp
    │   ├── log_event.h
    │   ├── log_format.cpp
    │   ├── log_format.h
    │   ├── log.h
    │   ├── log_level.h
    │   ├── log_main.cpp
    │   ├── log_main.h
    │   ├── log_write.cpp
    │   ├── log_write.h
    │   ├── makefile
    │   ├── readme.md
    │   └── testlog.cc
    ├── README.md
    ├── sqlutils
    │   ├── Makefile
    │   ├── mysql.cpp
    │   ├── mysql.h
    │   ├── redis.cpp
    │   ├── redis.h
    │   ├── sql_base.h
    │   ├── test.jpg
    │   ├── test_mysql.cc
    │   └── test_redis.cc
    ├── test
    │   ├── makefile
    │   ├── testbuffer.cc
    │   ├── testcallstack.cc
    │   ├── testlist.cc
    │   ├── testmutex.cc
    │   ├── test_rbtree.cc
    │   ├── test_rwmutex.cc
    │   ├── test_singleton.cc
    │   ├── teststring8_2.cc
    │   ├── teststring8.cc
    │   ├── testthread.cc
    │   ├── test_timer.cc
    │   └── test_utils.cc
    ├── threadpool
    │   ├── Makefile
    │   ├── test_threadloop.cc
    │   ├── threadpool.cpp
    │   └── threadpool.h
    └── utils
        ├── include
        │   ├── Buffer.h
        │   ├── condition.h
        │   ├── debug.h
        │   ├── Errors.h
        │   ├── exception.h
        │   ├── mutex.h
        │   ├── rbtree.h
        │   ├── singlelist.h
        │   ├── singleton.h
        │   ├── string8.h
        │   ├── sysdef.h
        │   ├── threaddef.h
        │   ├── thread.h
        │   ├── timer.h
        │   └── utils.h
        ├── Makefile
        ├── Readme.md
        └── src
            ├── Buffer.cpp
            ├── condition.cpp
            ├── mutex.cpp
            ├── rbtree.cpp
            ├── string8.cpp
            ├── thread.cpp
            ├── timer.cpp
            └── utils.cpp
    
    utils包含线程、互斥锁、条件变量、字符串、缓存、函数堆栈。
    dependence：
    linunwind libaliaslog libpthread
    linunwind: github下载;
    libaliaslog: 下面的log库; 

**log：**

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
