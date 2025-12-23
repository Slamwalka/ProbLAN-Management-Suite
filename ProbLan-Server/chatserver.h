#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QNetworkDatagram>
#include <QNetworkInterface>

#include "messagehandler.h"

struct ClientData {
    QString nickName;
    QString ip;
    qint32 socketDesc;
};

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    void startServer();

signals:
    void messageReceived(QString message);
    void newClientConnected(ClientData client);
    void clientDisconnected(ClientData client);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QList<QTcpSocket*> clients;
    void broadcast(const QByteArray &data, QTcpSocket *exclude = nullptr);
    QUdpSocket *discSocket = nullptr;
    QTimer discTimer;
    QList<QNetworkInterface> systemInterfaces = QNetworkInterface::allInterfaces();
    MessageHandler msgHand;
};

#endif // CHATSERVER_H
