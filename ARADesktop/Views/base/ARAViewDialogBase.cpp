#include "ARAViewDialogBase.h"

ARAViewDialogBase::ARAViewDialogBase(QObject *parent)
	: ARAViewBase(parent)
{
	this->ara_widget = new QDialog();
}

ARAViewDialogBase::~ARAViewDialogBase()
{
}

void ARAViewDialogBase::show()
{
	qobject_cast<QDialog *>(this->ara_widget)->show();
}

void ARAViewDialogBase::exec()
{
	qobject_cast<QDialog *>(this->ara_widget)->exec();
}