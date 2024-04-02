#include "EthernetFrameHandler.h"
#include "EndianConvert.h"

sEthernetFrame CEthernetFrameHandler::ParsePacket(const QString &vPacket)
{
    sEthernetFrame tEthernetFrame;

    do
    {
        QByteArray tPacketArray = vPacket.toUtf8();
        if(tPacketArray.isEmpty())
            break;

        //确认数据报长度，以太网帧最小长度：64字节
        if(tPacketArray.length() < 64)
            break;

        //解包，取出后为网络字节序，需转成主机字节序
        int tPacketByteIndex = 0;

        //ethernet frame head: destination mac
        memcpy(tEthernetFrame.FrameHeader.DestinationMAC, tPacketArray.mid(tPacketByteIndex, 6), 6);
        tPacketByteIndex += 6;

        //ethernet frame head: source mac
        memcpy(tEthernetFrame.FrameHeader.SourceMAC, tPacketArray.mid(tPacketByteIndex, 6), 6);
        tPacketByteIndex += 6;

        //ethernet frame head: type or length
        uint8_t* tTypeORLengthPTR = reinterpret_cast<uint8_t*>(&tEthernetFrame.FrameHeader.TypeORLength);
        CEndianConvert::DataEndianConvert(tTypeORLengthPTR, CEndianConvert::GetLocalEndianType(), tPacketArray.mid(tPacketByteIndex, 2), CEndianConvert::e_Endian_Big, 2);
        tPacketByteIndex += 2;

        //ethernet frame payload
        uint32_t tPayloadLength = tPacketArray.length() - tPacketByteIndex - 4;
        tEthernetFrame.PayloadArray = tPacketArray.mid(tPacketByteIndex, tPayloadLength);

        //ethernet frame check sequence
        uint8_t* tFrameCheckSequencePTR = reinterpret_cast<uint8_t*>(&tEthernetFrame.FrameCheckSequence);
        CEndianConvert::DataEndianConvert(tFrameCheckSequencePTR, CEndianConvert::GetLocalEndianType(), tPacketArray.right(4), CEndianConvert::e_Endian_Big, 4);

    }while(0);

    return tEthernetFrame;
}

QByteArray CEthernetFrameHandler::FormatPacket(const sEthernetFrame &vFrameData, bool vIsSent)
{
    uint64_t tPacketBufferLen = sizeof(vFrameData.FrameHeader) + vFrameData.PayloadArray.length() + sizeof(vFrameData.FrameCheckSequence);
    if(vIsSent)
        tPacketBufferLen += sizeof(sEthernetFrameFixedHeader);

    uint8_t* tPacketBuffer = new uint8_t[tPacketBufferLen];
    uint8_t* tPacketPTR = tPacketBuffer;
    uint64_t tPacketByteIndex = 0;

    //add ethernet frame fixed head while is sent
    if(vIsSent)
    {
        sEthernetFrameFixedHeader tEthernetFrameFixedHeader;
        uint32_t tFrameFixedHeaderLen = sizeof(tEthernetFrameFixedHeader);
        memcpy((tPacketPTR + tPacketByteIndex), &tEthernetFrameFixedHeader, tFrameFixedHeaderLen);
        tPacketByteIndex += tFrameFixedHeaderLen;
    }

    //ethernet frame head: destination mac
    memcpy((tPacketPTR + tPacketByteIndex), vFrameData.FrameHeader.DestinationMAC, 6);
    tPacketByteIndex += 6;

    //ethernet frame head: source mac
    memcpy((tPacketPTR + tPacketByteIndex), vFrameData.FrameHeader.SourceMAC, 6);
    tPacketByteIndex += 6;

    //ethernet frame head: type or length
    uint8_t* tTypeORLengthPTR = (uint8_t*)&vFrameData.FrameHeader.TypeORLength;
    CEndianConvert::DataEndianConvert((tPacketPTR + tPacketByteIndex), CEndianConvert::e_Endian_Big, tTypeORLengthPTR, CEndianConvert::GetLocalEndianType(), 2);
    tPacketByteIndex += 2;

    //ethernet frame payload
    uint32_t tPayloadLength = vFrameData.PayloadArray.length();
    if(tPayloadLength > 0)
    {
        memcpy((tPacketPTR + tPacketByteIndex), vFrameData.PayloadArray, tPayloadLength);
        tPacketByteIndex += tPayloadLength;
    }

    //ethernet frame check sequence
    uint8_t* tFrameCheckSequencePTR = (uint8_t*)&vFrameData.FrameCheckSequence;
    CEndianConvert::DataEndianConvert((tPacketPTR + tPacketByteIndex), CEndianConvert::e_Endian_Big, tFrameCheckSequencePTR, CEndianConvert::GetLocalEndianType(), 4);

    QByteArray tPacketArray = QByteArray((char*)tPacketBuffer, tPacketBufferLen);
    delete[] tPacketBuffer;

    return tPacketArray;
}
