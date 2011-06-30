#ifndef CORE_CORE_H
#define CORE_CORE_H

#include <QPluginLoader>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QStringList>
#include <QHash>
#include <QList>
#include <QStandardItemModel>

#include <jack/jack.h>

class Core;

#include "updates.h"
#include "config.h"
#include "encoder.h"
#include "ext/machine.h"
#include "gears/master.h"
#include "gears/lineinput.h"
#include "gears/fileinput.h"

class Core : public QObject {

Q_OBJECT

public:
	explicit Core(QObject *parent = 0);

	// Funtionalities

	QString pluginsPath;
	void loadPlugins();

	Master *master;
	QString filename;

	Updates *updates;
	int checkUpdates();

	// Jack handling

	int jack_init();
	int load(QString filename);
	int start(bool record);
	int stop();
	int save();
	int save(QString filename);

	jack_client_t *client;
	jack_port_t *input_port[4];
	jack_port_t *output_port[4];

	// *** Store

	int beat_per_pattern, total_patterns;
	int sampling_rate, buffer_size;

	// Gears
	QStandardItemModel *gearsTree;
	QList<Machine *> gears;

	// Gears instances (machines)
	QList<Machine *> machines;

	// Machine render order
	QList<Machine *> order;

	// Methods

	int orderMachines();
	int toggleConnection(Machine *from, Machine *to);

signals:
	void messageCritical(const QString &title, const QString &text);
};

#endif
