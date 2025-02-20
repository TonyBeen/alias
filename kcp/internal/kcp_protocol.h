#ifndef __KCP_PROTOCOL_H__
#define __KCP_PROTOCOL_H__

#include <stdint.h>
#include <stddef.h>

#include "list.h"

#include "kcp_def.h"
#include "socket_set.h"
#include "kcp_config.h"
#include "kcpp.h"

#define KCP_HEADER_SIZE (24)

/// @brief KCP协议命令类型
enum KcpCommand {
    KCP_CMD_SYN = 1,    // SYN
    KCP_CMD_ACK,        // ACK
    KCP_CMD_PUSH,       // PUSH
    KCP_CMD_WASK,       // Window Probe (ask)
    KCP_CMD_WINS,       // Window Size (tell)
    KCP_CMD_PING,       // PING
    KCP_CMD_PONG,       // PONG
    KCP_CMD_MTU_PROBE,  // MTU probe
    KCP_CMD_MTU_ACK,    // MTU probe
    KCP_CMD_FIN,        // FIN
    KCP_CMD_RST,        // RST
};
typedef int32_t kcp_command_t;

enum KcpConnectionState {
    KCP_STATE_DISCONNECTED = 1,
    KCP_STATE_SYN_SENT,
    KCP_STATE_SYN_RECEIVED,
    KCP_STATE_CONNECTED,
    KCP_STATE_FIN_SENT,
    KCP_STATE_FIN_RECEIVED,
};
typedef int32_t kcp_connection_state_t;

/// @brief KCP报文段
typedef struct KcpSengment {
    struct list_head node; // 链表节点
    uint32_t conv;      // 会话ID
    uint32_t cmd;       // 命令
    uint32_t frg;       // 分片序号
    uint32_t wnd;       // 窗口大小
    uint32_t ts;        // 时间戳
    uint32_t sn;        // 序号
    uint32_t una;       // 未确认序号
    uint32_t len;       // 数据长度
    uint32_t resendts;  // 重传时间戳
    uint32_t rto;       // 超时重传时间
    uint32_t fastack;   // 快速重传
    uint32_t xmit;      // 重传次数
    char     data[1];   // 数据
} kcp_sengment_t;

typedef struct KcpAck {
    struct list_head node;
    uint32_t sn;    // 序号
    uint32_t ts;    // 时间戳
} kcp_ack_t;

/// @brief KCP控制块
typedef struct KcpSocket {
    // 基础配置
    uint32_t conv;          // 会话ID，用于标识一个会话
    uint32_t mtu;           // 最大传输单元
    uint32_t mss;           // 最大报文段大小，默认mtu-24字节
    uint32_t state;         // 连接状态，0=正常，-1=断开

    // 发送和接收序号
    uint32_t snd_una;       // 第一个未确认的包序号
    uint32_t snd_nxt;       // 下一个待发送的包序号
    uint32_t rcv_nxt;       // 待接收的下一个包序号

    // 时间戳相关
    uint32_t ts_recent;     // 最近一次收到包的时间戳
    uint32_t ts_lastack;    // 最近一次收到ACK的时间戳
    uint32_t ssthresh;      // 慢启动阈值，默认为IKCP_THRESH_INIT(2)

    // RTT相关
    int32_t rx_rttval;      // RTT 的偏差, 用于计算 RTT 的波动
    int32_t rx_srtt;        // 平滑的 RTT 值, 用于计算平均 RTT
    int32_t rx_rto;         // 超时重传时间，初始为 IKCP_RTO_DEF(200ms)
    int32_t rx_minrto;      // 最小重传超时时间，默认为 IKCP_RTO_MIN(100ms)

    // 窗口相关
    uint32_t snd_wnd;       // 发送窗口大小，默认32
    uint32_t rcv_wnd;       // 接收窗口大小，默认128
    uint32_t rmt_wnd;       // 远端窗口大小，默认128
    uint32_t cwnd;          // 拥塞窗口大小，初始为0
    uint32_t probe;         // 探测标志，用于窗口探测

    // 时间相关
    uint32_t current;       // 当前时间
    uint32_t interval;      // 内部更新时间间隔，默认100ms
    uint32_t ts_flush;      // 下次刷新时间
    uint32_t xmit;          // 总重传次数

    // 队列计数器
    uint32_t nrcv_buf;      // 接收缓存中的包数量
    uint32_t nsnd_buf;      // 发送缓存中的包数量
    uint32_t nrcv_que;      // 接收队列中的包数量
    uint32_t nsnd_que;      // 发送队列中的包数量

    // 配置标志
    uint32_t nodelay;      // 是否启用nodelay模式，0=不启用
    uint32_t updated;      // 是否调用过update

    // 探测相关
    uint32_t ts_probe;     // 下次探测时间
    uint32_t probe_wait;   // 探测等待时间

    // 链路控制
    uint32_t incr;         // 可发送的最大数据量

    // 数据队列
    struct list_head    snd_queue;  // 发送队列
    struct list_head    rcv_queue;  // 接收队列
    struct list_head    snd_buf;    // 发送缓存
    struct list_head    rcv_buf;    // 接收缓存

    // ACK相关
    kcp_ack_t   ack_item;   // ACK列表项
    kcp_ack_t   ack_unused; // 未使用的ACK列表项

    // 快速重传相关
    int fastresend;     // 触发快速重传的重复ACK个数
    int fastlimit;      // 快速重传次数限制，默认IKCP_FASTACK_LIMIT(5)

    // 其他配置
    int nocwnd;          // 是否关闭拥塞控制，0=不关闭
    int stream;          // 是否为流模式，0=消息模式(默认)，1=流模式
} kcp_socket_t;

typedef struct KcpFunctionCallback {
    on_kcp_accept_t     on_accepted;
    on_kcp_connected_t  on_connected;
    on_kcp_closed_t     on_closed;
} kcp_function_callback_t;

typedef struct KcpConnection {
    socket_set_node_t   socket_node;
    struct KcpContext*  kcp_ctx;
} kcp_connection_t;

struct KcpContext {
    socket_t                    sock;
    kcp_function_callback_t     callback;

    socket_set_t                socket_set;
    struct event_base*          event_base;
    void*                       user_data;
};

EXTERN_C_BEGIN

void kcp_socket_init(kcp_socket_t *kcp, uint32_t conv, void *user);

EXTERN_C_END

#endif // __KCP_PROTOCOL_H__
