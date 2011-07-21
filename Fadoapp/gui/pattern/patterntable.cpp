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

#include "patterntable.h"

PatternTable::PatternTable(Pattern *pattern) {
	this->pattern = pattern;

	keyMap << Qt::Key_Z << Qt::Key_S << Qt::Key_X << Qt::Key_D << Qt::Key_C << Qt::Key_V << Qt::Key_G << Qt::Key_B << Qt::Key_H << Qt::Key_N << Qt::Key_J << Qt::Key_M;
	notes << "C" << "C#" << "D" << "D#" << "E" << "F" << "F#" << "G" << "G#" << "A" << "A#" << "B";

	setSelectionMode(QAbstractItemView::SingleSelection);

	setFont(QFont("TypeWriter", 6));
	
	model = new PatternTableData(pattern);
	setModel(model);

	QItemSelectionModel *selModel = selectionModel();
	connect(selModel, SIGNAL(currentColumnChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentColumnChangedSlot(const QModelIndex &, const QModelIndex &)));
}


void PatternTable::refresh()
{
	model->reload();
}


void PatternTable::currentColumnChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
	pattern->activeCol(current.column());
}


void PatternTable::keyPressEvent(QKeyEvent *event)
{
	int i = keyMap.indexOf(event->key());
	QModelIndex index = currentIndex();

	if (i >= 0) {
		model->setData(index, QVariant(notes[i]+pattern->octaveCombo->currentText()), 0);
		update(index);
		setCurrentIndex(index.sibling((index.row()+1) % model->rowCount(), index.column()));
	} else if (event->key() == Qt::Key_Delete) {
		model->setData(index, QVariant(""), 0);
		update(index);
		setCurrentIndex(index.sibling((index.row()+1) % model->rowCount(), index.column()));
	} else {
		QTableView::keyPressEvent(event);
	}
}
