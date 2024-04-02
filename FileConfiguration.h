#ifndef FILECONFIGURATION_H
#define FILECONFIGURATION_H

#include <QVariant>
#include <QList>

class CFileConfiguration
{
public:
    /**
     * @brief SetConfFile       设置配置文件
     * @param vConfPath         [in]配置文件所在目录，eg: D:/test/config/
     * @param vConfName         [in]配置文件名，eg: test.ini
     */
    void        SetConfFile(const QString& vConfPath, const QString& vConfName);

    /**
     * @brief GetConfFilePath   获取配置文件所在目录
     * @return
     */
    QString     GetConfFilePath(void);

    /**
     * @brief GetConfFileName   获取配置文件名
     * @return
     */
    QString     GetConfFileName(void);

    /**
     * @brief ClearConfCache    清空配置缓存
     */
    void        ClearConfCache(void);

    void        SetConfValue(const QString& vConfKey, const QVariant& vConfValue);
    QVariant    GetConfValue(const QString& vConfKey, const QVariant& vDefaultValue = QVariant());

    virtual bool ConfRefresh(void);
    virtual bool ConfSave(void);

protected:
    CFileConfiguration(void);
    virtual ~CFileConfiguration(void);

    bool CheckConfFileInfo(void);
    bool CheckConfFileExist(void);

protected:
    QString m_ConfigFilePath;
    QString m_ConfigFileName;
    QList<QPair<QString, QVariant>> m_ConfValuePairList; //key, value
};

class CIniFileConfiguration : public CFileConfiguration
{
public:
    virtual bool ConfRefresh(void);
    virtual bool ConfSave(void);
};

#endif // FILECONFIGURATION_H
