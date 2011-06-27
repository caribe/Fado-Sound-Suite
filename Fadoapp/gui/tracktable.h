#ifndef GUI_TRACKTABLE_H
#define GUI_TRACKTABLE_H

#include <QtGui>

class TrackTable;

#include "gui/track.h"
#include "gui/tracktabledata.h"

class TrackTable : public QTableView
{
	Q_OBJECT

	public:
		Track *track;
		Store *store;
		TrackTableData *model;

		TrackTable(Track *track);
		void refresh();
		void keyPressEvent(QKeyEvent *event);
		
		void setValue(int value);

	private slots:
		void currentColumnChangedSlot(const QModelIndex &current, const QModelIndex &previous);
};

#endif
