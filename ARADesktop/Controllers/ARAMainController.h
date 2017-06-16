#pragma once

#include "ARAControllerBase.h"

class ARAViewBase;

class ARAMainController : public ARAControllerBase
{
	Q_OBJECT

	public:
		ARAMainController(ARAControllerBase *parent = nullptr);
		~ARAMainController();
};
