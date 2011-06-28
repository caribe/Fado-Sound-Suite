#include "gui/tracktabledata.h"

TrackTableData::TrackTableData(Track *track, Store *store)
{
	this->store = store;
	this->track = track;
}


void TrackTableData::reload() {
	reset();
}


int TrackTableData::columnCount(const QModelIndex& parent) const
{
	return store->machines.size();
}



int TrackTableData::rowCount(const QModelIndex& parent) const
{
	return store->total_patterns;
}



QVariant TrackTableData::data(const QModelIndex& index, int role) const
{
	/*
	if (index.isValid() == false) {
		return QVariant();
	} else if (role == Qt::BackgroundRole) {
		Master *master = (Master *)store->machines[0];
		if (master->track_first == index.row()) {
			if (master->track_last == index.row()) {
				return QBrush(QColor(0xff, 0xff, 0x80));
			} else {
				return QBrush(QColor(0x80, 0xff, 0x80));
			}
		} else if (master->track_last == index.row()) {
			return QBrush(QColor(0xff, 0x80, 0x80));
		}
	} else if (role != Qt::DisplayRole) { 
		return QVariant();
	} else if (store->machines[index.column()]->track.contains(index.row())) {
		int pattern = store->machines[index.column()]->track[index.row()];
		if (pattern == -2) {
			return QVariant("<mute>");
		} else if (pattern == -1) {
			return QVariant("<break>");
		} else if (store->machines[index.column()]->patterns[pattern].contains(-1)) {
			return QVariant(store->machines[index.column()]->patterns[pattern][-1]["name"]);
		} else {
			return QVariant("Pattern "+QString::number(pattern));
		}
	}
	*/
	return QVariant();
}



QVariant TrackTableData::headerData(int section, Qt::Orientation orientation, int role) const
{
	/*
	if (role != Qt::DisplayRole) { 
		return QVariant();
	} else if (orientation == Qt::Horizontal) {
		return QVariant(store->machines[section]->name);
	} else if (orientation == Qt::Vertical) {
		return QVariant(section);
	}
	*/
	return QVariant();
}
