#ifndef CPCAPFILEHANDLER_H
#define CPCAPFILEHANDLER_H

#include "PcapFileHandlerDefine.h"
#include <QString>

class CPcapFileHandler
{
public:
    /**
     * @brief SplitPackets  拆分数据包
     * @param vDataBuffer   [in]数据缓存
     * @return              [out]返回数据单包列表，如果存在全局数据包，则列表首个字符串为全局数据包
     */
    static QStringList SplitPackets(QString& vDataBuffer, bool vHasGlobalPacket = true);

    /**
     * @brief ParsePacket   解析数据包
     * @param vPacket
     * @return
     */
    static sPcapPacket ParsePacket(const QString& vPacket);

    /**
     * @brief ParseFileGlobalPacket 解析全局数据包
     * @param vPacket
     * @return
     */
    static sPcapFileHeader ParseFileGlobalPacket(const QString& vPacket);

    /**
     * @brief FormatPacket  格式化数据包
     * @param vFrameData
     * @return
     */
    static QByteArray FormatPacket(const sPcapPacket& vPcapPacket);

    /**
     * @brief FormatGlobalPacket    格式化全局数据包
     * @return
     */
    static QByteArray FormatFileGlobalPacket(const sPcapFileHeader& vPcapFileHeader);
};

#endif // CPCAPFILEHANDLER_H
