#ifndef CENDIANCONVERT_H
#define CENDIANCONVERT_H

#include <QString>

class CEndianConvert
{
public:
    typedef enum
    {
        e_Endian_Little,    //小端模式
        e_Endian_Big,       //大端模式
    }eEndianType;

public:
    /**
     * @brief GetLocalEndianType    获取当前字节序类型
     * @return
     */
    static eEndianType GetLocalEndianType(void);

    /**
     * @brief DataEndianConvert     数据字节序转换
     * @param vDESTPTR              [in]目标数据指针，内存大小 必须 >= vSRCPTR
     * @param vDESTEndianType
     * @param vSRCPTR               [in]源数据指针，eg：unsigned short source;
     * @param vSRCEndianType
     * @param vByteNumber           [in]源数据类型字节数，eg：sizeof(unsigned short)
     * @return
     */
    static uint8_t* DataEndianConvert(uint8_t* vDESTPTR, eEndianType vDESTEndianType, const uint8_t* vSRCPTR, eEndianType vSRCEndianType, int vByteNumber);

    /**
     * @brief DataEndianConvert     数据字节序转换
     * @param vDESTPTR
     * @param vDESTEndianType
     * @param vSRCByteArray         [in]源数据数组
     * @param vSRCEndianType
     * @param vByteNumber
     * @return
     */
    static uint8_t* DataEndianConvert(uint8_t* vDESTPTR, eEndianType vDESTEndianType, const QByteArray& vSRCByteArray, eEndianType vSRCEndianType, int vByteNumber);
};

#endif // CENDIANCONVERT_H
