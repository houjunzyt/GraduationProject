#ifndef LoginWindow_H
#define LoginWindow_H

#include "basewindow.h"
#include <QListWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QTcpSocket>
#include <QObject>
#include <QMouseEvent>
#include <QHostAddress>
#include <QMessageBox>
#include <QNetworkInterface>

namespace Ui {
	class LoginWindow;
}

class LoginWindow : public BaseWindow
{
	Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

signals:
	void rotateWindow();
	void closeWindow();
	void hideWindow();

    void switchGetData();//给switch窗口
    void TempGetData();
private:
	// 初始化标题栏;
	void initMyTitle();
	// 初始化窗口;
	void initWindow();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

	
private slots:
    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

    void on_loginButton_clicked();

private:
	Ui::LoginWindow *ui;
    QPoint last;//鼠标事件

	// 翻转;
	QGraphicsScene m_scene;
	QGraphicsWidget *m_graphicsWidget;
	QGraphicsView m_view;

	bool m_isPressed;
	QPoint m_startMovePos;
};

#endif // LoginWindow_H
