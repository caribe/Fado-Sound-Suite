#ifndef GUI_TRACKTABLEDATA_H
#define GUI_TRACKTABLEDATA_H

#include <QAbstractTableModel>

class TrackTableData;

#include "track.h"

class TrackTableData : public QAbstractTableModel
{
	Q_OBJECT

	public:
		Core *core;
		Track *track;

		TrackTableData(Track *track, Core *core);
		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		int columnCount(const QModelIndex& parent = QModelIndex()) const;
		QVariant data(const QModelIndex& index, int role) const;

		void reload();

		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif
