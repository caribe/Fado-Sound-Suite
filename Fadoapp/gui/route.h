#ifndef GUI_ROUTE_H
#define GUI_ROUTE_H

#include <QtCore>
#include <QtGui>
#include <QDebug>

class Route;

#include "gui/routetree.h"
#include "mainwindow.h"
#include "gui/machinebox.h"
#include "gui/linkbox.h"

class Route : public QFrame
{
	Q_OBJECT

	public:
		Route(MainWindow *mainWindow);

		MainWindow *mainWindow;
		QGridLayout grid;
		QGraphicsView *routeEditor;
		QGraphicsScene *routeScene;
		QHash<int, MachineBox *> machines;
		QLinkedList<LinkBox *> connections;
		Store *store;
		QGraphicsPathItem *extraline;

		void addMachine(Machine *machine);
		void delMachine(MachineBox *machine);
		void delMachine(Machine *machine);
		void newMachine(QString type, QString author, QString code);

		void addConnection(int i1, int i2);
		void delConnection(MachineBox *m1, MachineBox *m2);
		void moveConnection(MachineBox *m);
		void delConnection(Machine *m1, Machine *m2);

		void connectionStart();
		void connectionMove(int x1, int y1, int x2, int y2);
		void connectionFinish(MachineBox *, int x, int y);
};


#endif
