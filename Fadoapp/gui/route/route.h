/*
 * Fado Sound Suite - Modular synthetic music generator and sound processor
 *
 * Copyright (C) 2011 Vincenzo Buttazzo <vbuttazzo@yahoo.com>
 *
 * This file is part of Fado Sound Suite.
 *
 * Fado Sound Suite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fado Sound Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
#include <QMenu>
#include <QInputDialog>

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
		QMenu *routeEditorMenu;
		QGraphicsScene *routeScene;

		QHash<Machine *, MachineBox *> machineBoxes;
		QLinkedList<LinkBox *> connections;
		QGraphicsPathItem *extraline;

		void addMachine(Machine *machine);
		void delMachine(MachineBox *machine);
		void delMachine(Machine *machine);
		void renMachine(MachineBox *machineBox);

		void addConnection(Machine *m1, Machine *m2);
		void addConnection(MachineBox *m1, MachineBox *m2);
		void delConnection(MachineBox *m1, MachineBox *m2);
		void delConnection(LinkBox *lb);

		void moveConnection(MachineBox *m);
		void delConnection(Machine *m1, Machine *m2);

		void connectionStart();
		void connectionMove(int x1, int y1, int x2, int y2);
		void connectionFinish(MachineBox *, int x, int y);

		void slotDisplayPatterns(Machine *m);

	signals:
		void signalDisplayStatus(const QString &);
		void signalClearStatus();
		void machinesChanged();
		void signalDisplayPatterns(Machine *);

	public slots:
		void slotDisplayStatus(const QString &);
		void slotClearStatus();

	private slots:
		void newMachine(const QModelIndex &index);
};


#endif
