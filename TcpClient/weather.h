#ifndef WEATHER_H
#define WEATHER_H

#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QTextCodec>

namespace Ui {
class Weather;
}

class Weather : public QDialog
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    ~Weather();

private:
    Ui::Weather *ui;
    QNetworkAccessManager *manager;
    QNetworkRequest *quest;
    QString WindPower;
    QString Temperature;
    QString WeatherStatus;

private slots:
    void replyFinished(QNetworkReply *reply);
    void on_GetWeather_clicked();
    void on_SyncWeather_clicked();
};


#endif
