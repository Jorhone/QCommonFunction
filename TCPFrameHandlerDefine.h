#ifndef TCPFRAMEHANDLERDEFINE_H
#define TCPFRAMEHANDLERDEFINE_H

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

struct sTCPFrameBaseHeader
{
    unsigned short  SourctPort;             //源端口
    unsigned short  DestinationPort;        //目的端口
    unsigned int    SequenceNumber;         //序列号
    unsigned int    AcknowledgmentNumber;   //确认号
    unsigned short  HeaderLength  :4,       //首部长度，指出TCP首部共有多少个4字节
                    Reserved      :3,       //Reserved
                    Nonce         :1,       //Nonce
                    CWR           :1,       //Congestion Window Reduced
                    ECE           :1,       //ECN-Echo
                    URG           :1,       //Urgent
                    ACK           :1,       //Acknowledgment
                    PSH           :1,       //Push
                    RST           :1,       //Reset
                    SYN           :1,       //Syn
                    FIN           :1;       //Fin
    unsigned short  Window;                 //窗口大小
    unsigned short  Checksum;               //校验和
    unsigned short  UrgentPointer;          //紧急指针
};

struct sTCPFrameHeader
{
    sTCPFrameBaseHeader BaseHeader;     //帧头，基础部分
    QByteArray          OptionArray;    //选项，可选部分
};

struct sTCPFrame
{
    sTCPFrameHeader FrameHeader;
    QByteArray      PayloadArray;   //帧载荷数据段
};

#pragma pack(pop)

#endif // TCPFRAMEHANDLERDEFINE_H
