#ifndef SETTIME_H
#define SETTIME_H

#include <QDialog>
#include <QTimer>//定时器头文件

namespace Ui {
class Settime;
}

class Settime : public QDialog
{
    Q_OBJECT

public:
    explicit Settime(QWidget *parent = nullptr);
    ~Settime();

private:
    Ui::Settime *ui;
    QTimer *mtimer;//定义一个定时器指针
    uint8_t ButtonFlag;

private slots:
    void on_SetOk_clicked();
    void on_LocalTime_clicked();

    void update_time();

};



#endif // SETTIME_H
