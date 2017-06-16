#include <QtWidgets/QApplication>

#include "ARAMainController.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    ARAMainController mainCont;

	return a.exec();
}
