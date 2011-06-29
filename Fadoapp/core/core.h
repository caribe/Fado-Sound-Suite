#ifndef CORE_CORE_H
#define CORE_CORE_H

#include <QPluginLoader>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include <jack/jack.h>

class Core;

#include "core/updates.h"
#include "core/config.h"
#include "core/encoder.h"
#include "core/store.h"
#include "ext/machine.h"
#include "gears/master.h"
#include "gears/lineinput.h"
#include "gears/fileinput.h"

class Core : public QObject {

Q_OBJECT

public:
	explicit Core(QObject *parent = 0);
	void loadPlugins();
	int jack_init();
	int load(QString filename);
	int start(bool record);
	int stop();
	int save();
	int save(QString filename);
	int optimizeMachines();
	int checkUpdates();

	Store *store;
	Updates *updates;
	Master *master;
	QString errstr;
	QString filename;

	jack_client_t *client;
	jack_port_t *input_port[4];
	jack_port_t *output_port[4];

	QString pluginsPath;

signals:
	void messageCritical(const QString &title, const QString &text);
};

#endif
