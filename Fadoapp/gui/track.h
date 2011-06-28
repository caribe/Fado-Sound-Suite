#ifndef GUI_TRACK_H
#define GUI_TRACK_H

#include <QFrame>
#include <QListWidget>
#include <QPushButton>

class Track;

#include "mainwindow.h"
#include "gui/tracktable.h"

class Track : public QFrame
{
	Q_OBJECT

	public:
		Track(MainWindow *parent);
		
		MainWindow *parent;
		Store *store;
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
