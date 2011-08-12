/*
 * Fado Sound Suite - Modular synthetic music generator and sound processor
 *
 * Copyright (C) 2011 Vincenzo Buttazzo <vbuttazzo@yahoo.com>
 *
 * This file is part of Fado Sound Suite.
 *
 * Fado Sound Suite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fado Sound Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * aunsigned long with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("Vastapalla");
	QCoreApplication::setOrganizationDomain("saitfainder.slakko.org");
	QCoreApplication::setApplicationName("Fado");
	QCoreApplication::setApplicationVersion("1.0.0");

	QPixmap pixmap(":/icons/splashscreen.jpg");
	QSplashScreen splash(pixmap);
	splash.show();

	MainWindow w;
	splash.finish(&w);
	w.check();
	w.show();

	return app.exec();
}
