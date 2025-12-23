#include "appconfig.h"

AppConfig& AppConfig::instance()
{
    static AppConfig _instance;
    return _instance;
}

AppConfig::AppConfig()
    : settings("ProbLanSuite", "Server")
{

}

int AppConfig::getServerPort() const
{
    return settings.value("tcpServerPort", 45454).toInt();
}

int AppConfig::getDiscoverPort() const
{
    return settings.value("udpServerPort", 45455).toInt();
}

QString AppConfig::getUsername() const
{
    return settings.value("username", "UNKNOWN").toString();
}

// --- Setter (Speichern in Datei) ---

void AppConfig::setServerPort(int port)
{
    settings.setValue("server_port", port);
    // QSettings speichert automatisch beim Beenden oder periodisch.
    // Man kann sync() erzwingen, ist aber meist nicht nötig.
}

void AppConfig::setDiscoverPort(int port)
{
    settings.setValue("discover_port", port);
}

void AppConfig::setUsername(const QString &name)
{
    settings.setValue("username", name);
}

void AppConfig::iconPath(const QString &path)
{
    settings.setValue("icon", path);
}

void AppConfig::setSettingsPath(QString Vendor, QString Software)
{
    m_vendor = Vendor;
    m_software = Software;
}
