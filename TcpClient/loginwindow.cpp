#include "LoginWindow.h"
#include "ui_loginwindow.h"
#include <QPainter>
#include <QMovie>
#include <QHBoxLayout>
#include <QCursor>
#include <QMenu>
#include <QDebug>
#include <QTime>
#include <QPropertyAnimation>
#include <QLabel>
#include <QListWidget>
#include <QDialog>
#include <QFile>
#include <QTextStream>
#include "switchbutton.h"

QTcpSocket *tcpClient=nullptr;//客户端套接字
extern Switch *switchwidget;
QByteArray buffer;//接收缓冲区

LoginWindow::LoginWindow(QWidget *parent)
    : BaseWindow(parent)
    , ui(new Ui::LoginWindow)
    , m_view(&m_scene)
    , m_isPressed(false)
    , m_graphicsWidget(nullptr)
{
    ui->setupUi(this);
    initWindow();
    initMyTitle();
    this->loadStyleSheet(":/Resources/LoginWindow/LoginWindow.css");
    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SIGNAL(hideWindow()));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

// 初始化标题栏;
void LoginWindow::initMyTitle()
{
    // 因为这里有控件层叠了，所以要注意控件raise()方法的调用顺序;
    m_titleBar->move(0, 0);
    m_titleBar->raise();
    m_titleBar->setBackgroundColor(0, 0, 0, true);
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
    // 这里需要设置成false，不允许通过标题栏拖动来移动窗口位置,否则会造成窗口位置错误;
    m_titleBar->setMoveParentWindowFlag(false);
    ui->loginButton->raise();
}

// 初始化窗口;
void LoginWindow::initWindow()
{
    //背景GIG图;
    QLabel* pBack = new QLabel(this);
    QMovie *movie = new QMovie();
    movie->setFileName(":/Resources/LoginWindow/back.gif");
    pBack->setMovie(movie);
    movie->start();
    pBack->move(0, 0);

    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    tcpClient->abort();                 //取消原有连接

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));//连接信号槽
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(ReadError(QAbstractSocket::SocketError)));
    ui->IPEdit->setPlaceholderText(QStringLiteral("Server IP"));//输入框隐藏
    ui->PORTEdit->setPlaceholderText(QStringLiteral("Port,PassWD"));
    ui->loginButton->setEnabled(true);//激活按键
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(on_loginButton_clicked()));
}

void LoginWindow::ReadData()
{
    int GetIndex=-1;
    buffer = tcpClient->readAll();
    if(!buffer.isEmpty())
    {
       qDebug() << "client get:" << buffer <<endl;
       GetIndex=buffer.indexOf("SSW:");//判断命令
       if(GetIndex>=0)
       {
           GetIndex=-1;
           emit switchGetData();//按键状态
       }

       GetIndex=buffer.indexOf("Temp:");//判断命令
       if(GetIndex>=0)
       {
           GetIndex=-1;
           emit  TempGetData();
       }
    }
}

void LoginWindow::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    QMessageBox msgBox;
    msgBox.setText(tr("failed to connect server because %1").arg(tcpClient->errorString()));
    msgBox.exec();
}

void LoginWindow::on_loginButton_clicked()
{
    if(ui->IPEdit->text()!=nullptr&&ui->PORTEdit->text()!=nullptr)//输入框不为空
    {
        QString data,data1;
        int GetIndex=-1;
        data=ui->PORTEdit->text();
        GetIndex=data.indexOf(",");//字符
        if(GetIndex>=0)
        {
            data1=data.mid(0,GetIndex);
            data=data.mid(GetIndex+1,4);
            tcpClient->connectToHost(ui->IPEdit->text(),data1.toInt());
            if (tcpClient->waitForConnected(1000))  // 连接成功则进入
            {
                QString SendData="setID:";
                emit rotateWindow();
                SendData+=data;
                tcpClient->write(SendData.toLatin1());//发送配对密码
            }
        }
        else
        {
            ui->PORTEdit->setPlaceholderText(QStringLiteral("Formal Error:\",\""));
        }
    }
    else//为空时选择了
    {
        if ( ui->IPEdit->text()== nullptr)
             ui->IPEdit->setPlaceholderText(QStringLiteral("Please Input Server IP"));
        if ( ui->PORTEdit->text()== nullptr)
             ui->PORTEdit->setPlaceholderText(QStringLiteral("Please Input Port,PassWD"));
    }
}



void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos() + movePoint;
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x(), widgetPos.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void LoginWindow::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
 //   return __super::closeEvent(event);
}


