#include "chatserver.h"
#include "appconfig.h"

ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{
}

void ChatServer::startServer()
{
    if(!this->listen(QHostAddress::Any, AppConfig::instance().getServerPort()))
    {
        qDebug() << "Kein Serverstart möglich!";
    }

    if(!discSocket)
    {
        discSocket = new QUdpSocket;
    }

    connect(&discTimer, &QTimer::timeout, this, [&]()
            {
                for(QNetworkInterface &currentInterface : systemInterfaces)
                {
                    QNetworkInterface::InterfaceFlags flags = currentInterface.flags();

                    if (!flags.testFlag(QNetworkInterface::IsUp) || flags.testFlag(QNetworkInterface::IsLoopBack))
                    {
                        continue;
                    }

                    QList<QNetworkAddressEntry> entries = currentInterface.addressEntries();

                    for(QNetworkAddressEntry &currentEntry : entries)
                    {
                        if(!currentEntry.broadcast().isNull())
                        {
                            discSocket->writeDatagram(QByteArray("DISCOVERPROBLANSERVER"), currentEntry.broadcast(), AppConfig::instance().getDiscoverPort());
                        }
                    }
                }
            });

    discTimer.setInterval(2500);
    discTimer.setTimerType(Qt::VeryCoarseTimer);
    discTimer.start();
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    socket->setProperty("Descriptor", socketDescriptor);
    socket->setProperty("SocketIP", socket->peerAddress().toString());

    connect(socket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatServer::onDisconnected);

    clients.append(socket);

    ClientData currentClient;
    currentClient.nickName = "UNKNOWN";
    currentClient.ip = socket->peerAddress().toString();
    currentClient.socketDesc = socket->socketDescriptor();

    emit newClientConnected(currentClient);
}

void ChatServer::onReadyRead()
{
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender());
    if(!senderSocket) return;

    QByteArray data = senderSocket->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    QMetaObject::Connection conU = connect(&msgHand, &MessageHandler::userNameChanged, [&senderSocket, this, &conU](const QString &newUserName)
                                           {
                                               emit newClientConnected({newUserName, senderSocket->peerAddress().toString(), static_cast<qint32>(senderSocket->socketDescriptor())});
                                               disconnect(conU);
                                           });

    QMetaObject::Connection conM = connect(&msgHand, &MessageHandler::userNameChanged, [ this, &conM](const QString &Message)
                                           {
                                               emit messageReceived(Message);
                                               disconnect(conM);
                                           });

    msgHand.handleMessage(data);

    if(obj["type"].toString() == "message" && obj["target"].toString() == "global")
    {
        broadcast(data);

        QString displayMsg = QString("%1").arg(
            obj["content"].toString()
            );

        emit messageReceived(displayMsg);
    }
}

void ChatServer::onDisconnected()
{
    QTcpSocket *currentSocket = qobject_cast<QTcpSocket*>(sender());

    if(!currentSocket) return;
    clients.removeOne(currentSocket);

    qDebug() << "Entferne Client " << currentSocket->property("Descriptor");

    ClientData currentClient;
    currentClient.ip = currentSocket->property("SocketIP").toString();
    currentClient.socketDesc = currentSocket->property("Descriptor").toInt();

    emit clientDisconnected(currentClient);
    currentSocket->deleteLater();
}

void ChatServer::broadcast(const QByteArray &data, QTcpSocket *exclude)
{
    for(QTcpSocket *client : std::as_const(clients))
    {
        if(client != exclude && client->state() == QAbstractSocket::ConnectedState)
        {
            client->write(data);
        }
    }
}
