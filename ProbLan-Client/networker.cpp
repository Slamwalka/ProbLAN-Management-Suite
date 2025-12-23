#include "networker.h"
#include "appconfig.h"

NetWorker::NetWorker(QObject *parent) :
    QTcpSocket(parent),
    udpSocket(new QUdpSocket(parent))
{

    connect(this, &QTcpSocket::readyRead, this, &NetWorker::readIncomingData);

    connect(this, &QTcpSocket::disconnected, [=]()
            {
                qDebug() << "Beginne suche nach Server";
                udpSocket->bind(AppConfig::instance().getDiscoverPort(),QUdpSocket::ShareAddress);
            });

    connect(udpSocket, &QUdpSocket::readyRead, this, &NetWorker::connectToServer);

    emit this->disconnected();
}

bool NetWorker::connectToServer()
{
    if(this->state() == QAbstractSocket::ConnectingState || this->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "Socket ist bereits Verbunden oder Verbindet sich bereits";
        return false;
    }

    QHostAddress senderIP;
    if(udpSocket->hasPendingDatagrams())
    {
        QByteArray incomingUdpData;
        incomingUdpData.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(incomingUdpData.data(), incomingUdpData.size(), &senderIP);

        if(incomingUdpData != "DISCOVERPROBLANSERVER")
        {
            qDebug() << "UDP Datagram erhalten - Falscher Key";
            return false;
        }

        udpSocket->close();
    }

    this->connectToHost(senderIP, AppConfig::instance().getServerPort());
    this->waitForConnected();

    if(this->state() == QAbstractSocket::ConnectedState || this->state() == QAbstractSocket::HostLookupState)
    {
        qDebug() << "Verbunden zu " << senderIP;
        return true;
    }

    qDebug() << "TCP Verbindung fehlgeschlagen";
    return false;
}

void NetWorker::readIncomingData()
{
    QByteArray data = this->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    QString dispMsg = QString("[%1] %2: %3").arg(obj["timestamp"].toString(),
                                                 obj["sender"].toString(),
                                                 obj["content"].toString()
                                                 );

    qDebug() << "INCOMING: " << dispMsg;
}

void NetWorker::sendMessage(const QString &msg, const MessageType &type, const QString &destination)
{
    QJsonObject MsgFormat;

    switch(type) {
    case message:
        MsgFormat["type"] = "message";
        break;
    case config:
        MsgFormat["type"] = "config";
        break;
    case picture:
        MsgFormat["type"] = "picture";
        break;
    }

    MsgFormat["sender"] = "";
    MsgFormat["target"] = destination;
    MsgFormat["content"] = msg;
    MsgFormat["timestamp"] = QTime::currentTime().toString("HH:mm:ss");

    this->write(QJsonDocument(MsgFormat).toJson(QJsonDocument::Compact));
}
