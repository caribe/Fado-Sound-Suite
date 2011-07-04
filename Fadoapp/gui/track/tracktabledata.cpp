#include "tracktabledata.h"

TrackTableData::TrackTableData(Track *track, Core *core)
{
	this->core = core;
	this->track = track;
}


void TrackTableData::reload() {
	reset();
}


int TrackTableData::columnCount(const QModelIndex& parent) const
{
	return core->machines.size();
}



int TrackTableData::rowCount(const QModelIndex& parent) const
{
	return core->total_patterns;
}



QVariant TrackTableData::data(const QModelIndex& index, int role) const
{
	if (index.isValid() == false) {
		return QVariant();
	} else if (role == Qt::BackgroundRole) {
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
		if (pattern->type == MachinePattern::MutePattern) {
			return QVariant("<mute>");
		} else if (pattern->type == MachinePattern::BreakPattern) {
			return QVariant("<break>");
		} else {
			return pattern->name;
		}
	}

	return QVariant();
}



QVariant TrackTableData::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) { 
		return QVariant();
	} else if (orientation == Qt::Horizontal) {
		return QVariant(core->machines[section]->name);
	} else if (orientation == Qt::Vertical) {
		return QVariant(section);
	}

	return QVariant();
}
