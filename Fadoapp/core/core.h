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

#define PI_2 6.28318530718

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
	void loadPlugins();
	int jack_init();
	int load(QString filename);
	int start(bool record);
	int stop();
	int save();
	int save(QString filename);
	int optimizeMachines();
	int checkUpdates();

	Updates *updates;
	Master *master;
	QString errstr;
	QString filename;

	jack_client_t *client;
	jack_port_t *input_port[4];
	jack_port_t *output_port[4];

	QString pluginsPath;

	// Store

	QStandardItemModel *gearsTree;

	int beat_per_pattern, total_patterns;

	QList<QPluginLoader *> gears;
	QList<Machine *> machines;
	QMultiHash<int, int> connections;

	QMultiHash<int, int> oconn;
	QMultiHash<int, int> iconn;
	QList<int> order;

	int sampling_rate, buffer_size;

	int optimizeMachines(QHash<int, int> &changes);
	int orderMachines();

	int flipConnection(int from, int to);


signals:
	void messageCritical(const QString &title, const QString &text);
};

#endif
