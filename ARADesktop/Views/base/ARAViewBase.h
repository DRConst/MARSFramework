#pragma once

#include "ARAViewBase.h"

#include <QObject>
#include <QWidget>

class ARAViewBase : public QObject
{
		Q_OBJECT

	public:
		ARAViewBase(QObject *parent);
		~ARAViewBase();

		QWidget *getWidget();

        void show();

	protected:
		QWidget *ara_widget;
};
