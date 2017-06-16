#pragma once

#include <QObject>

class ARAViewBase;

class ARAControllerBase : public QObject
{
		Q_OBJECT

	public:
		ARAControllerBase(QObject *parent = nullptr);
		~ARAControllerBase();

        ARAViewBase *getView() const;

    protected:
        ARAViewBase *ara_view;
    
        ARAControllerBase *ara_parentController;
};
