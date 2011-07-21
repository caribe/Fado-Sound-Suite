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

#ifndef GUI_PATTERTABLE_H
#define GUI_PATTERTABLE_H

#include <QTableView>
#include <QKeyEvent>

class PatternTable;

#include "pattern.h"
#include "patterntabledata.h"

class PatternTable : public QTableView
{
	Q_OBJECT

	public:
		Pattern *pattern;
		PatternTable(Pattern *pattern);

		PatternTableData *model;
		void refresh();

		void keyPressEvent(QKeyEvent *event);
		
		QList<int> keyMap;
		QList<QString> notes;

	private slots:
		void currentColumnChangedSlot(const QModelIndex &current, const QModelIndex &previous);
};

#endif
