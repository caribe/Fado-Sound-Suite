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
			Master *master = (Master *)core->machines[0];
			if (master->track_first == index.row()) {
				if (master->track_last == index.row()) {
					return QBrush(QColor(0xff, 0xff, 0x80));
				} else {
					return QBrush(QColor(0x80, 0xff, 0x80));
				}
			} else if (master->track_last == index.row()) {
				return QBrush(QColor(0xff, 0x80, 0x80));
			}
		} else if (role == Qt::DisplayRole or role == Qt::EditRole) {
			Machine *machine = core->machines[index.column()];
			MachinePattern *pattern = machine->track[index.row()];
			if (pattern) {
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
			return core->machines[section]->name;
		} else if (orientation == Qt::Vertical) {
			return QVariant(section);
		}
	}

	return QVariant();
}
