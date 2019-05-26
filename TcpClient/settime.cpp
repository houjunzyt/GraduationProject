#include "settime.h"
#include "ui_settime.h"
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
#include <QTime>
#include <QTcpSocket>

extern QTcpSocket *tcpClient;

Settime::Settime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settime)
{
    ui->setupUi(this);
    mtimer = new QTimer();
    //当定时器启动后会在设定的时间到发生timeout信号
    connect(mtimer,SIGNAL(timeout()),this, SLOT(update_time()));
    ButtonFlag=0;//初始化标志位
    ui->label->setStyleSheet("background:transparent");//提示label为透明
    ui->TimeEdit->setStyleSheet("background:white;\
                                padding-left:5px ;\
                                padding-top:1px ;\
                                border-top-left-radius:3px; \
                                border-top-right-radius:3px;\
                                border-bottom-left-radius:3px; \
                                border-bottom-right-radius:3px;\
                                border: 1px solid rgb(209 , 209 , 209);\
                                padding-top:0px ;\
                                border: 1px solid rgb(21 , 131 , 221);");
    QString BtnStyle ="\
                                QPushButton\
                                {\
                                    color:white;\
                                    background-color:rgb(14 , 150 , 254);\
                                    border-radius:5px;\
                                }\
                                QPushButton:hover\
                                {\
                                    color:white;\
                                    background-color:rgb(44 , 137 , 255);\
                                }";//按键的风格
    ui->SetOk->setStyleSheet(BtnStyle+"\
                                 QPushButton:pressed\
                                 {\
                                     color:red;\
                                     background-color:rgb(14 , 135 , 228);\
                                     padding-left:3px;\
                                     padding-top:3px;\
                                 }");
    ui->LocalTime->setStyleSheet(BtnStyle+"\
                                 QPushButton:pressed\
                                 {\
                                     color:blue;\
                                     background-color:rgb(14 , 135 , 228);\
                                     padding-left:3px;\
                                     padding-top:3px;\
                                 }");
}

Settime::~Settime()
{
    delete ui;
}
void Settime::on_SetOk_clicked()
{
   if(tcpClient==nullptr)//如果套接字为空
   {
       ui->LocalTime->setText("Local Time");
       ButtonFlag=0;
       mtimer->stop();
       ui->TimeEdit->setPlaceholderText(QStringLiteral("sync failure"));
       return;
   }
   if(ui->LocalTime->text()=="Custom Time")//获取本地时间发送
   {
       QDateTime current_date_time =QDateTime::currentDateTime();
       QString current_date ="SetTime:";
       current_date += current_date_time.toString("yyyy-MM-dd hh:mm:ss");
       current_date += "end";
       if(current_date != "")
       {
           tcpClient->write(current_date.toLatin1()); //qt5出去了.toAscii()
       }
   }
   else
   {
       if(ui->LocalTime->text()=="Local Time")//发送用户设定的时间
       {
           if(ui->TimeEdit->text()=="")//输入框为空
           {
                ui->TimeEdit->setPlaceholderText(QStringLiteral("Pleas enter time !"));
           }
           else//输入框非空
           {
                tcpClient->write(ui->TimeEdit->text().toLatin1()); //qt5出去了.toAscii()
           }
       }
   }
}

void Settime::on_LocalTime_clicked()
{
    if(ui->LocalTime->text()=="Local Time")
    {
        //启动定时器
        mtimer->start(1000);//设定定时时间为1000毫秒
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
        ui->TimeEdit->setPlaceholderText(current_date);
        ui->LocalTime->setText("Custom Time");
        ButtonFlag=1;
    }
    else
    {
        //关闭定时器
        mtimer->stop();
        ui->LocalTime->setText("Local Time");
        ui->TimeEdit->setPlaceholderText(QStringLiteral(""));
        ButtonFlag=0;
    }
}

void Settime::update_time()
{
    //获取系统时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    if(ButtonFlag)
    {
       ui->TimeEdit->setPlaceholderText(current_date);//显示时间
    }
    else
    {
       ui->TimeEdit->setPlaceholderText(QStringLiteral(""));//关闭显示
    }
}
