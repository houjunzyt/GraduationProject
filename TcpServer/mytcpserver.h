#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
namespace Ui {
class MyTcpServer;
}

class MyTcpServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyTcpServer(QWidget *parent = 0);
    ~MyTcpServer();

private:
    Ui::MyTcpServer *ui;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> tcpClient;
    QList<QString>tcpClientID;
    QList<QHostAddress> list ;
    QTcpSocket *currentClient;

private slots:
    void NewConnectionSlot();
    void disconnectedSlot();
    void ReadData();

    void on_btnConnect_clicked();
    void on_btnSend_clicked();
    void on_btnClear_clicked();
};

#endif // MYTCPSERVER_H
