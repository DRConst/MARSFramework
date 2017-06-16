#include "ARAViewBase.h"

ARAViewBase::ARAViewBase(QObject *parent) : QObject(parent), ara_widget(nullptr)
{
}

ARAViewBase::~ARAViewBase()
{
	if(this->ara_widget != nullptr)
		this->ara_widget->deleteLater();
}

QWidget *ARAViewBase::getWidget()
{
	return this->ara_widget;
}

void ARAViewBase::show()
{
    this->ara_widget->show();
}
