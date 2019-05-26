#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <QDialog>

namespace Ui {
class Temperature;
}

class Temperature : public QDialog
{
    Q_OBJECT

public:
    explicit Temperature(QWidget *parent = nullptr );
    ~Temperature();

private:
    Ui::Temperature *ui;

private slots:
    void on_Temp_clicked();
    void on_PM25_clicked();
    void on_Hum_clicked();

    void onTempGetData();

};




#endif // TEMPERATURE_H
