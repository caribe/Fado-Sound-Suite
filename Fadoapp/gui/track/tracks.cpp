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

#include "tracks.h"

Tracks::Tracks(QWidget *parent, Core *core) : QSplitter(parent)
{
	this->core = core;

	// Tracks

	tracksModel = new TracksModel(this, core);

	tracksTable =  new QTableView(this);
	tracksTable->setModel(tracksModel);
	tracksTable->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(tracksTable->selectionModel(), SIGNAL(currentColumnChanged(QModelIndex,QModelIndex)), SLOT(refreshPatterns()));

	addWidget(tracksTable);

	// Hint

	hintList = new QListWidget();
	connect(hintList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(itemDoubleClickedSlot(QListWidgetItem *)));
	addWidget(hintList);
}


void Tracks::refreshMachines()
{
	tracksModel->reload();
}


void Tracks::refreshPatterns()
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	hintList->clear();

	foreach (MachinePattern *pat, machine->patterns) {
		hintList->addItem(pat->name);
	}
}



void Tracks::addButtonSlot()
{
	tracksModel->insertRows(tracksModel->rowCount(), 1);
}


void Tracks::delButtonSlot()
{
	tracksModel->removeRows(tracksModel->rowCount()-1, 1);
}



void Tracks::frsButtonSlot()
{
	int row = tracksTable->currentIndex().row();
	if (row == -1) row = 0;
	core->track_first = row;
	tracksTable->reset();
}


void Tracks::lstButtonSlot()
{
	int row = tracksTable->currentIndex().row();
	if (row == -1) row = core->total_patterns - 1;
	if (row < core->track_first) row = core->track_first;
	core->track_last = row;
	tracksTable->reset();
}



void Tracks::itemDoubleClickedSlot(QListWidgetItem *item)
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	QModelIndex index = tracksTable->currentIndex();
	machine->track[index.row()] = machine->patterns[hintList->currentRow()];

	selectNext();
}



void Tracks::deleteButtonSlot() {
	QModelIndex index = tracksTable->currentIndex();

	tracksModel->clear(index);

	selectNext();
}



void Tracks::muteButtonSlot()
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	QModelIndex index = tracksTable->currentIndex();
	machine->track[index.row()] = core->mutePattern;

	selectNext();
}



void Tracks::breakButtonSlot()
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	QModelIndex index = tracksTable->currentIndex();
	machine->track[index.row()] = core->breakPattern;

	selectNext();
}



void Tracks::selectNext() {
	QModelIndex index = tracksTable->currentIndex();
	int currentRow = index.row();
	int nextRow = (currentRow + 1) % tracksModel->rowCount();
	tracksTable->setCurrentIndex(index.sibling(nextRow, index.column()));
}



Machine *Tracks::currentMachine()
{
	QModelIndex index = tracksTable->currentIndex();
	if (index.isValid()) return core->machines[index.column()];
	return 0;
}
