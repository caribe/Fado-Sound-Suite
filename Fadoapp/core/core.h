#ifndef CORE_CORE_H
#define CORE_CORE_H

#include <jack/jack.h>

#include <Qt/qlibrary.h>
#include <QtCore>

class Core;

#include "core/settings.h"
#include "core/updates.h"
#include "core/master.h"
#include "core/lineinput.h"
#include "core/fileinput.h"
#include "core/config.h"
#include "core/encoder.h"

#include "../Fadolib/store.h"
#include "../Fadolib/machine.h"

#include "mainwindow.h"

#define LIB_PATH "../Fadogears/"

class Core {
	public:
		Core(MainWindow *mainWindow);
		int init();
		int jack_init();
		int load(QString filename);
		int start(bool record);
		int stop();
		int save();
		int save(QString filename);
		int close();
		int optimizeMachines();
		int checkUpdates();

		Store *store;
		MainWindow *mainWindow;
		Settings *settings;
		Updates *updates;
		Master *master;
		QString errstr;
		QString filename;

		jack_client_t *client;
		jack_port_t *input_port[4];
		jack_port_t *output_port[4];
};

#endif
