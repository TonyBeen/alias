#ifndef __KCP_INC_H__
#define __KCP_INC_H__

#include <kcp_def.h>

#if defined(OS_WINDOWS)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#elif defined(OS_LINUX)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/time.h>

#ifndef SO_REUSEPORT
#define SO_REUSEPORT 15
#endif

#endif

#endif // __KCP_INC_H__
