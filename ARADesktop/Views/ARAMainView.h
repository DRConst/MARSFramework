#pragma once

#include "ARAViewDialogBase.h"

namespace Ui
{
	class ARAMainView;
}

class ARAMainView : public ARAViewDialogBase
{
	Q_OBJECT

    public:
	    ARAMainView(QObject *parent = Q_NULLPTR);
	    ~ARAMainView();

    private:
	    Ui::ARAMainView *ara_ui;
};
