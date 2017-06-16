#include "ARAMainController.h"
#include "ARAMainView.h"

ARAMainController::ARAMainController(ARAControllerBase *parent): ARAControllerBase(parent)
{
    this->ara_view = new ARAMainView();

    this->ara_view->show();
}

ARAMainController::~ARAMainController()
{
}