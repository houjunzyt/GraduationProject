#include "temperature.h"
#include "ui_temperature.h"
#include <QPainter>
#include <QMovie>
#include <QHBoxLayout>
#include <QCursor>
#include <QMenu>
#include <QDebug>
#include <QLabel>
#include <QListWidget>
#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QTcpSocket>
#include "loginwindow.h"
#include "menuwindow.h"
#include <iostream>

using namespace std;

extern LoginWindow* m_loginWindow;//外部引用声明
extern Menuwindow* m_loginNetSetWindow;
extern QTcpSocket *tcpClient;
extern QByteArray buffer;//接收缓冲区

Temperature::Temperature(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Temperature)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white");

    QString BtnStyle =" QPushButton\
                        {\
                            color:white;\
                            border-radius:5px;\
                        }\
                        QPushButton:hover\
                        {\
                            color:white;\
                        }\
                        QPushButton:pressed\
                        {\
                            color:white;\
                            padding-left:3px;\
                            padding-top:3px;\
                        }";
    ui->Temp->setStyleSheet(BtnStyle);
    ui->Temp->setBackgroundRole(QPalette::Base);
    QPixmap mypixmap;
    mypixmap.load(":/Temp/Resources/Temp/temp.PNG");
    ui->Temp->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->Temp->setIcon(mypixmap);
    ui->Temp->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

    ui->PM25->setStyleSheet(BtnStyle);
    ui->PM25->setBackgroundRole(QPalette::Base);
    mypixmap.load(":/Temp/Resources/Temp/PM.PNG");
    ui->PM25->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->PM25->setIcon(mypixmap);
    ui->PM25->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

    ui->Hum->setStyleSheet(BtnStyle);
    ui->Hum->setBackgroundRole(QPalette::Base);
    mypixmap.load(":/Temp/Resources/Temp/hum.PNG");
    ui->Hum->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->Hum->setIcon(mypixmap);
    ui->Hum->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

    connect(m_loginWindow, SIGNAL(TempGetData()), this, SLOT(onTempGetData()));//接收到数据
}

Temperature::~Temperature()
{
    delete ui;
}

void Temperature::on_Temp_clicked()
{
    QString data="GetTemp:";
    tcpClient->write(data.toLatin1());
}

void Temperature::on_PM25_clicked()
{
    QString data="GetPM:";
    tcpClient->write(data.toLatin1());
}


void Temperature::on_Hum_clicked()
{
    QString data="GetHum:";
    tcpClient->write(data.toLatin1());
}

void Temperature::onTempGetData()
{
    int GetIndex=-1,nextIndex=-1;
    QString Massage;
    Massage=QString::fromLocal8Bit(buffer);

    GetIndex=buffer.indexOf("Temp:T1:");//判断命令
    if(GetIndex>=0)
    {
        QString data;
        double temp1;
        nextIndex=buffer.indexOf("T2:");//判断命令
        if(nextIndex<0)
            return;
        data=Massage.mid(GetIndex+8,nextIndex-8-GetIndex);
        temp1=data.toDouble();
        temp1/=10;
        data=QString::number(temp1);
        data+="℃";
        ui->T1->setText(data);//显示
        GetIndex=nextIndex;
        nextIndex=-1;
        data="";//清空缓冲区

        nextIndex=buffer.indexOf("T3:");//判断命令
        if(nextIndex<0)
            return;
        data=Massage.mid(GetIndex+3,nextIndex-3-GetIndex);
        temp1=data.toDouble();
        temp1/=10;
        data=QString::number(temp1);
        data+="℃";
        ui->T2->setText(data);//显示
        GetIndex=nextIndex;
        nextIndex=-1;
        data="";//清空缓冲区


        nextIndex=buffer.indexOf("T4:");//判断命令
        if(nextIndex<0)
            return;
        data=Massage.mid(GetIndex+3,nextIndex-3-GetIndex);
        temp1=data.toDouble();
        temp1/=10;
        data=QString::number(temp1);
        data+="℃";
        ui->T3->setText(data);//显示
        GetIndex=nextIndex;
        nextIndex=-1;
        data="";//清空缓冲区

        nextIndex=buffer.indexOf("endT");//判断命令
        if(nextIndex<0)
            return;
        data=Massage.mid(GetIndex+3,nextIndex-3-GetIndex);
        temp1=data.toDouble();
        temp1/=10;
        data=QString::number(temp1);
        data+="℃";
        ui->T4->setText(data);//显示
        GetIndex=nextIndex;
        nextIndex=-1;
        GetIndex=-1;
        data="";//清空缓冲区

    }

    GetIndex=buffer.indexOf("Temp:Hum:");//判断命令
    if(GetIndex>=0)
    {
        QString data;
        nextIndex=buffer.indexOf("endH");//判断命令
        if(nextIndex<0)
            return;
        data=Massage.mid(GetIndex+9,nextIndex-9-GetIndex);
        data+=" %RH";
        ui->HUM->setText(data);//显示
        nextIndex=-1;
        GetIndex=-1;
        data="";//清空缓冲区
    }
    GetIndex=buffer.indexOf("Temp:PM:");//判断命令
    if(GetIndex>=0)
    {
        QString data;
        nextIndex=buffer.indexOf("endP");//判断命令
        if(nextIndex<0)
            return;
        data=Massage.mid(GetIndex+8,nextIndex-8-GetIndex);
        ui->PM->setText(data);//显示
        nextIndex=-1;
        GetIndex=-1;
        data="";//清空缓冲区
    }
}
