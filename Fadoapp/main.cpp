#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("Vastapalla");
	QCoreApplication::setOrganizationDomain("saitfainder.altervista.org");
	QCoreApplication::setApplicationName("Fado");
	QCoreApplication::setApplicationVersion("1.0.0");

	QPixmap pixmap(":/splashscreen.jpg");
	QSplashScreen splash(pixmap);
	splash.show();

	MainWindow w;
	w.show();
	splash.finish(&w);
	return app.exec();
}
