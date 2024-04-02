#ifndef ETHERNETFRAMEHANDLERDEFINE_H
#define ETHERNETFRAMEHANDLERDEFINE_H

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

//以太网帧固定头，标记了一帧数据的开始，在MAC收到数据包后会自动过滤掉
struct sEthernetFrameFixedHeader
{
    uint8_t Preamble[7] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};   //前导码，也称报头，用于使收发节点的时钟同步。物理层使用连续7个字节的0x55实现数据的同步。
    uint8_t StartFrameDelimiter = 0xD5; //帧起始定界符，用于区分前导码与数据段，固定为1字节的0xD5。
};

struct sEthernetFrameHeader
{
    uint8_t     DestinationMAC[6] = {0};    //目的MAC地址
    uint8_t     SourceMAC[6] = {0};         //源MAC地址
    uint16_t    TypeORLength = 0;           //数据包类型/长度
                                            // -- value < 0x0600，表示以太网帧数据段的长度
                                            // -- value > 0x0600，表示与以太网帧相关的MAC客户端协议的类型
                                            // -- -- 0x0800：IP协议(网际协议)
                                            // -- -- 0x0806：ARP协议(地址解析协议)
                                            // -- -- 0x8035：RARP协议(反向地址转换协议)
};

struct sEthernetFrame
{
    sEthernetFrameHeader    FrameHeader;
    QByteArray              PayloadArray;       //载荷数据段，46~1500字节
    uint32_t                FrameCheckSequence; //帧校验序列，确保数据的正确传输，在数据的尾部加入了4字节的循环冗余校验码(CRC)来检验数据是否传输错误。CRC数据校验从目的MAC地址开始。
};

#pragma pack(pop)

#endif // ETHERNETFRAMEHANDLERDEFINE_H
