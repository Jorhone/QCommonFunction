#include "EndianConvert.h"

CEndianConvert::eEndianType CEndianConvert::GetLocalEndianType()
{
    uint32_t tTestData = 0x12345678;
    uint8_t* tTestDataPTR = reinterpret_cast<uint8_t*>(&tTestData);

    //大小端模式，仅影响数据在内存中的存储方式，数据的高低位字节不变，eg：高 0x12 0x34 0x56 0x78 低
    //小端模式：整数的低位字节存储在内存的低地址，eg：内存 0001 0002 0003 0004，数据 0x78 0x56 0x34 0x12
    //大端模式：整数的高位字节存储在内存的低地址，eg：内存 0001 0002 0003 0004，数据 0x12 0x34 0x56 0x78

    if(*tTestDataPTR == 0x78)
        return e_Endian_Little;

    return e_Endian_Big;
}

uint8_t *CEndianConvert::DataEndianConvert(uint8_t *vDESTPTR, eEndianType vDESTEndianType, const uint8_t *vSRCPTR, eEndianType vSRCEndianType, int vByteNumber)
{
    for (int i = 0; i < vByteNumber; i++)
    {
        int tSRCByteIndex = 0;
        int tDESTByteIndex = 0;

        if(vSRCEndianType == vDESTEndianType)
        {
            tSRCByteIndex = i;
            tDESTByteIndex = i;
        }
        else
        {
            tSRCByteIndex = i;
            tDESTByteIndex = vByteNumber - i - 1;
        }

        vDESTPTR[tDESTByteIndex] = vSRCPTR[tSRCByteIndex];
    }

    return vDESTPTR;
}

uint8_t *CEndianConvert::DataEndianConvert(uint8_t *vDESTPTR, eEndianType vDESTEndianType, const QByteArray &vSRCByteArray, eEndianType vSRCEndianType, int vByteNumber)
{
    uint8_t* tSRCBuffer = new uint8_t[vByteNumber];
    memset(tSRCBuffer, 0, vByteNumber);

    if(vSRCByteArray.length() >= vByteNumber)
        memcpy(tSRCBuffer, vSRCByteArray, vByteNumber);

    for (int i = 0; i < vByteNumber; i++)
    {
        int tSRCByteIndex = 0;
        int tDESTByteIndex = 0;

        if(vSRCEndianType == vDESTEndianType)
        {
            tSRCByteIndex = i;
            tDESTByteIndex = i;
        }
        else
        {
            tSRCByteIndex = i;
            tDESTByteIndex = vByteNumber - i - 1;
        }

        vDESTPTR[tDESTByteIndex] = tSRCBuffer[tSRCByteIndex];
    }

    delete[] tSRCBuffer;
    return vDESTPTR;
}
