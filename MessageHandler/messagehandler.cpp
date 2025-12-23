#include "messagehandler.h"
#include <QMetaEnum>

MessageHandler::MessageHandler(QObject *parent) : QObject(parent)
{

}

bool MessageHandler::handleMessage(const QByteArray &currentMessage)
{
    QJsonParseError *error = nullptr;
    QJsonDocument doc = QJsonDocument::fromJson(currentMessage, error);

    if(error && error->error != QJsonParseError::NoError)
    {
        qDebug() << "Parsing current Data Error :\r\n--> " << error->errorString() << "\r\n" << message;
        delete error;
        return false;
    }

    QJsonObject docObj = doc.object();

    if(docObj[m_messageKeys[type]] == m_typeKeys[config])
    {
        qDebug() << "Handling configMessage from " << docObj[m_messageKeys[sender]].toString();
        handleSettings(docObj);
    }

    if(docObj[m_messageKeys[type]] == m_typeKeys[message])
    {
        //Handle Message
    }

    if(docObj[m_messageKeys[type]] == m_typeKeys[picture])
    {
        //Handle pictureTransfer
    }

    return true;
}

void MessageHandler::handleSettings(const QJsonObject &currentMessage)
{
    if(currentMessage[m_messageKeys[content]].toString().startsWith(m_configKeys[username]))
    {
        emit userNameChanged(currentMessage[m_messageKeys[content]].toString().remove(m_configKeys[username]));
    }
}
