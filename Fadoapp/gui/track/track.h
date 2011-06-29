#ifndef GUI_TRACK_H
#define GUI_TRACK_H

#include <QFrame>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class Track;

#include "gears/master.h"
#include "tracktable.h"

class Track : public QFrame
{
	Q_OBJECT

	public:
		Track(Core *core);
		
		Core *core;
		QListWidget *hintList;
		TrackTable *tracksTable;
		int activeMachine;

		void refreshMachines();
		void refreshPatterns();

		QHash<QString, int> mapping;

	private slots:
		void itemDoubleClickedSlot(QListWidgetItem *item);
		void addButtonSlot();
		void delButtonSlot();
		void frsButtonSlot();
		void lstButtonSlot();
};

#endif
