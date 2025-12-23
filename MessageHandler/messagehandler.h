#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "MessageHandler_global.h"

class MESSAGEHANDLER_EXPORT MessageHandler : public QObject
{
    Q_OBJECT

public:
    MessageHandler(QObject *parent = nullptr);
    bool handleMessage(const QByteArray &message);

signals:
    void userNameChanged(const QString &newUserName);
    void newMessage(const QString &sender, const QString &destination, const QString &msg);

private:
    void handleSettings(const QJsonObject &currentMessage);

private:

    enum ConfigType {
        username = 0,
        icon,
        picdata,
        rawdata
    };

    QMap<ConfigType, QString> m_configKeys
    {
        {username, "USERNAME:"},
        {icon, "ICON:"},
        {picdata, "PDATA:"},
        {rawdata, "RDATA:"}
    };


    enum MessageType {
        message = 0,
        config,
        picture,
        data
    };

    QMap<MessageType, QString> m_typeKeys
        {
            {message, "message"},
            {config, "config"},
            {picture, "picture"},
            {data, "data"}
        };

    enum MessageKeys {
        type = 0,
        sender,
        target,
        content,
        timestamp
    };

    QMap<MessageKeys, QString> m_messageKeys
        {
            {type, "type"},
            {sender, "sender"},
            {target, "target"},
            {content, "content"},
            {timestamp, "timestamp"}
        };
};

#endif // MESSAGEHANDLER_H
