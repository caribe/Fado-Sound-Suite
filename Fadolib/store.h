#ifndef STORE_H
#define STORE_H

#include <QStringList>
#include <QHash>

#include "machine.h"

#define PI_2 6.28318530718

class Machine;

typedef Machine* (*MachineFactory)();
typedef QStringList (*MachineInfo)();

//! General store of machines
class Store {
	public:
		//! Constructor
		Store();

		QHash<QString, QHash<QString, QHash<QString, MachineFactory> > > factory;
		QHash<QString, QHash<QString, QHash<QString, MachineInfo> > > info;

		int beat_per_pattern, total_patterns;

		QHash<int, Machine *> machines;
		QHash<int, QHash<int, int> > connections;

		QHash<int, QList<int> > oconn;
		QHash<int, QList<int> > iconn;
		QList<int> order;
		int sampling_rate, buffer_size;

		int optimizeMachines(QHash<int, int> &changes);
		int orderMachines();

		int flipConnection(int from, int to);
};

#endif
