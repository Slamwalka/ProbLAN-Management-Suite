#ifndef NETWORKER_H
#define NETWORKER_H

#include <QTcpSocket>
#include <QUdpSocket>
#include <QJsonObject>
#include <QJsonDocument>

enum MessageType {
    message = 0,
    config,
    picture
};


class NetWorker : public QTcpSocket
{
    Q_OBJECT
public:
    explicit NetWorker(QObject *parent = nullptr);
    void sendMessage(const QString &msg, const MessageType &type, const QString &destination);

public slots:

private slots:
    bool connectToServer();
    void readIncomingData();

private:

private:
    QUdpSocket *udpSocket = nullptr;
};

#endif // NETWORKER_H
