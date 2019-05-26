#include "mytcpserver.h"
#include "ui_mytcpserver.h"

MyTcpServer::MyTcpServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyTcpServer)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/APPICON/image/logo.png"));//APP图标
    this->setWindowTitle("Hou Jun's Graduation Design");//软件的标题
    tcpServer = new QTcpServer(this);

    list = QNetworkInterface::allAddresses();
    for(int i=0;i<list.count();i++)
    {
        QHostAddress addr=list.at(i);
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
        {
          ui->editIP->addItem(addr.toString());
        }
    }
    ui->btnConnect->setEnabled(true);
    ui->btnSend->setEnabled(false);

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
}

MyTcpServer::~MyTcpServer()
{
    delete ui;
}
// newConnection -> newConnectionSlot 新连接建立的槽函数
void MyTcpServer::NewConnectionSlot()
{
    currentClient = tcpServer->nextPendingConnection();
    tcpClient.append(currentClient);//添加一个客户端容器
    tcpClientID.append("NULL");//增加一个容器和tcpClient一致

    ui->cbxConnection->addItem(tr("%1:%2").arg(currentClient->peerAddress().toString()\
                                          .arg(currentClient->peerPort())));
    connect(currentClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));

    QString connectMSG;
    connectMSG=tr("Client :[%1]").arg(currentClient->peerAddress().toString().split("::ffff:")[1]);//格式化显示
    connectMSG+=" Connected";
    ui->edtRecv->append(connectMSG);//显示连接
}

// 客户端数据可读信号，对应的读数据槽函数
void MyTcpServer::ReadData()
{
    int32_t GetIndex=-1;
    uint8_t times=0;//记录匹配次数
    // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
    for(int i=0; i<tcpClient.length(); i++)
    {
        QByteArray buffer = tcpClient[i]->readAll();
        if(buffer.isEmpty())    continue;//无数据
        GetIndex=buffer.indexOf("setID:");
        if(GetIndex>=0)//设置ID值
        {
            tcpClientID.replace(i,buffer.mid(GetIndex+6,4));//获取后面四位ID值,添加到对应的容器
            qDebug() << "add ID: " << tcpClientID.at(i);
        }

        //将数据转发
        if(tcpClientID.at(i)!="NULL"&&GetIndex<0)//当前的数据已经赋值配对密码
        {
            for(int j=0; j<tcpClient.length();j++)
            {
                if(tcpClientID.at(i)==tcpClientID.at(j))//密码配对成功，将数据转发
                {
                    if(i!=j)//不要转发给发过去的客户端
                        tcpClient[j]->write(buffer);
                    times++;
                }
            }
            if(times<=1)//就发了一次,没有找到的配对的伙伴，通知客户端
            {
                QString NoticStr="NoPartner";
                tcpClient[i]->write(NoticStr.toLatin1());//qt5除去了.toAscii()，转化为单字节编码
            }
            times=0;//清除标志
        }
        else//告诉发过来的客户端没有配对密码
        {
            QString ErrorStr="NoPassWord";
            tcpClient[i]->write(ErrorStr.toLatin1());//qt5除去了.toAscii()，转化为单字节编码
        }

        static QString IP_Port, IP_Port_Pre;
        IP_Port = tr("[%1:%2]:").arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
                                     .arg(tcpClient[i]->peerPort());//格式化显示

        // 若此次消息的地址与上次不同，则需显示此次消息的客户端地址
        if(IP_Port != IP_Port_Pre)
            ui->edtRecv->append(IP_Port);

        ui->edtRecv->append(buffer);

        //更新ip_port
        IP_Port_Pre = IP_Port;
    }
}
// disconnected -> disconnectedSlot 客户端断开连接的槽函数
void MyTcpServer::disconnectedSlot()
{
    //由于disconnected信号并未提供SocketDescriptor，所以需要遍历寻找
    for(int i=0; i<tcpClient.length(); i++)
    {
        if(tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {

            QString connectMSG;
            connectMSG=tr("Client :[%1]").arg(currentClient->peerAddress().toString().split("::ffff:")[1]);//格式化显示
            connectMSG+=" Disconnected";
            ui->edtRecv->append(connectMSG);//显示断开

            // 删除存储在combox中的客户端信息
            ui->cbxConnection->removeItem(ui->cbxConnection->findText(tr("%1:%2")\
                                  .arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
                                  .arg(tcpClient[i]->peerPort())));
            // 删除存储在tcpClient列表中的客户端信息
             tcpClient[i]->destroyed();
             tcpClient.removeAt(i);
            //也同时删除配对密码
             tcpClientID.removeAt(i);
        }
    }
}
// 监听--断开
void MyTcpServer::on_btnConnect_clicked()
{
    if(ui->btnConnect->text()=="监听")
    {
        bool ok = tcpServer->listen(QHostAddress::Any, ui->edtPort->text().toInt());
        if(ok)
        {
            ui->btnConnect->setText("断开");
            ui->btnSend->setEnabled(true);
        }
    }
    else
    {
        for(int i=0; i<tcpClient.length(); i++)//断开所有连接
        {
            tcpClient[i]->disconnectFromHost();
            bool ok = tcpClient[i]->waitForDisconnected(1000);
            if(!ok)
            {
                // 处理异常
            }
            tcpClient.removeAt(i);  //从保存的客户端列表中取去除
        }
        tcpServer->close();     //不再监听端口
        ui->btnConnect->setText("监听");
        ui->btnSend->setEnabled(false);
    }
}
// 发送数据
void MyTcpServer::on_btnSend_clicked()
{
    QString data = ui->edtSend->toPlainText();
    if(data == "")  return;    // 文本输入框为空时
    //全部连接
    if(ui->cbxConnection->currentIndex() == 0)
    {
        for(int i=0; i<tcpClient.length(); i++)
            tcpClient[i]->write(data.toLatin1()); //qt5除去了.toAscii()，转化为单字节编码
    }
    //指定连接
    else
    {
        QString clientIP = ui->cbxConnection->currentText().split(":")[0];
        int clientPort = ui->cbxConnection->currentText().split(":")[1].toInt();
//        qDebug() << clientIP;
//        qDebug() << clientPort;
        for(int i=0; i<tcpClient.length(); i++)
        {
            if(tcpClient[i]->peerAddress().toString().split("::ffff:")[1]==clientIP\
                            && tcpClient[i]->peerPort()==clientPort)
            {
                tcpClient[i]->write(data.toLatin1());
                return; //ip:port唯一，无需继续检索
            }
        }
    }
}

void MyTcpServer::on_btnClear_clicked()
{
    ui->edtRecv->clear();
}

