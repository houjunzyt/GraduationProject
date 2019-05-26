#ifndef LOGINNETSETWINDOW_H
#define LOGINNETSETWINDOW_H

#include <QMainWindow>
#include "ui_menuwindow.h"
#include "basewindow.h"

class Menuwindow : public BaseWindow
{
	Q_OBJECT

public:
    Menuwindow(QWidget *parent = nullptr );
    ~Menuwindow();

signals:
	void rotateWindow();
	void closeWindow();
	void hideWindow();

private:
	void initMyTitle();
	void initWindow();
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void closeEvent(QCloseEvent *event);
private slots:
    void onButtonCloseClicked();
    void on_TempButton_clicked();
    void on_SwitchButton_clicked();
    void on_CrtlButton_clicked();
    void on_WeatherButton_clicked();
private:
    Ui::Menuwindow ui;
	bool m_isPressed;
	QPoint m_startMovePos;
//protected:
  //  MyTitleBar* m_titleBar;


};

#endif // LOGINNETSETWINDOW_H
