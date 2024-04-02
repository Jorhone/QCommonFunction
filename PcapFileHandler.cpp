#include "PcapFileHandler.h"
#include <QStringList>

QStringList CPcapFileHandler::SplitPackets(QString &vDataBuffer, bool vHasGlobalPacket)
{
    QStringList tPacketList;

/*
    Pcap File:
        Global Packet
        Packet Header
        Packet Data
        Packet Header
        Packet Data
        ......
*/

    QByteArray tDataByteArray = vDataBuffer.toUtf8();
    if(vHasGlobalPacket)
    {
        uint32_t tDataByteArrayLen = tDataByteArray.length();
        uint32_t tPcapFileHeaderLen = sizeof(sPcapFileHeader);
        if(tDataByteArrayLen >= tPcapFileHeaderLen)
        {
            QByteArray tPacketArray = tDataByteArray.left(tPcapFileHeaderLen);
            tPacketList.push_back(tPacketArray);

            // 删除已取出的内容
            tDataByteArray.remove(0, tPcapFileHeaderLen);
        }
    }

    while(!tDataByteArray.isEmpty())
    {
        uint32_t tPacketArrayLen = sizeof(sPcapPacketHeader);
        //检查包头长度
        if((uint32_t)tDataByteArray.length() < tPacketArrayLen)
            break;

        uint32_t tCaplen = tDataByteArray.mid(8, 4).toUInt();
        tPacketArrayLen += tCaplen;
        //检查包长度
        if((uint32_t)tDataByteArray.length() < tPacketArrayLen)
            break;

        QByteArray tPacketArray = tDataByteArray.left(tPacketArrayLen);
        tPacketList.push_back(tPacketArray);

        // 删除已取出的内容
        tDataByteArray.remove(0, tPacketArrayLen);
    }

    return tPacketList;
}

sPcapPacket CPcapFileHandler::ParsePacket(const QString &vPacket)
{
    sPcapPacket tPcapPacket;

    do
    {
        QByteArray tPacketArray = vPacket.toUtf8();
        if(tPacketArray.isEmpty())
            break;

        uint32_t tPacketArrayLen = sizeof(sPcapPacketHeader);
        //确认包头长度
        if((uint32_t)tPacketArray.length() < tPacketArrayLen)
            break;

        uint32_t tCaplen = tPacketArray.mid(8, 4).toUInt();
        tPacketArrayLen += tCaplen;
        //检查包长度
        if((uint32_t)tPacketArray.length() < tPacketArrayLen)
            break;

        //解包
        int tPacketByteIndex = 0;

        //pcap packet head: timestamp second
        tPcapPacket.PacketHeader.TimestampSecond = tPacketArray.mid(tPacketByteIndex, 4).toUInt();
        tPacketByteIndex += 4;

        //pcap packet head: timestamp microsecond
        tPcapPacket.PacketHeader.TimestampMicrosecond = tPacketArray.mid(tPacketByteIndex, 4).toUInt();
        tPacketByteIndex += 4;

        //pcap packet head: caplen
        tPcapPacket.PacketHeader.Caplen = tPacketArray.mid(tPacketByteIndex, 4).toUInt();
        tPacketByteIndex += 4;

        //pcap packet head: len
        tPcapPacket.PacketHeader.Len = tPacketArray.mid(tPacketByteIndex, 4).toUInt();
        tPacketByteIndex += 4;

        //pcap packet data
        tPcapPacket.PacketData = tPacketArray.mid(tPacketByteIndex, tPcapPacket.PacketHeader.Caplen);

    }while(0);

    return tPcapPacket;
}

sPcapFileHeader CPcapFileHandler::ParseFileGlobalPacket(const QString &vPacket)
{
    sPcapFileHeader tPcapFileHeader;
    memset(&tPcapFileHeader, 0, sizeof(tPcapFileHeader));

    do
    {
        QByteArray tPacketArray = vPacket.toUtf8();
        if(tPacketArray.isEmpty())
            break;

        if((uint32_t)tPacketArray.length() < sizeof(sPcapFileHeader))
            break;

        //解包
        int tPacketByteIndex = 0;

        //pcap file head: magic
        tPcapFileHeader.Magic = tPacketArray.mid(tPacketByteIndex, 4).toUInt();
        tPacketByteIndex += 4;

        //pcap file head: version major
        tPcapFileHeader.VersionMajor = tPacketArray.mid(tPacketByteIndex, 2).toUInt();
        tPacketByteIndex += 2;

        //pcap file head: version minor
        tPcapFileHeader.VersionMinor = tPacketArray.mid(tPacketByteIndex, 2).toUInt();
        tPacketByteIndex += 2;

        //pcap file head: thiszone
        tPcapFileHeader.Thiszone = tPacketArray.mid(tPacketByteIndex, 4).toUInt();
        tPacketByteIndex += 4;

        //pcap file head: sigfigs
        tPcapFileHeader.Sigfigs = tPacketArray.mid(tPacketByteIndex, 4).toUInt();
        tPacketByteIndex += 4;

        //pcap file head: snaplen
        tPcapFileHeader.Snaplen = tPacketArray.mid(tPacketByteIndex, 4).toUInt();
        tPacketByteIndex += 4;

        //pcap file head: linktype
        tPcapFileHeader.Linktype = tPacketArray.mid(tPacketByteIndex, 4).toUInt();

    }while(0);

    return tPcapFileHeader;
}

QByteArray CPcapFileHandler::FormatPacket(const sPcapPacket &vPcapPacket)
{
    uint64_t tPacketBufferLen = sizeof(vPcapPacket.PacketHeader) + vPcapPacket.PacketHeader.Caplen;
    uint8_t* tPacketBuffer = new uint8_t[tPacketBufferLen];
    memset(tPacketBuffer, 0, tPacketBufferLen);

    uint8_t* tPacketPTR = tPacketBuffer;
    uint64_t tPacketByteIndex = 0;

    //pcap packet head: timestamp second
    uint8_t* tTimestampSecondPTR = (uint8_t*)&vPcapPacket.PacketHeader.TimestampSecond;
    memcpy((tPacketPTR + tPacketByteIndex), tTimestampSecondPTR, 4);
    tPacketByteIndex += 4;

    //pcap packet head: timestamp microsecond
    uint8_t* tTimestampMicrosecondPTR = (uint8_t*)&vPcapPacket.PacketHeader.TimestampMicrosecond;
    memcpy((tPacketPTR + tPacketByteIndex), tTimestampMicrosecondPTR, 4);
    tPacketByteIndex += 4;

    //pcap packet head: caplen
    uint8_t* tCaplenPTR = (uint8_t*)&vPcapPacket.PacketHeader.Caplen;
    memcpy((tPacketPTR + tPacketByteIndex), tCaplenPTR, 4);
    tPacketByteIndex += 4;

    //pcap packet head: len
    uint8_t* tLenPTR = (uint8_t*)&vPcapPacket.PacketHeader.Len;
    memcpy((tPacketPTR + tPacketByteIndex), tLenPTR, 4);
    tPacketByteIndex += 4;

    //pcap packet data
    uint32_t tPacketDataLen = vPcapPacket.PacketHeader.Caplen;
    if(tPacketDataLen > (uint32_t)vPcapPacket.PacketData.length())
        tPacketDataLen = vPcapPacket.PacketData.length();

    memcpy((tPacketPTR + tPacketByteIndex), vPcapPacket.PacketData, tPacketDataLen);

    QByteArray tPacketArray = QByteArray((char*)tPacketBuffer, tPacketBufferLen);
    delete[] tPacketBuffer;

    return tPacketArray;
}

QByteArray CPcapFileHandler::FormatFileGlobalPacket(const sPcapFileHeader &vPcapFileHeader)
{
    uint64_t tPacketBufferLen = sizeof(vPcapFileHeader);
    uint8_t* tPacketBuffer = new uint8_t[tPacketBufferLen];
    memset(tPacketBuffer, 0, tPacketBufferLen);

    uint8_t* tPacketPTR = tPacketBuffer;
    uint64_t tPacketByteIndex = 0;

    //pcap file head: magic
    uint8_t* tMagicPTR = (uint8_t*)&vPcapFileHeader.Magic;
    memcpy((tPacketPTR + tPacketByteIndex), tMagicPTR, 4);
    tPacketByteIndex += 4;

    //pcap file head: version major
    uint8_t* tVersionMajorPTR = (uint8_t*)&vPcapFileHeader.VersionMajor;
    memcpy((tPacketPTR + tPacketByteIndex), tVersionMajorPTR, 2);
    tPacketByteIndex += 2;

    //pcap file head: version minor
    uint8_t* tVersionMinorPTR = (uint8_t*)&vPcapFileHeader.VersionMinor;
    memcpy((tPacketPTR + tPacketByteIndex), tVersionMinorPTR, 2);
    tPacketByteIndex += 2;

    //pcap file head: thiszone
    uint8_t* tThiszonePTR = (uint8_t*)&vPcapFileHeader.Thiszone;
    memcpy((tPacketPTR + tPacketByteIndex), tThiszonePTR, 4);
    tPacketByteIndex += 4;

    //pcap file head: sigfigs
    uint8_t* tSigfigsPTR = (uint8_t*)&vPcapFileHeader.Sigfigs;
    memcpy((tPacketPTR + tPacketByteIndex), tSigfigsPTR, 4);
    tPacketByteIndex += 4;

    //pcap file head: snaplen
    uint8_t* tSnaplenPTR = (uint8_t*)&vPcapFileHeader.Snaplen;
    memcpy((tPacketPTR + tPacketByteIndex), tSnaplenPTR, 4);
    tPacketByteIndex += 4;

    //pcap file head: linktype
    uint8_t* tLinktypePTR = (uint8_t*)&vPcapFileHeader.Linktype;
    memcpy((tPacketPTR + tPacketByteIndex), tLinktypePTR, 4);

    QByteArray tPacketArray = QByteArray((char*)tPacketBuffer, tPacketBufferLen);
    delete[] tPacketBuffer;

    return tPacketArray;
}
