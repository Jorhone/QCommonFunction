#ifndef PCAPFILEHANDLERDEFINE_H
#define PCAPFILEHANDLERDEFINE_H

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

struct sPcapFileHeader
{
    uint32_t Magic = 0xA1B2C3D4;    //标记文件开始
    uint16_t VersionMajor = 2;      //当前文件的主要版本号
    uint16_t VersionMinor = 4;      //当前文件的次要版本号
    uint32_t Thiszone = 0;          //当地的标准时间，如果用的是GMT则全零
    uint32_t Sigfigs = 0;           //时间戳的精度
    uint32_t Snaplen = 65535;       //最大的存储长度，设置所抓获的数据包的最大长度，如果所有数据包都要抓获，将值设置为65535
    uint32_t Linktype = 1;          //链路类型。解析数据包首先要判断它的 LinkType，所以这个值很重要。一般的值为1，即以太网常用的 LinkType
};

struct sPcapPacketHeader
{
    uint32_t TimestampSecond = 0;       //时间戳高位，精度：秒
    uint32_t TimestampMicrosecond = 0;  //时间戳低位，精度：微秒
    uint32_t Caplen = 0;                //到下一帧的偏移，表示数据段长度
    uint32_t Len = 0;                   //实际上下一帧的偏移，绝大多数情况下等于caplen，可能会小于caplen
};

struct sPcapPacket
{
    sPcapPacketHeader   PacketHeader;
    QByteArray          PacketData;
};

#pragma pack(pop)

#endif // PCAPFILEHANDLERDEFINE_H
