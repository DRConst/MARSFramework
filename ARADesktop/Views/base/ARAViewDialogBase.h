#pragma once

#include "ARAViewBase.h"

#include <QDialog>

class ARAViewDialogBase : public ARAViewBase
{
		Q_OBJECT

	public:
		ARAViewDialogBase(QObject *parent);
		~ARAViewDialogBase();

		virtual void show();
		virtual void exec();
};
