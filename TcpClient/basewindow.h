#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QDialog>
#include "mytitlebar.h"

class BaseWindow : public QDialog
{
	Q_OBJECT

public:
    BaseWindow(QWidget *parent = nullptr );
	~BaseWindow();
protected:
	void initTitleBar();
	void paintEvent(QPaintEvent *event);
	void loadStyleSheet(const QString &sheetName);
	void createMyTitle(QWidget* parentWidget);

private slots:
	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
    MyTitleBar* m_titleBar;
	
};

#endif // BASEWINDOW_H
