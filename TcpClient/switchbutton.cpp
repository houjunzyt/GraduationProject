#include "switchbutton.h"
#include "ui_switchbutton.h"
#include <QTcpSocket>
#include "loginwindow.h"
#include "menuwindow.h"

extern LoginWindow* m_loginWindow;//外部引用声明
extern Menuwindow* m_loginNetSetWindow;
extern QTcpSocket *tcpClient;
extern QByteArray buffer;//接收缓冲区

Switch::Switch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Switch)
{
    ui->setupUi(this);
    Flag_Strip=0;//初始化私有数据
    Flag_Wash=0;
    Flag_water=0;
    Flag_Air=0;
    GetStatus=0;
    this->setStyleSheet("background-color: white");
    QString BtnStyle =" QPushButton\
                        {\
                            color:white;\
                            background-color:rgb(14 , 150 , 254);\
                            border-radius:5px;\
                        }\
                        QPushButton:hover\
                        {\
                            color:white;\
                            background-color:rgb(44 , 137 , 255);\
                        }\
                        QPushButton:pressed\
                        {\
                            color:white;\
                            background-color:rgb(14 , 135 , 228);\
                            padding-left:3px;\
                            padding-top:3px;\
                        }";
    QString BtnStyle1 =" QPushButton\
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
    ui->SyncButton->setStyleSheet(BtnStyle);
    ui->AirButton->setStyleSheet(BtnStyle1);
    ui->WaterButton->setStyleSheet(BtnStyle1);
    ui->WashButton->setStyleSheet(BtnStyle1);
    ui->StripButton->setStyleSheet(BtnStyle1);

    ui->AirButton->setBackgroundRole(QPalette::Base);
    ui->WaterButton->setBackgroundRole(QPalette::Base);
    ui->WashButton->setBackgroundRole(QPalette::Base);
    ui->StripButton->setBackgroundRole(QPalette::Base);

    QPixmap mypixmap;
    mypixmap.load(":/Button/Resources/Button/off.PNG");

    ui->AirButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->AirButton->setIcon(mypixmap);
    ui->AirButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

    ui->WaterButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->WaterButton->setIcon(mypixmap);
    ui->WaterButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

    ui->WashButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->WashButton->setIcon(mypixmap);
    ui->WashButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));


    ui->StripButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->StripButton->setIcon(mypixmap);
    ui->StripButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

    connect(m_loginWindow, SIGNAL(switchGetData()), this, SLOT(onswitchGetData()));//接收到数据

}

Switch::~Switch()
{
    delete ui;
}

void Switch::on_StripButton_clicked()
{
    QPixmap mypixmap;
    QString data="SetS:";
    if(Flag_Strip==0)//开按钮
    {
        Flag_Strip=1;
        mypixmap.load(":/Button/Resources/Button/on.PNG");//按下背景
        if(GetStatus>0)
        {
            GetStatus--;
        }
        else//发送
        {
            data+="41";
            tcpClient->write(data.toLatin1());//发送数据
        }
    }
    else//关按钮
    {
        Flag_Strip=0;
        mypixmap.load(":/Button/Resources/Button/off.PNG");
        if(GetStatus>0)
        {
            GetStatus--;
        }
        else//发送
        {
            data+="40";
            tcpClient->write(data.toLatin1());
        }
    }
    ui->StripButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->StripButton->setIcon(mypixmap);
    ui->StripButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));
}

void Switch::on_WashButton_clicked()
{
    QPixmap mypixmap;
    QString data="SetS:";
    if(Flag_Wash==0)//开按钮
    {
        Flag_Wash=1;
        mypixmap.load(":/Button/Resources/Button/on.PNG");
        if(GetStatus>0)
        {
            GetStatus--;
        }
        else//发送
        {
            data+="31";
            tcpClient->write(data.toLatin1());
        }
    }
    else//关按钮
    {
        Flag_Wash=0;
        mypixmap.load(":/Button/Resources/Button/off.PNG");
        if(GetStatus>0)
        {
            GetStatus--;
        }
        else//发送
        {
            data+="30";
            tcpClient->write(data.toLatin1());
        }
    }
    ui->WashButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->WashButton->setIcon(mypixmap);
    ui->WashButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));

}

void Switch::on_WaterButton_clicked()
{
    QPixmap mypixmap;
    QString data="SetS:";
    if(Flag_water==0)//开按钮
    {
        Flag_water=1;
        mypixmap.load(":/Button/Resources/Button/on.PNG");
        if(GetStatus>0)
        {
            GetStatus--;
        }
        else//发送
        {
            data+="21";
            tcpClient->write(data.toLatin1());
        }
    }
    else//关按钮
    {
        Flag_water=0;
        mypixmap.load(":/Button/Resources/Button/off.PNG");
        if(GetStatus>0)
        {
            GetStatus--;
        }
        else//发送
        {
            data+="20";
            tcpClient->write(data.toLatin1());
        }
    }
    ui->WaterButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->WaterButton->setIcon(mypixmap);
    ui->WaterButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));
}

void Switch::on_AirButton_clicked()
{
    QPixmap mypixmap;
    QString data="SetS:";
    if(Flag_Air==0)//开按钮
    {
        Flag_Air=1;
        mypixmap.load(":/Button/Resources/Button/on.PNG");
        if(GetStatus>0)
        {
            GetStatus--;
        }
        else//发送
        {
            data+="11";
            tcpClient->write(data.toLatin1());
        }

    }
    else//关按钮
    {
        Flag_Air=0;
        mypixmap.load(":/Button/Resources/Button/off.PNG");
        if(GetStatus>0)
        {
            GetStatus--;
        }
        else//发送
        {
            data+="10";
            tcpClient->write(data.toLatin1());
        }
    }
    ui->AirButton->setFixedSize( mypixmap.width(), mypixmap.height() );
    ui->AirButton->setIcon(mypixmap);
    ui->AirButton->setIconSize(QSize(mypixmap.width(),mypixmap.height()));
}

void Switch::on_SyncButton_clicked()
{
    QString data="GetSwitch:";
    tcpClient->write(data.toLatin1());
}

void Switch::onswitchGetData()
{
    int GetIndex=-1;
    qDebug()<<"get data"<<buffer;
    GetIndex=buffer.indexOf("SSW:");
    if(GetIndex<0)//不存在命令
        return;
    if(buffer[GetIndex+4]=='0')
    {
        Flag_Air=1;
    }
    else
    {
        Flag_Air=0;
    }

    if(buffer[GetIndex+5]=='0')
    {
        Flag_water=1;
    }
    else
    {
        Flag_water=0;
    }

    if(buffer[GetIndex+6]=='0')
    {
        Flag_Wash=1;
    }
    else
    {
        Flag_Wash=0;
    }

    if(buffer[GetIndex+7]=='0')
    {
        Flag_Strip=1;
    }
    else
    {
        Flag_Strip=0;
    }
    GetStatus=4;
    on_StripButton_clicked();
    on_WashButton_clicked();
    on_WaterButton_clicked();
    on_AirButton_clicked();
    buffer="";//清除接收缓冲区
}
