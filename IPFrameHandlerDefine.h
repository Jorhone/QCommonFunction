#ifndef IPFRAMEHANDLERDEFINE_H
#define IPFRAMEHANDLERDEFINE_H

#include <QString>

/**
 * 数据类型定义
 */

//---------------------------------------------------------------------------

/**
 * 数据结构/类定义
 */

; //为了消除告警：warning: unterminated '#pragma pack (push, ...)' at end of file
#pragma pack(push, 1)

struct sIPFrameBaseHeader
{
    unsigned char   Version         :4,     //版本号，目前最广泛的是4=B1000，即常说的IPv4
                    HeaderLength    :4;     //首部长度，指出数据包首部共有多少个4字节
    unsigned char   ServiceType;            //服务类型，包括8个二进制位
    unsigned short  TotalLength;            //包裹总长，当前数据包的总长度，单位是字节
    unsigned short  Identification;         //重组标识，发送主机赋予的标识，以便接收方进行分片重组
    unsigned short  Flags           :3,     //标志，指定后续是否有连续段
                    FragmentOffset  :13;    //段偏移量，与更多段位组合，帮助接收方组合分段的报文，以字节为单位
    unsigned char   TimeToLive;             //生存时间，经常ping命令看到的TTL（Time To Live）就是这个，每经过一个路由器，该值就减一，到零丢弃
    unsigned char   Protocol;               //协议代码，表明使用该包裹的上层协议，如TCP=6，ICMP=1，UDP=17等
    unsigned short  HeaderChecksum;         //首部校验和
    unsigned int    SourceAddress;          //源地址，常见的IP是将每个字节用点（.）分开，如此而已
    unsigned int    DestinationAddress;     //目的地址
};

struct sIPFrameHeader
{
    sIPFrameBaseHeader  BaseHeader;     //帧头，基础部分
    QByteArray          OptionArray;    //选项，可选部分
};

struct sIPFrame
{
    sIPFrameHeader  FrameHeader;
    QByteArray      PayloadArray;   //帧载荷数据段
};

#pragma pack(pop)

#endif // IPFRAMEHANDLERDEFINE_H
