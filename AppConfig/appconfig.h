#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "AppConfig_global.h"

#include <QString>
#include <QSettings>

class APPCONFIG_EXPORT AppConfig
{
public:
    static AppConfig& instance();

    int getServerPort() const;
    int getDiscoverPort() const;
    QString getUsername() const;
    QString getLastIp() const;

    void setServerPort(int port);
    void setDiscoverPort(int port);
    void setUsername(const QString &name);
    void setLastIp(const QString &ip);
    void iconPath(const QString &path);

    void setSettingsPath(QString Vendor, QString Software);

private:
    AppConfig();

    AppConfig(const AppConfig&) = delete;
    void operator=(const AppConfig&) = delete;

    QSettings settings;
    QString m_vendor = "ProbLanSuite";
    QString m_software = "Server";
};

#endif // APPCONFIG_H
