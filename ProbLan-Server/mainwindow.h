#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chatserver.h>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addChatMessage(QString sender, QString text, bool isMe);
    void updateClientList(ClientData data);
    void removeDisconnectedClient(ClientData data);

private:
    Ui::MainWindow *ui;
    ChatServer chatServer;

private:
    QTreeWidgetItem* findCurrentEntry(ClientData data);
};
#endif // MAINWINDOW_H
