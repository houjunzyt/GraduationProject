#include "weather.h"
#include "ui_weather.h"
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

Weather::Weather(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Weather)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
    ui->City->setStyleSheet("background:transparent");//提示label为透明
    ui->Temperature->setStyleSheet("background:transparent");
    ui->WindPower->setStyleSheet("background:transparent");
    ui->WeatherStatus->setStyleSheet("background:transparent");
    ui->CityEdit->setStyleSheet("background:white;\
                                padding-left:5px ;\
                                padding-top:1px ;\
                                border-top-left-radius:3px; \
                                border-top-right-radius:3px;\
                                border-bottom-left-radius:3px; \
                                border-bottom-right-radius:3px;\
                                border: 1px solid rgb(209 , 209 , 209);\
                                padding-top:0px ;\
                                border: 1px solid rgb(21 , 131 , 221);");
    ui->TempEdit->setStyleSheet("background:white;\
                                padding-left:5px ;\
                                padding-top:1px ;\
                                border-top-left-radius:3px; \
                                border-top-right-radius:3px;\
                                border-bottom-left-radius:3px; \
                                border-bottom-right-radius:3px;\
                                border: 1px solid rgb(209 , 209 , 209);\
                                padding-top:0px ;\
                                border: 1px solid rgb(21 , 131 , 221);");
    ui->WeatherEdit->setStyleSheet("background:white;\
                                padding-left:5px ;\
                                padding-top:1px ;\
                                border-top-left-radius:3px; \
                                border-top-right-radius:3px;\
                                border-bottom-left-radius:3px; \
                                border-bottom-right-radius:3px;\
                                border: 1px solid rgb(209 , 209 , 209);\
                                padding-top:0px ;\
                                border: 1px solid rgb(21 , 131 , 221);");
    ui->WindEdit->setStyleSheet("background:white;\
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
    ui->GetWeather->setStyleSheet(BtnStyle+"\
                                 QPushButton:pressed\
                                 {\
                                     color:red;\
                                     background-color:rgb(14 , 135 , 228);\
                                     padding-left:3px;\
                                     padding-top:3px;\
                                 }");
    ui->SyncWeather->setStyleSheet(BtnStyle+"\
                                  QPushButton:pressed\
                                  {\
                                      color:red;\
                                      background-color:rgb(14 , 135 , 228);\
                                      padding-left:3px;\
                                      padding-top:3px;\
                                  }");
    }

Weather::~Weather()
{
    delete ui;
}

void Weather::on_GetWeather_clicked()
{
    /*设置发送数据*/

    QString local_city = ui->CityEdit->text().trimmed();
    char quest_array[256]="http://wthrcdn.etouch.cn/weather_mini?city=";
    QNetworkRequest quest;
    sprintf(quest_array,"%s%s",quest_array,local_city.toUtf8().data());
    quest.setUrl(QUrl(quest_array));
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    manager->get(quest);
}

void Weather::on_SyncWeather_clicked()
{
    QString weatherData="SetWeather:";
    qDebug()<<"Sync!!";
    if(ui->TempEdit->text()!=nullptr)
    {
        if(ui->WeatherEdit->text()=="多云")
        {
            weatherData+="A1 ";
        }
        else
        if(ui->WeatherEdit->text()=="阴")
        {
            weatherData+="A2 ";
        }
        else
        if(ui->WeatherEdit->text()=="阵雨")
        {
            weatherData+="B1 ";
        }
        else
        if(ui->WeatherEdit->text()=="小雨")
        {
            weatherData+="B6 ";
        }
        else
        if(ui->WeatherEdit->text()=="中雨")
        {
            weatherData+="B4 ";
        }
        else
        if(ui->WeatherEdit->text()=="大雨")
        {
            weatherData+="B2 ";
        }
        else
        if(ui->WeatherEdit->text()=="暴雨")
        {
            weatherData+="B3 ";
        }
        else
        if(ui->WeatherEdit->text()=="雷阵雨")
        {
            weatherData+="B5 ";
        }
        else
        if(ui->WeatherEdit->text()=="晴")
        {
            weatherData+="C1 ";
        }
        QString s1=ui->TempEdit->text().mid(0,2);
        QString s2=ui->TempEdit->text().mid(4,2);
        weatherData+=s1;
        weatherData+=s2;
        weatherData+="end";
        tcpClient->write(weatherData.toLatin1()); //qt5出去了.toAscii()
    }
}

void Weather::replyFinished(QNetworkReply *reply)
{
    int GetIndex=-1;
    qDebug()<<"finish!!";
    //QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString all = reply->readAll();//codec->toUnicode().toLocal8Bit();
    GetIndex=all.indexOf("invilad-citykey");
    if(GetIndex>=0)
    {
        ui->CityEdit->setText("invilad citykey!");
        ui->TempEdit->setText(" ");
        ui->WindEdit->setText(" ");
        ui->WeatherEdit->setText(" ");
        return ;
    }
    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);

        qDebug() << err.error;

    if(!json_recv.isNull())
    {
        QJsonObject object = json_recv.object();

        if(object.contains("data"))
        {
            QJsonValue value = object.value("data");  // 获取指定 key 对应的 value
            if(value.isObject())
            {
                QJsonObject object_data = value.toObject();
                if(object_data.contains("forecast"))
                {
                    QJsonValue value = object_data.value("forecast");
                    if(value.isArray())
                    {
                        QJsonObject today_weather = value.toArray().at(0).toObject();
                        WeatherStatus = today_weather.value("type").toString();

                        QString low = today_weather.value("low").toString();
                        QString high = today_weather.value("high").toString();
                        Temperature = low.mid(low.length()-3,4) +"~"+ high.mid(high.length()-3,4);
                        QString strength = today_weather.value("fengli").toString();
                        strength.remove(0,8);
                        strength.remove(strength.length()-2,2);
                        WindPower = today_weather.value("fengxiang").toString() + strength;
                        ui->WeatherEdit->setText(WeatherStatus);
                        ui->TempEdit->setText(Temperature);
                        ui->WindEdit->setText(WindPower);
                    }
                }
            }
        }

    }else
    {
        qDebug()<<"json_recv is NULL or is not a object !!";
    }
    reply->deleteLater();
}
