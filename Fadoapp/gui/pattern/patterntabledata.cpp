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

#include "patterntabledata.h"

PatternTableData::PatternTableData(Pattern *pattern)
{
	this->pattern = pattern;
}


void PatternTableData::reload() {
	reset();
}


int PatternTableData::columnCount(const QModelIndex& parent) const
{
	Machine *machine = pattern->currentMachine();
	if (machine == 0) {
		return 0;
	} else {
		return machine->params.size();
	}
}



int PatternTableData::rowCount(const QModelIndex& parent) const
{
	return 16;
}



QVariant PatternTableData::data(const QModelIndex& index, int role) const
{
	Machine *machine = pattern->currentMachine();

	if (machine != 0 and index.isValid() and (role == Qt::DisplayRole or role== Qt::EditRole)) {
		MachinePattern *pat = pattern->currentPattern();
		if (pat != 0) {
			int row = index.row();
			int col = index.column();
			if (pat->params.contains(row) and pat->params[row].contains(col)) {
				return pat->params[index.row()][index.column()];
			} else {
				return QVariant();
			}
		}
	}

	return QVariant();
}


Qt::ItemFlags PatternTableData::flags(const QModelIndex & index) const
{
	Machine *machine = pattern->currentMachine();
	if (machine == 0) return 0;

	if (machine->params[index.column()]->type == Param::ParamNote) {
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	}
}


bool PatternTableData::setData(const QModelIndex& index, const QVariant & value, int role)
{
	Machine *machine = pattern->currentMachine();
	if (machine == 0) return 0;

	MachinePattern *pat = pattern->currentPattern();
	int row = index.row();
	int col = index.column();

	QString strValue = value.toString();

	strValue = machine->validate(col, strValue);

	pat->params[row][col] = strValue;

	return true;
}


QVariant PatternTableData::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			Machine *machine = pattern->currentMachine();
			if (machine == 0) return QVariant();
			return machine->params[section]->name;
		} else if (orientation == Qt::Vertical) {
			return section;
		}
	}

	return QVariant();
}

