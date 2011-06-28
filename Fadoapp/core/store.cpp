#include "store.h"

#include <cstdio>

Store::Store()
{
	beat_per_pattern = 16;
	total_patterns = 16;
	machineTree = new QStandardItemModel();
}


int Store::optimizeMachines(QHash<int, int> &changes)
{
	/*
	int max = 0;
	int i;

	foreach (i, machines.values()) if (i > max) max = i;
	QList<int> order;

	for (i = 0; i <= max; i++) if (machines.contains(i)) order.append(i);

	for (i = 0; i < order.size(); i++) if (order[i] != i) {
		machines[i] = machines[order[i]];
		machines.remove(order[i]);
		machines[i]->id = i;
		
		changes[order[i]] = i;

		foreach (int j, connections.keys()) {
			if (j == order[i]) {
				connections[i] = connections[j];
				connections.remove(j);
			} else {
				foreach (int k, connections[j].keys()) if (k == order[i]) {
					connections[i][k] = connections[j][k];
					connections[j].remove(k);
				}
			}
		}
	}
*/
	return 0;
}


int Store::orderMachines()
{
	/*
	QHash<int, QList<int> > oconn;
	QHash<int, QList<int> > iconn;
	QList<int> order;

	// oconn and iconn keys must exists
	foreach (int i, machines.keys()) {
		if (oconn.contains(i) == false) oconn[i].clear();
		if (iconn.contains(i) == false) iconn[i].clear();
	}

	// oconn e iconn initialization
	foreach (int i, connections.keys()) {
		foreach (int j, connections[i].keys()) {
			oconn[i] << j;
			iconn[j] << i;
		}
	}

	this->iconn = iconn;
	this->oconn = oconn;

	int watchdog = machines.keys().length();

	// *** Ordening machines to elaboration

	while (iconn.size() > 0 and --watchdog >= 0) { // While there are machines to check
		QList<int> iconnKeys = iconn.keys();
		for (int i = 0; i < iconnKeys.length(); i++) { // Cycle on in-connections hash
			if (iconn[iconnKeys[i]].length() == 0) { // If there are unmanaged in-connections
				QList<int> oconnValues = oconn.value(iconnKeys[i]);
				for (int j = 0; j < oconnValues.length(); j++) { // Get out-connections of this machine
					QList<int> t;
					QList<int> iconnValues = iconn.value(oconnValues[j]);
					for (int k = 0; k < iconnValues.length(); k++) { // Cycle on in-connections of child machines
						if (iconnValues[k] != iconnKeys[i]) { // Recreates the set without the machine
							t.append(iconnValues[k]);
						}
					}
					iconn[oconnValues[j]] = t;
				}

				order << iconnKeys[i];
				iconn.remove(iconnKeys[i]);
				oconn.remove(iconnKeys[i]);
			}
		}
	}

	// Loop
	if (watchdog < 0) return 1;

	this->order = order;
*/
	return 0;
}



int Store::flipConnection(int from, int to)
{
	/*
	if (connections.contains(from) and connections[from].contains(to)) {
		connections[from].remove(to);
		if (connections[from].isEmpty()) connections.remove(from);
		orderMachines();
		return 1;
	} else if (connections.contains(to) and connections[to].contains(from)) {
		connections[to].remove(from);
		if (connections[to].isEmpty()) connections.remove(to);
		orderMachines();
		return 1;
	} else {
		Machine *f = machines[from];
		Machine *t = machines[to];

		if (f->author == "Core" and f->name == "output") return 2;
		if (t->type == "generator") return 2;

		connections[from][to] = 100;
		orderMachines();
		return 0;
	}
	*/
}
