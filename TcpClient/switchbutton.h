#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QDialog>

namespace Ui {
class Switch;
}

class Switch : public QDialog
{
    Q_OBJECT

public:
    explicit Switch(QWidget *parent = nullptr );
    ~Switch();


private:
    Ui::Switch *ui;

    uint8_t Flag_Strip;
    uint8_t Flag_Wash;
    uint8_t Flag_water;
    uint8_t Flag_Air;
    uint8_t GetStatus;//用于标志获取了一次

private slots:

    void on_StripButton_clicked();
    void on_WashButton_clicked();
    void on_WaterButton_clicked();
    void on_AirButton_clicked();
    void on_SyncButton_clicked();

    void onswitchGetData();

};

#endif // SWITCHBUTTON_H
