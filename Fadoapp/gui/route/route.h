#ifndef GUI_ROUTE_H
#define GUI_ROUTE_H

#include <QFrame>
#include <QGridLayout>
#include <QGraphicsView>
#include <QLinkedList>
#include <QSplitter>
#include <QTreeView>
#include <QMessageBox>
#include <QDebug>

class Route;

#include "machinebox.h"
#include "linkbox.h"
#include "core/core.h"
#include "gears/lineinput.h"
#include "gears/fileinput.h"

class Route : public QSplitter
{
	Q_OBJECT

	public:
		Route(QWidget *parent, Core *core);
		Core *core;

		QGraphicsView *routeEditor;
		QGraphicsScene *routeScene;

		QHash<Machine *, MachineBox *> machineBoxes;
		QLinkedList<LinkBox *> connections;
		QGraphicsPathItem *extraline;

		void addMachine(Machine *machine);
		void delMachine(MachineBox *machine);
		void delMachine(Machine *machine);

		void addConnection(MachineBox *m1, MachineBox *m2);
		void delConnection(MachineBox *m1, MachineBox *m2);
		void delConnection(LinkBox *lb);

		void moveConnection(MachineBox *m);
		void delConnection(Machine *m1, Machine *m2);

		void connectionStart();
		void connectionMove(int x1, int y1, int x2, int y2);
		void connectionFinish(MachineBox *, int x, int y);

	signals:
		void signalDisplayStatus(const QString &);
		void signalClearStatus();
		void machinesChanged();

	public slots:
		void slotDisplayStatus(const QString &);
		void slotClearStatus();

	private slots:
		void newMachine(const QModelIndex &index);
};


#endif
