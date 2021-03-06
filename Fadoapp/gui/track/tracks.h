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
 * aunsigned long with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GUI_TRACKS_H
#define GUI_TRACKS_H

#include <QSplitter>
#include <QListWidget>
#include <QGroupBox>
#include <QTableView>
#include <QDebug>

class Tracks;

#include "gears/master.h"
#include "tracksmodel.h"

class Tracks : public QSplitter
{
	Q_OBJECT

	public:
		Tracks(QWidget *parent, Core *core);
		
		Core *core;
		QListWidget *hintList;
		QTableView *tracksTable;
		TracksModel *tracksModel;

		Machine *currentMachine();
		void refreshMachines();

	private:
		void selectNext();

	public slots:
		void refreshPatterns();
		void addButtonSlot();
		void delButtonSlot();
		void frsButtonSlot();
		void lstButtonSlot();
		void deleteButtonSlot();
		void muteButtonSlot();
		void breakButtonSlot();

	private slots:
		void itemDoubleClickedSlot(QListWidgetItem *item);
};

#endif
