**<font face="courier new"> utils：</font>**

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
    
> <font face="courier new"> utils包含线程、互斥锁、条件变量、字符串、缓存、函数堆栈。</font>
> **<font face="courier new"> dependence：</font>**
> <font face="courier new"> linunwind libaliaslog libpthread </font>

> linunwind: github下载;
> libaliaslog: 下面的log库; 

**<font face="courier new"> log：</font>**

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

> <font face="courier new"> 日志类 </font>

> <font face="courier new"> 只需包含log.h即可使用 </font>

> <font face="courier new"> 用法：定义一个字符串宏LOG_TAG; LOGI(const char *fmt,...); 与printf用法一致 </font>

> **<font face="courier new"> dependecce: </font>**

> <font face="courier new"> libpthread </font>

> <font face="courier new"> 样式： </font>

> <font face="courier new"> 07-11 15:01:26.751  9040  9040 [I] Test: main-0 </font>

> <font face="courier new"> 07-11 15:01:26.751  9040  9041 [I] Test: thread-00000 </font>

> <font face="courier new"> log独立于其他模块, 可以直接使用make install安装，提供sudo权限把动态库移动到/usr/lib/下</font>
