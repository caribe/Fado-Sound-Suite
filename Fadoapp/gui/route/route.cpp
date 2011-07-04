#include "route.h"

Route::Route(QWidget *parent, Core *core) : QSplitter(parent)
{
	this->core = core;

	// Machine tree

	QTreeView *routeTree = new QTreeView(this);
	routeTree->setModel(core->gearsTree);
	routeTree->model()->setHeaderData(0, Qt::Horizontal, tr("Machines"));
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

	Machine *machine = 0;

	if (id >= 0) {
		machine = core->gears[id]->factory();
		qDebug() << "Created" << machine;
	} else if (id == -2) {
		machine = (Machine *)new LineInput();
	} else if (id == -3) {
		machine = (Machine *)new FileInput();
	} else {
		qDebug() << "Unknown machine" << id;
	}

	if (machine) {
		machine->x = machine->y = -1; // Default position
		core->machines.append(machine);
		core->orderMachines();
		addMachine(machine);
	}
}



void Route::addMachine(Machine *machine)
{
	MachineBox *machineBox = new MachineBox(this, machine);
	machineBoxes[machine] = machineBox;
	routeScene->addItem(machineBox);
	qDebug() << "Added machine #" << machine->name;
	emit machinesChanged();
}



void Route::delMachine(Machine *machine)
{
	if (machineBoxes.contains(machine)) {

		qDebug() << "Remove #" << machine->name << endl;

		// Removing connections

		foreach (LinkBox *linkBox, connections) {
			if (linkBox->m1->m == machine or linkBox->m2->m == machine) {
				connections.removeOne(linkBox);
				delete linkBox;
			}
		}

		foreach (Machine *dst, machine->connectionDst.keys()) core->toggleConnection(machine, dst);
		foreach (Machine *src, machine->connectionSrc.keys()) core->toggleConnection(src, machine);

		// Machine destruction

		delete machineBoxes[machine]; // Destroys the MachineBox
		machineBoxes.remove(machine); // Removes the hash entry

		core->machines.removeOne(machine); // Deletes the machine from store list
		// delete machine; // Destroys the machine instance

		// Graph update

		core->orderMachines();

	}
}



void Route::delMachine(MachineBox *machineBox)
{
	delMachine(machineBox->m);
}


void Route::addConnection(MachineBox *m1, MachineBox *m2)
{
	LinkBox *linkBox = new LinkBox(this, m1, m2);
	routeScene->addItem(linkBox);
	connections << linkBox;
}



void Route::delConnection(LinkBox *linkBox)
{
	connections.removeOne(linkBox);
	routeScene->removeItem(linkBox);
	delete linkBox;
}



void Route::delConnection(MachineBox *m1, MachineBox *m2)
{
	foreach (LinkBox *linkBox, connections) {
		if ((linkBox->m1 == m1 and linkBox->m2 == m2) or (linkBox->m1 == m2 and linkBox->m2 == m1)) {
			delConnection(linkBox);
		}
	}
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

	foreach (MachineBox *m2, machineBoxes) {
		// out << "Check #" << m1->id() << " #" << m2->id() << endl;
		if (x >= m2->x() - 50 and x <= m2->x() + 50 and y >= m2->y() - 25 and y <= m2->y() + 25) {
			int flag = core->toggleConnection(m1->m, m2->m);
			// out << "Connection " << flag << endl;

			if (core->orderMachines()) {
				if (flag == 0) {
					addConnection(m1, m2);
				} else if (flag == 1) {
					delConnection(m1, m2);
				}
			} else { // Loop detected
				QMessageBox::critical(this->parentWidget(), "Connection creates loop", "Sorry, but the new connection will create a loop and therefore it can't be inserted");
				core->toggleConnection(m1->m, m2->m);
			}

			return;
		}
	}
}



void Route::slotDisplayStatus(const QString &status)
{
	emit signalDisplayStatus(status);
}



void Route::slotClearStatus()
{
	emit signalClearStatus();
}
