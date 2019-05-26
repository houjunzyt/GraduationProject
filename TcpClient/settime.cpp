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
    //����ʱ������������趨��ʱ�䵽����timeout�ź�
    connect(mtimer,SIGNAL(timeout()),this, SLOT(update_time()));
    ButtonFlag=0;//��ʼ����־λ
    ui->label->setStyleSheet("background:transparent");//��ʾlabelΪ͸��
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
                                }";//�����ķ��
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
   if(tcpClient==nullptr)//����׽���Ϊ��
   {
       ui->LocalTime->setText("Local Time");
       ButtonFlag=0;
       mtimer->stop();
       ui->TimeEdit->setPlaceholderText(QStringLiteral("sync failure"));
       return;
   }
   if(ui->LocalTime->text()=="Custom Time")//��ȡ����ʱ�䷢��
   {
       QDateTime current_date_time =QDateTime::currentDateTime();
       QString current_date ="SetTime:";
       current_date += current_date_time.toString("yyyy-MM-dd hh:mm:ss");
       current_date += "end";
       if(current_date != "")
       {
           tcpClient->write(current_date.toLatin1()); //qt5��ȥ��.toAscii()
       }
   }
   else
   {
       if(ui->LocalTime->text()=="Local Time")//�����û��趨��ʱ��
       {
           if(ui->TimeEdit->text()=="")//�����Ϊ��
           {
                ui->TimeEdit->setPlaceholderText(QStringLiteral("Pleas enter time !"));
           }
           else//�����ǿ�
           {
                tcpClient->write(ui->TimeEdit->text().toLatin1()); //qt5��ȥ��.toAscii()
           }
       }
   }
}

void Settime::on_LocalTime_clicked()
{
    if(ui->LocalTime->text()=="Local Time")
    {
        //������ʱ��
        mtimer->start(1000);//�趨��ʱʱ��Ϊ1000����
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
        ui->TimeEdit->setPlaceholderText(current_date);
        ui->LocalTime->setText("Custom Time");
        ButtonFlag=1;
    }
    else
    {
        //�رն�ʱ��
        mtimer->stop();
        ui->LocalTime->setText("Local Time");
        ui->TimeEdit->setPlaceholderText(QStringLiteral(""));
        ButtonFlag=0;
    }
}

void Settime::update_time()
{
    //��ȡϵͳʱ��
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    if(ButtonFlag)
    {
       ui->TimeEdit->setPlaceholderText(current_date);//��ʾʱ��
    }
    else
    {
       ui->TimeEdit->setPlaceholderText(QStringLiteral(""));//�ر���ʾ
    }
}
