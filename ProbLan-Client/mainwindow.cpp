#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "appconfig.h"
#include "messagebubble.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setDisabled(true);

    AppConfig::instance().setSettingsPath("ProbLanSuite", "Client");

    QString lastUserName = AppConfig::instance().getUsername();
    if(lastUserName.startsWith("UNKNOWN") || lastUserName.isEmpty())
    {
        char username[256];
        DWORD username_len = sizeof(username);

        GetUserNameA(username, &username_len);
        lastUserName =  username;
    }

    ui->lineEdit_2->setText(lastUserName);


    connect(ui->lineEdit, &QLineEdit::returnPressed, [&](){
        addChatMessage(ui->lineEdit_2->text(), ui->lineEdit->text(), true);
        netWork.sendMessage(ui->lineEdit->text(), message, ui->tabWidget->currentWidget()->objectName());
        ui->lineEdit->clear();
    });

    connect(ui->pushButton, &QPushButton::pressed, [&](){
        addChatMessage(ui->lineEdit_2->text(), ui->lineEdit->text(), true);
        netWork.sendMessage(ui->lineEdit->text(), message, ui->tabWidget->currentWidget()->objectName());
        ui->lineEdit->clear();
    });

    connect(&netWork, &QTcpSocket::disconnected, [&](){
        ui->label_3->setText("🔴 Getrennt |");
        ui->centralwidget->setDisabled(true);
        ui->label_2->clear();
    });

    connect(&netWork, &QTcpSocket::connected, [&](){
        ui->label_3->setText("🟢 Verbunden |");
        ui->centralwidget->setDisabled(false);
        ui->label_2->setText(netWork.localAddress().toString().remove("::ffff:"));
        netWork.sendMessage("USERNAME:" + ui->lineEdit_2->text(), config, "");
    });

    connect(ui->lineEdit_2, &QLineEdit::textChanged, [&](){
        netWork.sendMessage("USERNAME:" + ui->lineEdit_2->text(), config, "");
        AppConfig::instance().setUsername(ui->lineEdit_2->text());
    });

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addChatMessage(QString sender, QString text, bool isMe)
{
    // Dummy Pfad für das Beispiel. Später nimmst du das aus deiner User-Datenbank
    QString avatarPath = isMe ? "/me.jpg" : ":/images/other.png";

    // 1. Das Custom Widget erstellen
    MessageBubble *bubbleWidget = new MessageBubble(sender, text, avatarPath, isMe);

    // 2. Ein Item für die Liste erstellen
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

    // 3. Größe berechnen lassen!
    // Das ist der Trick: Das Widget sagt dem Item, wie groß es sein muss.
    item->setSizeHint(bubbleWidget->sizeHint());

    // 4. Widget und Item verheiraten
    ui->listWidget->setItemWidget(item, bubbleWidget);

    // 5. Automatisch nach unten scrollen
    ui->listWidget->scrollToBottom();
}
