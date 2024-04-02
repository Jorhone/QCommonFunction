#ifndef CETHERNETFRAMEHANDLER_H
#define CETHERNETFRAMEHANDLER_H

#include "EthernetFrameHandlerDefine.h"
#include <QString>

class CEthernetFrameHandler
{
public:
    /**
     * @brief ParsePacket   解析数据包
     * @param vPacket
     * @return
     */
    static sEthernetFrame ParsePacket(const QString& vPacket);

    /**
     * @brief FormatPacket  格式化数据包
     * @param vFrameData
     * @return
     */
    static QByteArray FormatPacket(const sEthernetFrame& vFrameData, bool vIsSent = true);

};

#endif // CETHERNETFRAMEHANDLER_H
