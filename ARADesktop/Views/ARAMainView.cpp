#include "ARAMainView.h"
#include "ui_ARAMainView.h"

ARAMainView::ARAMainView(QObject *parent): ARAViewDialogBase(parent), ara_ui(new Ui::ARAMainView)
{
	this->ara_ui->setupUi(this->ara_widget);
}

ARAMainView::~ARAMainView()
{
}
