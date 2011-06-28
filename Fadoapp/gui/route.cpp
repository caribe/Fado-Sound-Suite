#include "gui/route.h"

Route::Route(Store *store)
{
	this->store = store;

	// Machine tree

	QTreeView *routeTree = new QTreeView(this);
	routeTree->setModel(store->gearsTree);
	connect(routeTree, SIGNAL(doubleClicked(QModelIndex)), SLOT(newMachine(QModelIndex)));

	// Editor

	routeScene = new QGraphicsScene(this);
	routeScene->setSceneRect(0, 0, 1440, 768);

	routeEditor = new QGraphicsView(routeScene, this);
	routeEditor->setRenderHints(QPainter::Antialiasing);
	routeEditor->centerOn(0, 0);

	extraline = new QGraphicsPathItem();
	extraline->hide();
	routeScene->addItem(extraline);

	// Layout

	addWidget(routeTree);
	addWidget(routeEditor);

	setStretchFactor(1, 1);

}


void Route::newMachine(const QModelIndex &index)
{
	int id = index.data(Qt::UserRole + 1).toInt();

	Machine *machine;

	if (id > 0) {
		machine = qobject_cast<Machine *>(store->gears[id]->instance());
	} else if (id == -1){
		machine  = (Machine *)new LineInput();
	} else if (id == -2) {
		machine  = (Machine *)new FileInput();
	}

	machine->id = store->machines.length();
	store->machines.append(machine);

	addMachine(machine);

	// mainWindow->refreshMachines();
}



void Route::addMachine(Machine *machine)
{
	MachineBox *box = new MachineBox(this, machine);
	machines[machine->id] = box;
	routeScene->addItem(box);
	qDebug() << "Added machine #" << machine->id << endl;
}



void Route::delMachine(Machine *machine)
{
	if (machines.contains(machine->id)) delMachine(machines[machine->id]);
}



void Route::delMachine(MachineBox *machine)
{
	/*
	qDebug() << "Remove #" << machine->id() << endl;
	machines.remove(machine->id());
	store->machines.remove(machine->id());
	// routeEditor->layout->removeWidget(machine);

	foreach (int from, store->connections.keys()) {
		if (from == machine->m->id) {
			store->connections.remove(from);
		} else {
			foreach (int to, store->connections[from].keys()) {
				if (to == machine->m->id) store->connections[from].remove(to);
			}
			if (store->connections.values(from).isEmpty()) store->connections.remove(from);
		}
	}

	foreach (LinkBox *linkBox, connections) {
		if (linkBox->m1 == machine or linkBox->m2 == machine) {
			connections.removeOne(linkBox);
			delete linkBox;
		}
	}

	delete machine->m;
	delete machine;

	mainWindow->core->optimizeMachines();
	QList<MachineBox *> temp = machines.values();
	machines.clear();
	foreach (MachineBox *m, temp) {
		machines[m->id()] = m;
	}
	*/
}


void Route::addConnection(int i1, int i2)
{
	/*
	if (machines.contains(i1) == false) {
		qDebug() << "Machine #" << i1 << " not found" << endl;
		return;
	}

	if (machines.contains(i2) == false) {
		qDebug() << "Machine #" << i2 << " not found" << endl;
		return;
	}

	LinkBox *linkBox = new LinkBox(this, machines[i1], machines[i2]);
	routeScene->addItem(linkBox);
	connections << linkBox;
	*/
}


void Route::delConnection(MachineBox *m1, MachineBox *m2)
{
	/*
	foreach (LinkBox *linkBox, connections) {
		if ((linkBox->m1 == m1 and linkBox->m2 == m2) or (linkBox->m1 == m2 and linkBox->m2 == m1)) {
			connections.removeOne(linkBox);
			routeScene->removeItem(linkBox);
			delete linkBox;
		}
	}
	*/
}


void Route::moveConnection(MachineBox *m)
{
	foreach (LinkBox *linkBox, connections) {
		if (linkBox->m1 == m or linkBox->m2 == m) {
			linkBox->repos();
		}
	}
}



void Route::connectionStart()
{
	extraline->show();
}


void Route::connectionMove(int x1, int y1, int x2, int y2)
{
	QPainterPath *path = new QPainterPath(QPointF(x1, y1));
	path->lineTo(x2, y2);
	extraline->setPath(*path);
}


void Route::connectionFinish(MachineBox *m1, int x, int y)
{
	extraline->hide();
	foreach (MachineBox *m2, machines.values()) {
		// out << "Check #" << m1->id() << " #" << m2->id() << endl;
		if (x >= m2->x() - 50 and x <= m2->x() + 50 and y >= m2->y() - 25 and y <= m2->y() + 25) {
			int flag = store->flipConnection(m1->id(), m2->id());
			// out << "Connection " << flag << endl;

			if (store->orderMachines() == 1) { // Loop detected
				QMessageBox::critical(this, "Connection creates loop", "Sorry, but the new connection will create a loop and therefore it can't be inserted");
				store->flipConnection(m1->id(), m2->id());
			} else {
				if (flag == 0) {
					addConnection(m1->id(), m2->id());
				} else if (flag == 1) {
					delConnection(m1, m2);
				}
			}

			return;
		}
	}
}
