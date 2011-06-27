#ifndef CORE_SETTINGS_H
#define CORE_SETTINGS_H

#include <QtXml/QDomDocument>

class Settings;

#include "mainwindow.h"

class Settings
{
	public:
		Settings(Core *core);
		int load();
		void save();

		Core *core;
		QString filename;
		QString version;
};

#endif
