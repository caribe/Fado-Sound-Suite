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

#include "tracksmodel.h"

TracksModel::TracksModel(Tracks *track, Core *core)
{
	this->core = core;
	this->track = track;
}

void TracksModel::reload() {
	reset();
}


int TracksModel::columnCount(const QModelIndex &parent) const
{
	return core->machines.length();
}



int TracksModel::rowCount(const QModelIndex &parent) const
{
	return core->total_patterns;
}



QVariant TracksModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid()) {
		if (role == Qt::BackgroundRole) {
			if (core->track_first == index.row()) {
				if (core->track_last == index.row()) {
					return QBrush(QColor(0xff, 0xff, 0x80));
				} else {
					return QBrush(QColor(0x80, 0xff, 0x80));
				}
			} else if (core->track_last == index.row()) {
				return QBrush(QColor(0xff, 0x80, 0x80));
			}
		} else if (role == Qt::DisplayRole or role == Qt::EditRole) {
			Machine *machine = core->machines[index.column()];
			if (machine->track.contains(index.row())) {
				MachinePattern *pattern = machine->track[index.row()];
				if (pattern->type == MachinePattern::MutePattern) {
					return QVariant("<mute>");
				} else if (pattern->type == MachinePattern::BreakPattern) {
					return QVariant("<break>");
				} else {
					return pattern->name;
				}
			} else {
				return QVariant();
			}
		}
	}

	return QVariant();
}



QVariant TracksModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			if (core->machines[section]->alias.isNull()) {
				return core->machines[section]->name;
			} else {
				return core->machines[section]->alias;
			}
		} else if (orientation == Qt::Vertical) {
			return QVariant(section);
		}
	}

	return QVariant();
}


void TracksModel::clear(const QModelIndex &index) {
	if (index.isValid()) {
		Machine *machine = core->machines[index.column()];
		if (machine != NULL) {
			machine->track.remove(index.row());
			dataChanged(index, index);
		}
	}
}



bool TracksModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row+count-1);
	core->total_patterns++;
	endInsertRows();
	return true;
}



bool TracksModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (core->total_patterns == 0) {
		return false;
	} else {
		beginRemoveRows(parent, row, row+count-1);

		foreach (Machine *m, core->machines) {
			m->track.remove(row);
		}
		core->total_patterns--;

		if (core->track_first >= core->total_patterns) core->track_first = core->total_patterns - 1;
		if (core->track_last >= core->total_patterns) core->track_last = core->total_patterns - 1;

		endRemoveRows();

		return true;
	}
}
