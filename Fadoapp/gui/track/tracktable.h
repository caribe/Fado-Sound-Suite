#ifndef GUI_TRACKTABLE_H
#define GUI_TRACKTABLE_H

#include <QTableView>
#include <QKeyEvent>

class TrackTable;

#include "track.h"
#include "tracktabledata.h"

class TrackTable : public QTableView
{
	Q_OBJECT

	public:
		Track *track;
		Core *core;
		TrackTableData *model;

		TrackTable(Track *track);
		void refresh();
		void keyPressEvent(QKeyEvent *event);
		
		void setValue(int value);

	private slots:
		void currentColumnChangedSlot(const QModelIndex &current, const QModelIndex &previous);
};

#endif
