#ifndef STORE_H
#define STORE_H

#include <QStringList>
#include <QHash>
#include <QList>
#include <QStandardItemModel>
#include <QPluginLoader>

class Store;

#include "../ext/machine.h"

#define PI_2 6.28318530718

//! General store of machines
class Store : public QObject {
	public:
		//! Constructor
		Store();

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
};

#endif
