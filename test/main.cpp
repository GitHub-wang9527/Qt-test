#include "PixLabel.h"
#include "EventFWidget.h"
#include "PropertyButton.h"


#include "CheckboxCombo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    PixLabel pix;pix.show();
	return a.exec();
}
