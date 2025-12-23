#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollbar>
#include <QTextEdit>

#include "messagebubble.h"
#include "appconfig.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AppConfig::instance().setSettingsPath("ProbLanSuite", "Server");

    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listWidget->verticalScrollBar()->setSingleStep(10); // Weicheres Scrollen

    connect(&chatServer, &ChatServer::messageReceived, this, [=](const QString &msg) {
        addChatMessage("SlamWalka", msg, false);
    });
    connect(&chatServer, &ChatServer::newClientConnected, this, &MainWindow::updateClientList);
    connect(&chatServer, &ChatServer::clientDisconnected, this, &MainWindow::removeDisconnectedClient);

    chatServer.startServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addChatMessage(QString sender, QString text, bool isMe)
{
    QString avatarPath = isMe ? ":/images/me.png" : ":/images/other.png";
    MessageBubble *bubbleWidget = new MessageBubble(sender, text, avatarPath, isMe);
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

    item->setSizeHint(bubbleWidget->sizeHint());

    ui->listWidget->setItemWidget(item, bubbleWidget);

    ui->listWidget->scrollToBottom();
}

void MainWindow::updateClientList(ClientData data)
{
    QTreeWidgetItem *item = findCurrentEntry(data);
    QTreeWidgetItem *childIP = nullptr;
    QTreeWidgetItem *childSO = nullptr;

    if(!item)
    {
        item = new QTreeWidgetItem(ui->treeWidget);
        childIP = new QTreeWidgetItem(item);
        childSO = new QTreeWidgetItem(item);

    } else {
        if(item->childCount() == 2)
        {
            childIP = item->child(0);
            childSO = item->child(1);
        }
    }

    item->setText(0, data.nickName);

    if(childIP) childIP->setText(0, data.ip.remove("::ffff:"));
    if(childSO) childSO->setText(0, QString::number(data.socketDesc));
}

void MainWindow::removeDisconnectedClient(ClientData data)
{
    QTreeWidgetItem *current = findCurrentEntry(data);

    if(!current)
    {
        return;
    }

    qDebug() << "Entferne Client mit IP: " << data.ip << " und Socket: " << data.socketDesc;

    if(ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(current)))
    {
        delete current;
        qDebug() << "Client aus GUI entfernt.";
    }
}


QTreeWidgetItem *MainWindow::findCurrentEntry(ClientData data)
{
    for(int i = ui->treeWidget->topLevelItemCount() - 1; i >= 0; i--)
    {
        QTreeWidgetItem *currentEntry = ui->treeWidget->topLevelItem(i);

        for(int c = 0; c < currentEntry->childCount(); c++)
        {
            QTreeWidgetItem *child = currentEntry->child(c);

            if(child->text(0) == QString::number(data.socketDesc))
            {
                return currentEntry;
            }
        }
    }

    return nullptr;
}
