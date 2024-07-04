**utils：**

    utils/
    ├── include
    │   └── utils
    │       ├── alloc.h
    │       ├── any.hpp
    │       ├── auto_clean.hpp
    │       ├── bimap.h
    │       ├── bimap.hpp
    │       ├── bitmap.h
    │       ├── buffer.h
    │       ├── buffer_stream.h
    │       ├── buffer_stream_utils.h
    │       ├── condition.h
    │       ├── convert.h
    │       ├── debug.h
    │       ├── elapsed_time.h
    │       ├── endian.hpp
    │       ├── errors.h
    │       ├── exception.h
    │       ├── file.h
    │       ├── gcc.hpp
    │       ├── hash.h
    │       ├── map.h
    │       ├── map_node.h
    │       ├── mutex.h
    │       ├── rbtree_api.h
    │       ├── rbtree_base.h
    │       ├── refcount.h
    │       ├── shared_buffer.h
    │       ├── singleton.h
    │       ├── singleton_object.h
    │       ├── string8.h
    │       ├── sysdef.h
    │       ├── thread.h
    │       ├── timer.h
    │       ├── types.hpp
    │       └── utils.h
    ├── Makefile
    ├── Readme.md
    └── src
        ├── alloc.cpp
        ├── bitmap.cpp
        ├── buffer.cpp
        ├── buffer_stream.cpp
        ├── condition.cpp
        ├── convert.cpp
        ├── elapsed_time.cpp
        ├── endian.cpp
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

    utils包含线程、互斥锁、条件变量、信号量、线程、字符串、缓存、单例、定时器、异常、红黑树、HashMap、共享计数。
    依赖：libpthread

**log:**

    log/
    ├── bench_liblog.cc
    ├── callstack.cpp
    ├── callstack.h
    ├── example.json
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
    ├── mutex.hpp
    ├── nlohmann
    │   └── json.hpp
    ├── README
    ├── test_callstack.cc
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

    crypto/
    ├── aes_openssl.cpp
    ├── aes_openssl.h
    ├── bak
    │   ├── md5.cpp
    │   └── md5.h
    ├── base64_chromium.cpp
    ├── base64_chromium.h
    ├── base64_openssl.cpp
    ├── base64_openssl.h
    ├── crc32.cpp
    ├── crc32.h
    ├── crc32_table.hh
    ├── crypto.h
    ├── detail
    │   ├── hmac.h
    │   ├── keccak.cpp
    │   ├── keccak.h
    │   ├── LICENSE
    │   ├── md5.cpp
    │   ├── md5.h
    │   ├── readme.md
    │   ├── sha1.cpp
    │   ├── sha1.h
    │   ├── sha256.cpp
    │   ├── sha256.h
    │   ├── sha3.cpp
    │   └── sha3.h
    ├── Makefile
    ├── md5_openssl.cpp
    ├── md5_openssl.h
    ├── rsa_openssl.cpp
    ├── rsa_openssl.h
    ├── sha_openssl.cpp
    ├── sha_openssl.h
    └── test
        ├── for_base64_test.txt
        ├── for_md5_test.jpg
        ├── test_aes.cc
        ├── test_base64.cc
        ├── test_chromium_base64.cc
        ├── test_crc32.cc
        ├── test_main.cc
        ├── test_md5.cc
        ├── test_rsa.cc
        └── test_sha.cc

    加解密类
    包含对应的头文件，编译时加上 -leularcrypto
    依赖于openssl
