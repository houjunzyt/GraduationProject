#include "menuwindow.h"
#include <QPainter>
#include <QMovie>
#include <QMouseEvent>
#include "switchbutton.h"
#include "temperature.h"
#include "settime.h"
#include "basewindow.h"
#include <QDebug>
#include "loginwindow.h"
#include "weather.h"

extern LoginWindow* m_loginWindow;
extern Menuwindow* m_loginNetSetWindow;
Switch *switchwidget=nullptr;

Menuwindow::Menuwindow(QWidget *parent)
    :BaseWindow(parent)
{
	ui.setupUi(this);
	initWindow();
	initMyTitle();
}

Menuwindow::~Menuwindow()
{

}

void Menuwindow::initMyTitle()
{
    m_titleBar->move(0, 0);
    m_titleBar->raise();
    m_titleBar->setBackgroundColor(0, 0, 0, true);
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
    // 这里需要设置成false，不允许通过标题栏拖动来移动窗口位置,否则会造成窗口位置错误;
    m_titleBar->setMoveParentWindowFlag(false);
    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SIGNAL(hideWindow()));
}

void Menuwindow::initWindow()
{
	//背景GIG图;
	QMovie *movie = new QMovie();
	movie->setFileName(":/Resources/NetSetWindow/headBack.gif");
    ui.GifLabel->setMovie(movie);
	movie->start();
    ui.GifLabel->move(0, 0);

    //定义参数
    switchwidget = new Switch(this);
    Temperature *temperaturewidget = new Temperature(this);
    Settime *settimewidget = new Settime(this);
    Weather *weatherwidget = new Weather(this);
    //添加页面
    ui.stackedWidget->addWidget(temperaturewidget);
    ui.stackedWidget->addWidget(switchwidget);
    ui.stackedWidget->addWidget(settimewidget);
    ui.stackedWidget->addWidget(weatherwidget);
    //默认页面
    ui.stackedWidget->setCurrentWidget(temperaturewidget);
    QString BtnStyle ="\
                                QPushButton\
                                {\
                                    color:white;\
                                    border-radius:5px;\
                                }\
                                QPushButton:hover\
                                {\
                                    color:white;\
                                }";
    ui.TempButton->setStyleSheet(BtnStyle+"QPushButton:pressed\
                                            {\
                                                color:red;\
                                                padding-left:3px;\
                                                padding-top:3px;\
                                            }");
    ui.SwitchButton->setStyleSheet(BtnStyle+"QPushButton:pressed\
                                            {\
                                                color:green;\
                                                padding-left:3px;\
                                                padding-top:3px;\
                                            }");
    ui.CrtlButton->setStyleSheet(BtnStyle+"QPushButton:pressed\
                                            {\
                                                color:blue;\
                                                padding-left:3px;\
                                                padding-top:3px;\
                                            }");
    ui.WeatherButton->setStyleSheet(BtnStyle+"QPushButton:pressed\
                                            {\
                                                color:blue;\
                                                padding-left:3px;\
                                                padding-top:3px;\
                                            }");
}

void Menuwindow::onButtonCloseClicked()
{
    this->close();
}

void Menuwindow::on_TempButton_clicked()
{
   if(ui.stackedWidget->currentIndex()!=2)
   {
       ui.stackedWidget->setCurrentIndex(2);
   }
}

void Menuwindow::on_SwitchButton_clicked()
{
    if(ui.stackedWidget->currentIndex()!=3)
    {
        ui.stackedWidget->setCurrentIndex(3);
    }

}

void Menuwindow::on_CrtlButton_clicked()
{
    if(ui.stackedWidget->currentIndex()!=4)
    {
        ui.stackedWidget->setCurrentIndex(4);
    }
}

void Menuwindow::on_WeatherButton_clicked()
{
    if(ui.stackedWidget->currentIndex()!=5)
    {
        ui.stackedWidget->setCurrentIndex(5);
    }
}

void Menuwindow::paintEvent(QPaintEvent *event)
{
	// 绘制背景图;
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);
	pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(pathBack, QBrush(QColor(235, 242, 249)));
	
	QPainterPath pathBottom;
	pathBottom.setFillRule(Qt::WindingFill);
	pathBottom.addRoundedRect(QRect(0, 300, this->width(), this->height() - 300), 3, 3);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(pathBottom, QBrush(QColor(205, 226, 242)));

	painter.setPen(QPen(QColor(160 , 175 , 189)));
	painter.drawRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
}

// 以下通过mousePressEvent、mouseMoveEvent、mouseReleaseEvent三个事件实现了鼠标拖动标题栏移动窗口的效果;
void Menuwindow::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}

void Menuwindow::mouseMoveEvent(QMouseEvent *event)
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

void Menuwindow::mouseReleaseEvent(QMouseEvent *event)
{
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

void Menuwindow::closeEvent(QCloseEvent *event)
{
	emit closeWindow();
 //   return __super::closeEvent(event);
}
