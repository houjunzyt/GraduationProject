#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>

class AccountItem : public QWidget
{
	Q_OBJECT

public:
	AccountItem(QWidget *parent = NULL);

	// 设置登录用户信息;
	void setAccountInfo(int index, QString accountName, QString headFilePath);
	// 获取登录用户名称;
	QString getAccountName();
	// 获取当前itemWidget的index;
	int getItemWidgetIndex();

public slots:
	// 删除当前用户item;
	void onRemoveAccount();

private:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

Q_SIGNALS:
	// 通知登录界面显示当前用户信息;
	void signalShowAccountInfo(int index, QString accountName);
	// 通知登录界面删除当前用户信息;
	void signalRemoveAccount(int index);

private:
	bool m_mousePress;
	QLabel *m_accountNumber;
	QToolButton *m_deleteButton;
	QLabel *m_Icon;
	int m_index;
};

#endif // ACCOUNTITEM_H