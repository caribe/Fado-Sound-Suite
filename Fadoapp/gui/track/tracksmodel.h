#ifndef GUI_TRACKTABLEDATA_H
#define GUI_TRACKTABLEDATA_H

#include <QAbstractTableModel>

class TracksModel;

#include "tracks.h"

class TracksModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
		Core *core;
		Tracks *track;

		TracksModel(Tracks *track, Core *core);
		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		int columnCount(const QModelIndex& parent = QModelIndex()) const;
		QVariant data(const QModelIndex& index, int role) const;

		QVariant headerData(int section, Qt::Orientation orientation, int role) const;

		void reload();
};

#endif
