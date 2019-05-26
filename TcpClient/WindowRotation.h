#pragma once

#include <QtWidgets/QWidget>
#include "ui_WindowRotation.h"

class WindowRotation : public QWidget
{
	Q_OBJECT

public:
	WindowRotation(QWidget *parent = Q_NULLPTR);

private:
	Ui::WindowRotationClass ui;
};
