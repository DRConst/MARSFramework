#include "ARAControllerBase.h"
#include "ARAViewBase.h"

ARAControllerBase::ARAControllerBase(QObject *parent)
	: QObject(parent)
{
}

ARAControllerBase::~ARAControllerBase()
{
}

ARAViewBase *ARAControllerBase::getView() const
{
    return this->ara_view;
}
