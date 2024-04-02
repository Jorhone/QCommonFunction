#include "FileConfiguration.h"

#include <QSettings>
#include <QFile>
#include <QDir>

void CFileConfiguration::SetConfFile(const QString &vConfPath, const QString &vConfName)
{
    m_ConfigFilePath = vConfPath;
    m_ConfigFileName = vConfName;
}

QString CFileConfiguration::GetConfFilePath()
{
    return m_ConfigFilePath;
}

QString CFileConfiguration::GetConfFileName()
{
    return m_ConfigFileName;
}

void CFileConfiguration::ClearConfCache()
{
    m_ConfigFilePath.clear();
    m_ConfigFileName.clear();
    m_ConfValuePairList.clear();
}

void CFileConfiguration::SetConfValue(const QString &vConfKey, const QVariant &vConfValue)
{
    for(auto& tConfValuePair: m_ConfValuePairList)
    {
        QString tConfKey = tConfValuePair.first;
        if(tConfKey != vConfKey)
            continue;

        tConfValuePair.second = vConfValue;
        return;
    }

    m_ConfValuePairList.push_back({vConfKey, vConfValue});
}

QVariant CFileConfiguration::GetConfValue(const QString &vConfKey, const QVariant &vDefaultValue)
{
    QVariant tConfValue = vDefaultValue;
    for(auto& tConfValuePair: m_ConfValuePairList)
    {
        QString tConfKey = tConfValuePair.first;
        if(tConfKey != vConfKey)
            continue;

        tConfValue = tConfValuePair.second;
        break;
    }

    return tConfValue;
}

bool CFileConfiguration::ConfRefresh(void)
{
    return false;
}

bool CFileConfiguration::ConfSave(void)
{
    return false;
}

CFileConfiguration::CFileConfiguration()
{
    ClearConfCache();
}

CFileConfiguration::~CFileConfiguration()
{

}

bool CFileConfiguration::CheckConfFileInfo()
{
    if(m_ConfigFilePath.isEmpty())
        return false;

    if(m_ConfigFileName.isEmpty())
        return false;

    return true;
}

bool CFileConfiguration::CheckConfFileExist()
{
    QString tConfigFilePath = m_ConfigFilePath + m_ConfigFileName;

    bool tIsExist = QFile::exists(tConfigFilePath);
    return tIsExist;
}

bool CIniFileConfiguration::ConfRefresh()
{
    if(!CheckConfFileInfo())
        return false;

    if(!CheckConfFileExist())
        return false;

    //清空配置缓存
    m_ConfValuePairList.clear();

    QString tConfigFilePath = m_ConfigFilePath + m_ConfigFileName;
    QSettings tSettings(tConfigFilePath, QSettings::IniFormat);
    tSettings.setIniCodec("UTF-8");

    QStringList tConfKeyList = tSettings.allKeys();
    for(auto tConfKey: tConfKeyList)
    {
        QVariant tConfValue = tSettings.value(tConfKey);
        m_ConfValuePairList.push_back({tConfKey, tConfValue});
    }

    return true;
}

bool CIniFileConfiguration::ConfSave()
{
    if(!CheckConfFileInfo())
        return false;

    QDir tDIR;
    if(!tDIR.exists(m_ConfigFilePath))
    {
//        bool tResult = tDIR.mkdir(m_ConfigFilePath);
        bool tResult = tDIR.mkpath(m_ConfigFilePath); //递归创建完整目录路径
        if(!tResult)
            return false;
    }

    QString tConfigFilePath = m_ConfigFilePath + m_ConfigFileName;
    QSettings tSettings(tConfigFilePath, QSettings::IniFormat);
    tSettings.setIniCodec("UTF-8");

    for(auto& tConfValuePair: m_ConfValuePairList)
    {
        QString tConfKey = tConfValuePair.first;
        QVariant tConfValue = tConfValuePair.second;

        tSettings.setValue(tConfKey, tConfValue);
    }

    tSettings.sync();
    return true;
}
